#include "DynamicPluginTemplate.hpp"

#include "DynamicPluginTemplateWindow.hpp"
#include "ui_dynamicPluginTemplateWindow.h"

#include "StelApp.hpp"
#include "StelTranslator.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"


void DynamicPluginTemplateWindow::handleSerialError(QSerialPort::SerialPortError error)
{
    // Ignore NoError
    if (error == QSerialPort::NoError) return;

    isConnectedToSerialPort = false;
    ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: Device disconnected!</font>");
    ui->buttonConnectSerial->setText("Connect");
    serial->close();
    moveTerminalScrollToBottom();

    qDebug() << "Serial port error:" << error << serial->errorString();
}

void DynamicPluginTemplateWindow::handleSerialRead()
{
    if (!serial->isOpen()) {
        isConnectedToSerialPort = false;
        ui->serialTerminalTextBrowser->append("<font color='red'>Connection lost!</font>");
        ui->buttonConnectSerial->setText("Connect");
        moveTerminalScrollToBottom();
        return;
    }

    QByteArray data = serial->readAll();
    QString text = QString::fromUtf8(data);
    ui->serialTerminalTextBrowser->append("<font color='cyan'>RX: " + text + "</font>");
    moveTerminalScrollToBottom();

    #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        QStringList lines = text.split("\r\n", Qt::SkipEmptyParts);
    #else
        QStringList lines = text.split("\r\n", QString::SkipEmptyParts);
    #endif

    for (const QString& line : lines) {

        if (line.startsWith("S")) {
            QStringList Sargument = line.split(' ');

            if (Sargument.size() >= 3) {
                sensorAz = Sargument.at(1).toDouble();
                sensorAlt = Sargument.at(2).toDouble();

                ui->sensorAltAzLabel->setText(QString("Az: %1  Alt: %2")
                                                  .arg(sensorAz, 0, 'f', 5)
                                                  .arg(sensorAlt, 0, 'f', 5));
            }
        }

        if(line.startsWith("P")){
            //updateMotorSettings(line);    //Send only, no need to recieve
        }

    }

}

void DynamicPluginTemplateWindow::writeToSerial(const QString &data, const bool writeToTerminal)
{
    if (!serial->isOpen()) {
        isConnectedToSerialPort = false;
        ui->serialTerminalTextBrowser->append("<font color='red'>Connection lost!</font>");
        ui->buttonConnectSerial->setText("Connect");
        moveTerminalScrollToBottom();
        return;
    }
    if (serial && serial->isOpen()) {
        QString dataWithNewline = data + "\n";
        QByteArray byteData = dataWithNewline.toUtf8();
        qint64 bytesWritten = serial->write(byteData);

        if (bytesWritten != -1) {
            if(writeToTerminal) ui->serialTerminalTextBrowser->append("<font color='blue'>TX: " + data + "</font>");
        } else {
            ui->serialTerminalTextBrowser->append("<font color='red'>Write failed: " + serial->errorString() + "</font>");
        }
        moveTerminalScrollToBottom();
    }
}

double DynamicPluginTemplateWindow::calculateFieldRotation(StelObjectP obj, StelCore* core){

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
    if (!serial || !serial->isOpen())
    {
        coordinateTimer->stop();
        ui->serialTerminalTextBrowser->append("<font color='red'>Tracking stopped: Serial connection lost</font>");
        ui->buttonConnectSerial->setText("Connect");
        moveTerminalScrollToBottom();
        isConnectedToSerialPort = false;
        qDebug() << "Tracking stopped due to lost connection";
        return;
    }
    StelObjectMgr* objectMgr = GETSTELMODULE(StelObjectMgr);
    QList<StelObjectP> selectedObjects = objectMgr->getSelectedObject();
    if (!selectedObjects.isEmpty() && serial && serial->isOpen())
    {
        StelObjectP obj = selectedObjects[0];
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
}
