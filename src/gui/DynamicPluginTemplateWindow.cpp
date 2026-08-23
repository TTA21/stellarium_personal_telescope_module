/*
 * Copyright (C) 2015 Kirill Snezhko
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#include "DynamicPluginTemplate.hpp"

#include "DynamicPluginTemplateWindow.hpp"
#include "ui_dynamicPluginTemplateWindow.h"

#include "StelApp.hpp"
#include "StelTranslator.hpp"
#include "StelLocaleMgr.hpp"
#include "StelModuleMgr.hpp"

#include <QThread>

//ffplay -fflags nobuffer -vf "drawbox=x=(iw-1)/2:y=0:w=1:h=ih:color=red:t=fill,drawbox=x=0:y=(ih-1)/2:w=iw:h=1:color=red:t=fill" -an -x 600 -y 400 rtsp://192.168.59.1:7070/webcam

DynamicPluginTemplateWindow::DynamicPluginTemplateWindow():
    nudgeValue(0.1),
    isConnectedToSerialPort(false),
    serial(nullptr),
    uiDestroyed(false),
    isTracking(false),
    sensorAlt(0),
    sensorAz(0),
    showReticleSensors(false),
    tickCounter(0),
    tickRequestSensorHit(5),
    tickCounterInternal(0),
    motorPresetIndex(0)
{
    ui = new Ui_dynamicPluginTemplateWindowForm();

    MotorParameters defaultParamsAz = MotorParameters();
    defaultParamsAz.accel = 20000;
    defaultParamsAz.speed = 20000;
    defaultParamsAz.microstep = 256;
    defaultParamsAz.reduction = 25;
    defaultParamsAz.motorSteps = 200;
    defaultParamsAz.negativeBacklash = 0;
    defaultParamsAz.positiveBacklash = 0;
    defaultParamsAz.inverted = 0;

    MotorParameters defaultParamsAlt = MotorParameters();
    defaultParamsAlt.accel = 20000;
    defaultParamsAlt.speed = 20000;
    defaultParamsAlt.microstep = 256;
    defaultParamsAlt.reduction = 25;
    defaultParamsAlt.motorSteps = 200;
    defaultParamsAlt.negativeBacklash = 0;
    defaultParamsAlt.positiveBacklash = 0;
    defaultParamsAlt.inverted = 1;

    MotorParameters defaultParamsFRot = MotorParameters();
    defaultParamsFRot.accel = 20000;
    defaultParamsFRot.speed = 20000;
    defaultParamsFRot.microstep = 32;
    defaultParamsFRot.reduction = 99.5;
    defaultParamsFRot.motorSteps = 200;
    defaultParamsFRot.negativeBacklash = 0;
    defaultParamsFRot.positiveBacklash = 0;
    defaultParamsFRot.inverted = 0;

    MotorParameters defaultParamsFocus = MotorParameters();
    defaultParamsFocus.accel = 1000;
    defaultParamsFocus.speed = 500;
    defaultParamsFocus.microstep = 256;
    defaultParamsFocus.reduction = 1;
    defaultParamsFocus.motorSteps = 200;
    defaultParamsFocus.negativeBacklash = 0;
    defaultParamsFocus.positiveBacklash = 0;
    defaultParamsFocus.inverted = 0;

    MotorPreset defaultPreset = MotorPreset();
    defaultPreset.altMotor = defaultParamsAlt;
    defaultPreset.azMotor = defaultParamsAz;
    defaultPreset.fRotMotor = defaultParamsFRot;
    defaultPreset.focMotor = defaultParamsFocus;
    defaultPreset.presetName = "Default Generated";

    motorPresets.append(defaultPreset);

    coordinateTimer = new QTimer(this);
    coordinateTimerInternal = new QTimer(this);
    connect(coordinateTimer, &QTimer::timeout, this, &DynamicPluginTemplateWindow::tick);
    connect(coordinateTimerInternal, &QTimer::timeout, this, &DynamicPluginTemplateWindow::tickInternal);

    // Create the port up-front and wire its signals exactly once.
    // (Previously the port was created lazily inside the connect button and
    // re-connected on every open, entangling the signal lifecycle with the UI.)
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &DynamicPluginTemplateWindow::handleSerialRead);
    connect(serial, &QSerialPort::errorOccurred, this, &DynamicPluginTemplateWindow::handleSerialError);
}

DynamicPluginTemplateWindow::~DynamicPluginTemplateWindow()
{
    // Mark destroyed FIRST: QSerialPort is a child of this window and can
    // emit errorOccurred/readyRead while Qt tears it down (especially after
    // a USB drop or while Stellarium is quitting). Any such signal must not
    // touch the UI we are about to free.
    uiDestroyed = true;

    coordinateTimer->stop();
    coordinateTimerInternal->stop();

    if (serial) {
        serial->disconnect(this);   // only signals FROM the port to this window
        if (serial->isOpen()) {
            serial->clear();
            serial->close();
        }
        // serial is a child of this window; Qt deletes it with us.
    }

    delete ui;
    ui = nullptr;
}

void DynamicPluginTemplateWindow::retranslate()
{
	if (dialog)
	{
		ui->retranslateUi(dialog);
	}
}

void DynamicPluginTemplateWindow::moveTerminalScrollToBottom(){
    QScrollBar *sb = ui->serialTerminalTextBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void DynamicPluginTemplateWindow::createDialogContent()
{
    ui->setupUi(dialog);
	connect(&StelApp::getInstance(), SIGNAL(languageChanged()), this, SLOT(retranslate()));

    //loadMotorPresetsFromMemory();
    loadMotorPresets();

    //Tab1
    connect(ui->buttonNudgeLeft, SIGNAL(clicked()), this, SLOT(on_buttonNudgeLeft_clicked()));
    connect(ui->buttonNudgeUp, SIGNAL(clicked()), this, SLOT(on_buttonNudgeUp_clicked()));
    connect(ui->buttonNudgeDown, SIGNAL(clicked()), this, SLOT(on_buttonNudgeDown_clicked()));
    connect(ui->buttonNudgeRight, SIGNAL(clicked()), this, SLOT(on_buttonNudgeRight_clicked()));
    connect(ui->buttonFRotCCW, SIGNAL(clicked()), this, SLOT(on_buttonFRotCCW_clicked()));
    connect(ui->buttonFRotCW, SIGNAL(clicked()), this, SLOT(on_buttonFRotCW_clicked()));
    connect(ui->buttonTrackToggle, SIGNAL(clicked()), this, SLOT(on_buttonTrackToggle_clicked()));
    connect(ui->buttonToggleSensor, SIGNAL(clicked()), this, SLOT(on_buttonToggleSensor_clicked()));
    connect(ui->nudgeDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(on_nudgeDoubleSpinBox_valueChanged(double)));
    connect(ui->focusPushButton, SIGNAL(clicked()), this, SLOT(on_focusPushButton_clicked()));
    connect(ui->homeFocusPushButton, SIGNAL(clicked()), this, SLOT(on_homeFocusPushButton_clicked()));

    //tab2
    connect(ui->listSerialPortsButton, SIGNAL(clicked()), this, SLOT(on_listSerialPortsButton_clicked()));
    connect(ui->buttonConnectSerial, SIGNAL(clicked()), this, SLOT(on_buttonConnectSerial_clicked()));
    connect(ui->buttonTerminalSend, SIGNAL(clicked()), this, SLOT(on_buttonTerminalSend_clicked()));
    // Allow Enter in the input field to send, like a normal terminal
    connect(ui->terminalSendLineEdit, SIGNAL(returnPressed()), this, SLOT(on_buttonTerminalSend_clicked()));
    connect(ui->buttonClearTerminal, SIGNAL(clicked()), this, SLOT(on_buttonClearTerminal_clicked()));

    //tab3
    connect(ui->pushButtonTest, SIGNAL(clicked()), this, SLOT(on_pushButtonTest_clicked()));
    connect(ui->sendMotorParamsPushButton, SIGNAL(clicked()), this, SLOT(on_sendMotorParamsPushButton_clicked()));
    connect(ui->requestMotorUpdatePushButton, SIGNAL(clicked()), this, SLOT(on_requestMotorUpdatePushButton_clicked()));
    connect(ui->selectedMotorComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_selectedMotorComboBox_currentIndexChanged(int)));
    connect(ui->updatePresetPushButton, SIGNAL(clicked()), this, SLOT(on_updatePresetPushButton_clicked()));
    connect(ui->savePresetPushButton, SIGNAL(clicked()), this, SLOT(on_savePresetPushButton_clicked()));
    connect(ui->settingsPresetComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_settingsPresetComboBox_currentIndexChanged(int)));
    connect(ui->deletePresetPushButton, SIGNAL(clicked()), this, SLOT(on_deletePresetPushButton_clicked()));

	// Title bar: close button + drag-to-move (standard StelDialog pattern).
	// movedTo saves the window position to config.ini like all other plugins.
	connect(ui->titleBar, &TitleBar::closeClicked, this, &StelDialog::close);
	connect(ui->titleBar, &TitleBar::movedTo, this, &StelDialog::handleMovedTo);

    coordinateTimerInternal->start(30);

}


void DynamicPluginTemplateWindow::refreshSerialPortList(void)
{
    // Clear existing items
    ui->serialPortComboBox->clear();

    // Get available ports
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    // Add each port to the combo box
    for (const QSerialPortInfo &port : ports) {
        qDebug() << "Port:" << port.portName() << port.description();
        ui->serialPortComboBox->addItem(port.portName());
    }

    // Optional: Show message if no ports found
    if (ports.isEmpty()) {
        qDebug() << "No serial ports found";
        ui->serialTerminalTextBrowser->append("<font color='red'>No serial terminals found</font>");
    }

    moveTerminalScrollToBottom();
}

void DynamicPluginTemplateWindow::on_listSerialPortsButton_clicked(void)
{
    qDebug() << "Serial Ports Listing Button Clicked";

    if(isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: serial port is still connected, disconect first </font>");
        return;
    }

    ui->serialTerminalTextBrowser->append("<font color='blue'>Searching for serial terminals</font>");
    refreshSerialPortList();
}


void DynamicPluginTemplateWindow::on_buttonConnectSerial_clicked(void)
{
    qDebug() << "Serial Port Connect Button Clicked";

    if(isConnectedToSerialPort){
        // Manual disconnect: same centralized teardown as an error, but a
        // neutral (non-error) message.
        handleDisconnection("Disconnected");
        refreshSerialPortList();
        return;
    }

    if (!serial) return; // cannot happen: created in the constructor

    QString selectedPort = ui->serialPortComboBox->currentText().trimmed();
    if (selectedPort.isEmpty()) {
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: No serial port selected. Search for ports first.</font>");
        moveTerminalScrollToBottom();
        return;
    }

    qDebug() << "Serial port chosen: " << selectedPort;

    ui->serialTerminalTextBrowser->append("<font color='blue'>Serial port chosen: /dev/" + selectedPort + "</font>");
    ui->serialTerminalTextBrowser->append("<font color='blue'>Attempting Connection</font>");

    // Configure the port (signals were already connected once in the constructor)
    serial->setPortName("/dev/" + selectedPort);
    serial->setBaudRate(QSerialPort::Baud115200);  // Adjust as needed
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Attempt to open
    if (serial->open(QIODevice::ReadWrite)) {
        // The ESP32's native USB-Serial-JTAG port (ttyACM0, "USB Single
        // Serial") only starts routing host->device data after a DTR/RTS
        // handshake. QSerialPort does not assert these lines on open, so
        // without this the port appears connected but every command is
        // silently dropped by the chip until it is reset. Replicate what a
        // serial monitor does: drop the lines, then assert DTR.
        // (RTS stays low -> if this resets the chip it boots into the app,
        // never the download bootloader.)
        serial->setDataTerminalReady(false);
        serial->setRequestToSend(false);
        QThread::msleep(100);
        serial->setDataTerminalReady(true);
        QThread::msleep(300);
        serial->clear(); // discard any boot/reset output

        ui->buttonConnectSerial->setText("Disconnect");
        isConnectedToSerialPort = true;
        ui->serialTerminalTextBrowser->append("<font color='green'>Connected successfully!</font>");

        qDebug() << "Serial port opened successfully";
    } else {
        ui->buttonConnectSerial->setText("Connect");
        isConnectedToSerialPort = false;
        ui->serialTerminalTextBrowser->append("<font color='red'>Connection failed: " + serial->errorString() + "</font>");
        qDebug() << "Failed to open serial port:" << serial->errorString();
    }

    moveTerminalScrollToBottom();
}



void DynamicPluginTemplateWindow::on_buttonTerminalSend_clicked(void)
{
    qDebug() << "Serial Terminal Send Button Clicked";

    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: serial port is disconnected </font>");
        return;
    }

    QString message = ui->terminalSendLineEdit->text();

    qDebug() << message;

    if (writeToSerial(message, true)) {
        // Clear on success only; if the write failed keep the command so
        // the user can resend it after reconnecting.
        ui->terminalSendLineEdit->clear();
        ui->terminalSendLineEdit->setFocus();
    }
}


void DynamicPluginTemplateWindow::on_buttonClearTerminal_clicked(void)
{
    qDebug() << "Serial Terminal Clear Button Clicked";
    ui->serialTerminalTextBrowser->clear();
    moveTerminalScrollToBottom();
}


void DynamicPluginTemplateWindow::on_pushButtonTest_clicked(void)
{

}

void DynamicPluginTemplateWindow::tick(){

    if((tickCounter % tickRequestSensorHit) == 0){
        writeToSerial("S", false); //Ask for sensor data
    }

    DynamicPluginTemplateWindow::sendCoordinatesToMount();

    tickCounter++;
}

void DynamicPluginTemplateWindow::tickInternal(){
    DynamicPluginTemplateWindow::updateSelectedObjLabel();
    tickCounterInternal++;
}

void DynamicPluginTemplateWindow::updateSelectedObjLabel(){

    StelObjectMgr* objectMgr = GETSTELMODULE(StelObjectMgr);
    QList<StelObjectP> selectedObjects = objectMgr->getSelectedObject();

    if(selectedObjects.isEmpty()){
        ui->selectedObjLabel->setText( QString("Az: NAN  Alt: NAN"));
    }

    if (!selectedObjects.isEmpty()){
        StelObjectP obj = selectedObjects[0];
        StelCore* core = StelApp::getInstance().getCore();

        // Get Alt/Az
        Vec3d posAltAz = obj->getAltAzPosAuto(core);
        double az, alt;
        StelUtils::rectToSphe(&az, &alt, posAltAz);

        // Convert to degrees
        double azDeg = fmod(180.0 - az * 180.0 / M_PI + 360.0, 360.0);
        double altDeg = alt * 180.0 / M_PI;
        //To save processing power, this is disabled on the ui, but sent to the mount
        //double fieldRotation = calculateFieldRotation(obj, core);

        ui->selectedObjLabel->setText( QString("Az: %1  Alt: %2")
                                        .arg(azDeg, 0, 'f', 5)
                                        .arg(altDeg, 0, 'f', 5));
    }
}









