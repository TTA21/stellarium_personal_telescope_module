/*
 * Copyright (C) 2015 Kirill Snezhko
 * Copyright (C) 2018 Alexander Wolf
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

#ifndef DYNAMICPLUGINTEMPLATEWINDOW_HPP
#define DYNAMICPLUGINTEMPLATEWINDOW_HPP

#include "StelDialog.hpp"
#include "../DynamicPluginTemplate.hpp"

#include <QScrollBar>

#include <QSerialPort>
#include <QSerialPortInfo>  // For listing available ports
#include <QTimer>
#include "StelObjectMgr.hpp"
#include "StelObject.hpp"
#include "StelCore.hpp"
#include "StelUtils.hpp"
#include <QWidget>
#include <QCoreApplication>

#include "StelPainter.hpp"
#include "StelProjector.hpp"

#include <vlc/vlc.h>

class Ui_dynamicPluginTemplateWindowForm;

struct MotorParameters {
    double speed;
    double accel;
    int microstep;
    double reduction;
    int motorSteps;
    double positiveBacklash;
    double negativeBacklash;
    int inverted;
};

struct MotorPreset {
    MotorParameters altMotor;
    MotorParameters azMotor;
    MotorParameters fRotMotor;
    MotorParameters focMotor;

    QString presetName;
};

class DynamicPluginTemplateWindow : public StelDialog
{
    Q_OBJECT

public:
    DynamicPluginTemplateWindow();
    ~DynamicPluginTemplateWindow();

    void drawReticleSensor(StelCore* core, StelPainter& painter);

public slots:
    void retranslate();

protected:
    void createDialogContent();

    void moveTerminalScrollToBottom();

    void tick();    //Sends stuff to esp, tells coordinates, asks sensor data, etc
    void tickInternal();    //Anything that doenst connect to an external source

    void updateSelectedObjLabel();

    void updateMotorSettings(QString settings);

    void loadMotorPresetsFromMemory();

    void saveMotorPresets();
    void loadMotorPresets();
    void saveMotorParameters(QSettings& settings, const MotorParameters& params);
    MotorParameters loadMotorParameters(QSettings& settings);

    double calculateFieldRotation(StelObjectP obj, StelCore* core);

private slots:

    void on_buttonNudgeLeft_clicked(void);

    void on_buttonNudgeUp_clicked(void);

    void on_buttonNudgeDown_clicked(void);

    void on_buttonNudgeRight_clicked(void);

    void on_nudgeSlider_sliderReleased(void);

    void on_nudgeSlider_valueChanged(int value);

    void on_buttonFRotCCW_clicked(void);

    void on_buttonFRotCW_clicked(void);

    void on_listSerialPortsButton_clicked(void);

    void on_buttonConnectSerial_clicked(void);

    void on_buttonTerminalSend_clicked(void);

    void on_buttonClearTerminal_clicked(void);

    void handleSerialRead();
    void handleSerialError(QSerialPort::SerialPortError error);

    void on_pushButtonTest_clicked(void);

    void on_buttonTrackToggle_clicked(void);
    void sendCoordinatesToMount();

    void on_buttonToggleSensor_clicked(void);

    void on_nudgeDoubleSpinBox_valueChanged(double arg1);

    void on_sendMotorParamsPushButton_clicked(void);

    void on_requestMotorUpdatePushButton_clicked(void);

    void updateSettingsPanel(void);

    void on_selectedMotorComboBox_currentIndexChanged(int);

    void on_updatePresetPushButton_clicked(void);

    void on_savePresetPushButton_clicked(void);

    void on_settingsPresetComboBox_currentIndexChanged(int);

    void on_deletePresetPushButton_clicked(void);


    void on_focusPushButton_clicked(void);

    void on_homeFocusPushButton_clicked(void);


private:
    Ui_dynamicPluginTemplateWindowForm *ui;
    DynamicPluginTemplate *dynamicPluginTemplate;

    double nudgeValue;
    bool isConnectedToSerialPort;
    QSerialPort *serial;
    void writeToSerial(const QString &data, const bool writeToTerminal);
    QTimer *coordinateTimer;
    QTimer *coordinateTimerInternal;

    bool isTracking;

    double sensorAlt; //sensors
    double sensorAz;
    bool showReticleSensors;

    unsigned int tickCounter;
    unsigned int tickRequestSensorHit;

    unsigned int tickCounterInternal;

    QVector<MotorPreset> motorPresets;
    unsigned int motorPresetIndex;

    bool hasHomedFocus;
};

#endif // DYNAMICPLUGINTEMPLATEWINDOW_HPP
