#include "DynamicPluginTemplate.hpp"

#include "DynamicPluginTemplateWindow.hpp"
#include "ui_dynamicPluginTemplateWindow.h"

#include "StelApp.hpp"
#include "StelTranslator.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"

#include <QRegularExpression>

/**
 * Serial link hardening notes
 * ---------------------------
 * The ESP32 (Arduino core) terminates lines with a bare '\n' and can split
 * responses across multiple UART chunks or bundle several of them into one.
 * We therefore:
 *   - accumulate raw bytes in serialLineBuffer and only parse complete lines
 *   - validate every parsed field before touching sensor state
 *   - funnel every failure mode through handleDisconnection(), which is
 *     idempotent (reports the transition once, never spams) and always
 *     leaves the UI flags/timer/button in a consistent "disconnected" state
 *   - guard every slot against a half-destroyed window (uiDestroyed), which
 *     is what QSerialPort signals can hit during Stellarium shutdown while
 *     the port child object is still tearing down.
 */

void DynamicPluginTemplateWindow::handleSerialError(QSerialPort::SerialPortError error)
{
    // Emitted with NoError on some Qt versions during close(); ignore.
    if (error == QSerialPort::NoError) return;

    if (uiDestroyed || !serial) return;

    const QString message = serial->errorString();
    qWarning() << "Serial port error:" << error << message;

    // IMPORTANT: do NOT close the port from this slot. Qt emits these
    // signals from inside its socket-notifier activation code; closing the
    // port synchronously re-enters that machinery while it is mid-flight
    // and can crash it on a hot-unplug (observed: segfault on cable wiggle).
    // Defer to a queued event so the teardown runs in a clean event-loop
    // iteration, after Qt's own processing has fully unwound.
    QMetaObject::invokeMethod(this, [this, message] {
        processSerialError(message);
    }, Qt::QueuedConnection);
}

void DynamicPluginTemplateWindow::processSerialError(const QString &errorString)
{
    if (uiDestroyed || !serial) return;

    handleDisconnection("ERROR: Device disconnected! (" + errorString + ")");
}

void DynamicPluginTemplateWindow::handleDisconnection(const QString &reason)
{
    // Never touch the UI once destruction has started (serial is a child of
    // this window and its destructor can emit signals while we are gone).
    if (uiDestroyed || !dialog) return;

    const bool wasConnected = isConnectedToSerialPort || serial->isOpen();

    // 1. Tear down the link
    serialLineBuffer.clear();
    if (serial->isOpen()) {
        serial->clear();      // drop anything still in the read buffer
        serial->close();
    }

    // 2. Consistent state, regardless of which path triggered this
    isConnectedToSerialPort = false;

    if (coordinateTimer->isActive()) {
        coordinateTimer->stop();
    }
    if (isTracking) {
        isTracking = false;
        ui->buttonTrackToggle->setText("Track");
    }
    if (ui->buttonConnectSerial->text() != "Connect") {
        ui->buttonConnectSerial->setText("Connect");
    }

    // 3. Report the transition once (idempotent: silent when already disconnected)
    if (wasConnected) {
        ui->serialTerminalTextBrowser->append("<font color='red'>" + reason + "</font>");
        moveTerminalScrollToBottom();

        // The /dev node of a dropped USB device is gone; rescan so the user
        // can hit Connect as soon as the port reappears.
        refreshSerialPortList();
    }
}

void DynamicPluginTemplateWindow::handleSerialRead()
{
    if (uiDestroyed || !serial) return;

    if (!serial->isOpen()) {
        handleDisconnection("ERROR: Connection lost!");
        return;
    }

    serialLineBuffer.append(serial->readAll());

    // Guard against unbounded growth if the device spews data without line
    // endings (brown-out garbage, baud rate mismatch, ...).
    if (serialLineBuffer.size() > 4096) {
        if (!uiDestroyed && dialog) {
            ui->serialTerminalTextBrowser->append("<font color='red'>WARNING: Dropping serial data without line endings</font>");
            moveTerminalScrollToBottom();
        }
        serialLineBuffer.clear();
        return;
    }

    // Process only complete lines; keep the tail for the next burst.
    int nl;
    while ((nl = serialLineBuffer.indexOf('\n')) != -1) {
        QByteArray lineBytes = serialLineBuffer.left(nl);
        serialLineBuffer.remove(0, nl + 1);
        while (lineBytes.endsWith('\r')) lineBytes.chop(1); // tolerate CRLF
        handleSerialLine(lineBytes);
    }
}

