#include "DynamicPluginTemplate.hpp"

#include "DynamicPluginTemplateWindow.hpp"
#include "ui_dynamicPluginTemplateWindow.h"

#include "StelApp.hpp"
#include "StelTranslator.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"


void DynamicPluginTemplateWindow::on_buttonNudgeLeft_clicked(void)
{

    qDebug() << "Left Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }

    writeToSerial("N AZ " + QString::number(-nudgeValue), true);
}


void DynamicPluginTemplateWindow::on_buttonNudgeUp_clicked(void)
{

    qDebug() << "Up Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }
    writeToSerial("N ALT " + QString::number(nudgeValue), true);
}


void DynamicPluginTemplateWindow::on_buttonNudgeDown_clicked(void)
{

    qDebug() << "Down Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }
    writeToSerial("N ALT " + QString::number(-nudgeValue), true);
}


void DynamicPluginTemplateWindow::on_buttonNudgeRight_clicked(void)
{

    qDebug() << "Right Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }

    writeToSerial("N AZ " + QString::number(nudgeValue), true);
}

void DynamicPluginTemplateWindow::on_buttonFRotCCW_clicked(void)
{

    qDebug() << "FRot CCW Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }

    writeToSerial("N FROT " + QString::number(-nudgeValue), true);
}


void DynamicPluginTemplateWindow::on_buttonFRotCW_clicked(void)
{

    qDebug() << "FRot CW Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot nudge, serial port disconnected </font>");
        return;
    }

    writeToSerial("N FROT " + QString::number(nudgeValue), true);
}

void DynamicPluginTemplateWindow::on_buttonTrackToggle_clicked(void)
{
    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot track, serial port disconnected </font>");
        return;
    }

    if(!isTracking){
        ui->buttonTrackToggle->setText("Stop Tracking");
        isTracking = true;
        //showReticleSensors = true;
        coordinateTimer->start(10);
        qDebug() << "Started tracking";
    }else{
        ui->buttonTrackToggle->setText("Track");
        isTracking = false;
        //showReticleSensors = false;
        coordinateTimer->stop();
        qDebug() << "Stopped tracking";
    }
}

void DynamicPluginTemplateWindow::drawReticleSensor(StelCore* core, StelPainter& painter)
{
    if (!showReticleSensors || (sensorAz == 0 && sensorAlt == 0)) {
        return;
    }

    // Convert Alt/Az degrees to radians
    double azRad = sensorAz * M_PI / 180.0;
    double altRad = sensorAlt * M_PI / 180.0;

    // Convert spherical to rectangular coordinates
    Vec3d pos;
    StelUtils::spheToRect(azRad, altRad, pos);

    // Project to screen coordinates
    Vec3d screenPos;
    if (!painter.getProjector()->project(pos, screenPos)) {
        return; // Position not visible
    }

    // Draw the reticle
    painter.setColor(1.0f, 0.0f, 0.0f, 1.0f); // Red color

    // Draw crosshair
    float size = 20.0f;
    float x = screenPos[0];
    float y = screenPos[1];

    // Horizontal line
    painter.drawLine2d(x - size, y, x + size, y);
    // Vertical line
    painter.drawLine2d(x, y - size, x, y + size);

    // Draw circle around crosshair
    painter.drawCircle(x, y, size * 0.7f);
}

void DynamicPluginTemplateWindow::on_buttonToggleSensor_clicked(void)
{
    showReticleSensors = !showReticleSensors;
}

void DynamicPluginTemplateWindow::on_nudgeDoubleSpinBox_valueChanged(double arg1)
{
    qDebug() << "nudge double spinbox updated";
    nudgeValue = arg1;
}


void DynamicPluginTemplateWindow::on_focusPushButton_clicked(void)
{
    qDebug() << "Focus Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot focus, serial port disconnected </font>");
        return;
    }

    if(!hasHomedFocus){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot focus, necessary to home focus first </font>");
        return;
    }

    const double focusVal = ui->focuserDoubleSpinBox->value();

    writeToSerial("N FOC " + QString::number(focusVal), true);
}

void DynamicPluginTemplateWindow::on_homeFocusPushButton_clicked(void)
{
    qDebug() << "Focus Home Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot home focus, serial port disconnected </font>");
        return;
    }

    hasHomedFocus = true;
    writeToSerial("N HFOC 0", true);
}