void DynamicPluginTemplateWindow::handleSerialLine(const QByteArray &lineBytes)
{
    if (uiDestroyed || !dialog) return;

    const QString line = QString::fromUtf8(lineBytes).trimmed();
    if (line.isEmpty()) return; // firmware sends a trailing blank line after S/P

    ui->serialTerminalTextBrowser->append("<font color='cyan'>RX: " + line + "</font>");
    moveTerminalScrollToBottom();

    if (line.startsWith("S")) {
        const QStringList args = line.split(' ', Qt::SkipEmptyParts);

        if (args.size() >= 3) {
            bool okAz = false;
            bool okAlt = false;
            const double az = args.at(1).toDouble(&okAz);
            const double alt = args.at(2).toDouble(&okAlt);

            if (okAz && okAlt && qIsFinite(az) && qIsFinite(alt)) {
                sensorAz = az;
                sensorAlt = alt;

                ui->sensorAltAzLabel->setText(QString("Az: %1  Alt: %2")
                    .arg(sensorAz, 0, 'f', 5)
                    .arg(sensorAlt, 0, 'f', 5));
            } else {
                ui->serialTerminalTextBrowser->append("<font color='red'>WARNING: Malformed S response: " + line + "</font>");
                moveTerminalScrollToBottom();
            }
        }
        // "S" without arguments: firmware echoes nothing; ignore silently.
    }

    // 'P' lines are logged to the terminal above; no state to update.
}

bool DynamicPluginTemplateWindow::writeToSerial(const QString &data, const bool writeToTerminal)
{
    if (uiDestroyed || !serial) return false;

    if (!serial->isOpen()) {
        handleDisconnection("ERROR: Connection lost!");
        return false;
    }

    const QByteArray byteData = (data + "\n").toUtf8();

    // Write everything; the write buffer can be smaller than the payload.
    qint64 written = 0;
    while (written < byteData.size()) {
        const qint64 chunk = serial->write(byteData.constData() + written,
                                           byteData.size() - written);
        if (chunk <= 0) break; // device gone or write error
        written += chunk;
        if (written < byteData.size() && !serial->waitForBytesWritten(100)) break;
    }

    if (written == byteData.size()) {
        if (writeToTerminal && !uiDestroyed && dialog) {
            ui->serialTerminalTextBrowser->append("<font color='blue'>TX: " + data + "</font>");
            moveTerminalScrollToBottom();
        }
        return true;
    }

    qWarning() << "Serial write failed:" << serial->errorString();
    handleDisconnection("ERROR: Write failed: " + serial->errorString());
    return false;
}

double DynamicPluginTemplateWindow::calculateFieldRotation(StelObjectP obj, StelCore* core)
{
    Vec3d posEquatorial = obj->getEquinoxEquatorialPos(core);
    double ra, dec;
    StelUtils::rectToSphe(&ra, &dec, posEquatorial);

    // Get observer latitude
    double latitude = core->getCurrentLocation().getLatitude() * M_PI / 180.0;

    // Calculate parallactic angle (field rotation for alt-az mount)
    // Formula: tan(q) = sin(H) / (tan(lat) * cos(dec) - sin(dec) * cos(H))
    // where H is the hour angle

    double hourAngle = core->getLocalSiderealTime() - ra;

    double sinH = sin(hourAngle);
    double cosH = cos(hourAngle);
    double sinDec = sin(dec);
    double cosDec = cos(dec);
    double tanLat = tan(latitude);

    double fieldRotation = 0.0;

    // Avoid division by zero and handle special cases
    double denominator = tanLat * cosDec - sinDec * cosH;

    if (fabs(cosDec) < 1e-6) {
        // Object is very close to celestial pole
        fieldRotation = 0.0;
    } else {
        fieldRotation = atan2(sinH, denominator) * 180.0 / M_PI;
    }

    // Normalize to 0-360 range
    fieldRotation = fmod(fieldRotation + 360.0, 360.0);

    return fieldRotation;
}

void DynamicPluginTemplateWindow::sendCoordinatesToMount()
{
    if (!serial || !serial->isOpen()) {
        handleDisconnection("Tracking stopped: Serial connection lost");
        return;
    }

    StelObjectMgr* objectMgr = GETSTELMODULE(StelObjectMgr);
    QList<StelObjectP> selectedObjects = objectMgr->getSelectedObject();
    if (selectedObjects.isEmpty()) {
        return;
    }

    StelObjectP obj = selectedObjects.first();
    StelCore* core = StelApp::getInstance().getCore();

    // Get Alt/Az
    Vec3d posAltAz = obj->getAltAzPosAuto(core);
    double az, alt;
    StelUtils::rectToSphe(&az, &alt, posAltAz);

    // Convert to degrees
    double azDeg = fmod(180.0 - az * 180.0 / M_PI + 360.0, 360.0);
    double altDeg = alt * 180.0 / M_PI;
    double fieldRotation = calculateFieldRotation(obj, core);

    // Format and send
    QString command = QString("T %1 %2 %3")
                        .arg(azDeg, 0, 'f', 5)
                        .arg(altDeg, 0, 'f', 5)
                        .arg(fieldRotation, 0, 'f', 5);

    ui->lastAltAzLabel->setText( QString("Az: %1  Alt: %2  FRot: %3")
                                    .arg(azDeg, 0, 'f', 5)
                                    .arg(altDeg, 0, 'f', 5)
                                    .arg(fieldRotation, 0, 'f', 5) );

    ui->objectiveSensorDiffLabel->setText( QString("Az: %1  Alt: %2  FRot: %3")
                                            .arg(abs(azDeg - sensorAz), 0, 'f', 5)
                                            .arg(abs(altDeg - sensorAlt), 0, 'f', 5));
    writeToSerial(command, false);
}
