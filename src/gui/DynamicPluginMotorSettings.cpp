#include "DynamicPluginTemplate.hpp"

#include "DynamicPluginTemplateWindow.hpp"
#include "ui_dynamicPluginTemplateWindow.h"

#include "StelApp.hpp"

void DynamicPluginTemplateWindow::on_sendMotorParamsPushButton_clicked(void)
{
    /*
    char* axis, float speed, float accel, int microstep,
   float reduction, int motorSteps, float positiveBacklash,
   float negativeBacklash, int homingPin, int inverted
    */
    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot send cfg params, serial port disconnected </font>");
        return;
    }
    MotorPreset selectedPreset;
    try {
        selectedPreset = motorPresets.at(motorPresetIndex);
    } catch (const std::out_of_range& e) {
        ui->serialTerminalTextBrowser->append("<font color='red'> selected preset does not exist </font>");
        return;
    }

    QString Caz = "C AZ,";
    Caz += QString::number(selectedPreset.azMotor.speed) + ",";
    Caz += QString::number(selectedPreset.azMotor.accel) + ",";
    Caz += QString::number(selectedPreset.azMotor.microstep) + ",";
    Caz += QString::number(selectedPreset.azMotor.reduction) + ",";
    Caz += QString::number(selectedPreset.azMotor.motorSteps) + ",";
    Caz += QString::number(selectedPreset.azMotor.positiveBacklash) + ",";
    Caz += QString::number(selectedPreset.azMotor.negativeBacklash) + ",";
    Caz += "34,";
    Caz += QString::number(selectedPreset.azMotor.inverted);

    QString Calt = "C ALT,";
    Calt += QString::number(selectedPreset.altMotor.speed) + ",";
    Calt += QString::number(selectedPreset.altMotor.accel) + ",";
    Calt += QString::number(selectedPreset.altMotor.microstep) + ",";
    Calt += QString::number(selectedPreset.altMotor.reduction) + ",";
    Calt += QString::number(selectedPreset.altMotor.motorSteps) + ",";
    Calt += QString::number(selectedPreset.altMotor.positiveBacklash) + ",";
    Calt += QString::number(selectedPreset.altMotor.negativeBacklash) + ",";
    Calt += "34,";
    Calt += QString::number(selectedPreset.altMotor.inverted);

    QString Cfrot = "C FROT,";
    Cfrot += QString::number(selectedPreset.fRotMotor.speed) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.accel) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.microstep) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.reduction) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.motorSteps) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.positiveBacklash) + ",";
    Cfrot += QString::number(selectedPreset.fRotMotor.negativeBacklash) + ",";
    Cfrot += "34,";
    Cfrot += QString::number(selectedPreset.fRotMotor.inverted);

    QString Cfoc = "C FOC,";
    Cfoc += QString::number(selectedPreset.focMotor.speed) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.accel) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.microstep) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.reduction) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.motorSteps) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.positiveBacklash) + ",";
    Cfoc += QString::number(selectedPreset.focMotor.negativeBacklash) + ",";
    Cfoc += "34,";
    Cfoc += QString::number(selectedPreset.focMotor.inverted);

    writeToSerial(Caz, true);
    writeToSerial(Calt, true);
    writeToSerial(Cfrot, true);
    writeToSerial(Cfoc, true);
}


void DynamicPluginTemplateWindow::on_requestMotorUpdatePushButton_clicked(void)
{
    if(!isConnectedToSerialPort){
        ui->serialTerminalTextBrowser->append("<font color='red'>ERROR: cannot request motor update, serial port disconnected </font>");
        return;
    }

    writeToSerial("P", true);
}

void DynamicPluginTemplateWindow::updateMotorSettings(QString settings)
{
    qDebug() << "updateMotorSettings Called";

    QStringList parts = settings.split(",");

    if (parts.size() != 10) {
        qDebug() << "Invalid settings format - expected 10 parts, got" << parts.size();
        return;
    }

    // Extract and convert each variable
    QString axis = parts[0];
    float speed = parts[1].toDouble();
    float accel = parts[2].toDouble();
    int microstep = parts[3].toInt();
    float reduction = parts[4].toDouble();
    int motorSteps = parts[5].toInt();
    float positiveBacklash = parts[6].toDouble();
    float negativeBacklash = parts[7].toDouble();
    int homingPin = parts[8].toInt();
    int inverted = parts[9].toInt();

}

void DynamicPluginTemplateWindow::updateSettingsPanel(void){
    const QString selectedAxis = ui->selectedMotorComboBox->currentText();
    MotorPreset selectedPreset;
    try {
        selectedPreset = motorPresets.at(motorPresetIndex);
    } catch (const std::out_of_range& e) {
        ui->serialTerminalTextBrowser->append("<font color='red'> selected preset does not exist </font>");
        return;
    }

    MotorParameters parameters;
    if(selectedAxis == "Azimuth") parameters = selectedPreset.azMotor;
    else if(selectedAxis == "Altitude") parameters = selectedPreset.altMotor;
    else if(selectedAxis == "Field Rotation") parameters = selectedPreset.fRotMotor;
    else if(selectedAxis == "Focuser") parameters = selectedPreset.focMotor;
    else {
        ui->serialTerminalTextBrowser->append("<font color='red'> selected axis ["+ selectedAxis +"] does not exist </font>");
        return;
    }

    ui->speedDoubleSpinBox->setValue(parameters.speed);
    ui->accelerationDoubleSpinBox->setValue(parameters.accel);
    ui->reductionDoubleSpinBox->setValue(parameters.reduction);
    ui->CWBacklashDoubleSpinBox->setValue(parameters.negativeBacklash);
    ui->CCWBacklashDoubleSpinBox->setValue(parameters.positiveBacklash);

    if(parameters.microstep == 2) ui->microStepsComboBox->setCurrentIndex(0);
    if(parameters.microstep == 4) ui->microStepsComboBox->setCurrentIndex(1);
    if(parameters.microstep == 8) ui->microStepsComboBox->setCurrentIndex(2);
    if(parameters.microstep == 16) ui->microStepsComboBox->setCurrentIndex(3);
    if(parameters.microstep == 32) ui->microStepsComboBox->setCurrentIndex(4);
    if(parameters.microstep == 64) ui->microStepsComboBox->setCurrentIndex(5);
    if(parameters.microstep == 128) ui->microStepsComboBox->setCurrentIndex(6);
    if(parameters.microstep == 256) ui->microStepsComboBox->setCurrentIndex(7);

    if(parameters.motorSteps == 200) ui->motorStepsComboBox->setCurrentIndex(0);
    if(parameters.motorSteps == 400) ui->motorStepsComboBox->setCurrentIndex(1);

    if(parameters.inverted == 0) ui->rotationComboBox->setCurrentIndex(0);
    if(parameters.inverted == 1) ui->rotationComboBox->setCurrentIndex(1);

    ui->settingsPresetComboBox->setCurrentIndex(motorPresetIndex);

}

void DynamicPluginTemplateWindow::on_selectedMotorComboBox_currentIndexChanged(int index)
{
    updateSettingsPanel();
}

/*void DynamicPluginTemplateWindow::loadMotorPresetsFromMemory(){
    //TODO: store and load presets
    //Tight now will just load the standard

    ui->settingsPresetComboBox->clear();
    for(const MotorPreset& preset : motorPresets) {
        ui->settingsPresetComboBox->addItem(preset.presetName);
    }
    ui->settingsPresetComboBox->setCurrentIndex(0);

    updateSettingsPanel();
}*/

void DynamicPluginTemplateWindow::on_updatePresetPushButton_clicked(void)
{
    //save the change of the preset, then save the preset in memory
    const QString selectedAxis = ui->selectedMotorComboBox->currentText();
    MotorPreset& preset = motorPresets[ui->settingsPresetComboBox->currentIndex()];
    MotorParameters* parameters = nullptr;

    if(selectedAxis == "Azimuth") parameters = &preset.azMotor;
    else if(selectedAxis == "Altitude") parameters = &preset.altMotor;
    else if(selectedAxis == "Field Rotation") parameters = &preset.fRotMotor;
    else if(selectedAxis == "Focuser") parameters = &preset.focMotor;
    else {
        ui->serialTerminalTextBrowser->append("<font color='red'> selected axis ["+ selectedAxis +"] does not exist </font>");
        return;
    }

    const double speed = ui->speedDoubleSpinBox->value();
    const double accel = ui->accelerationDoubleSpinBox->value();
    const double reduction = ui->reductionDoubleSpinBox->value();
    const double CWBL = ui->CWBacklashDoubleSpinBox->value();
    const double CCWBL = ui->CCWBacklashDoubleSpinBox->value();
    const unsigned int microSteps = ui->microStepsComboBox->currentText().toDouble();
    const unsigned int motorSteps = ui->motorStepsComboBox->currentText().toDouble();
    const unsigned int inverted = ui->rotationComboBox->currentText() == "CW" ? 0 : 1;

    parameters->accel = accel;
    parameters->speed = speed;
    parameters->reduction = reduction;
    parameters->positiveBacklash = CCWBL;
    parameters->negativeBacklash = CWBL;
    parameters->microstep = microSteps;
    parameters->motorSteps = motorSteps;
    parameters->inverted = inverted;

    saveMotorPresets();
}

void DynamicPluginTemplateWindow::on_savePresetPushButton_clicked(void)
{
    const QString newPresetName = ui->newPresetNameLineEdit->text();

    MotorPreset selectedPreset;
    try {
        selectedPreset = motorPresets.at(motorPresetIndex);
    } catch (const std::out_of_range& e) {
        ui->serialTerminalTextBrowser->append("<font color='red'> selected preset does not exist </font>");
        return;
    }

    selectedPreset.presetName = newPresetName;

    motorPresets.append(selectedPreset);
    ui->settingsPresetComboBox->addItem(newPresetName);
    motorPresetIndex += 1;
    saveMotorPresets();
    updateSettingsPanel();
    //TODO save to memory
}

void DynamicPluginTemplateWindow::on_settingsPresetComboBox_currentIndexChanged(int index)
{
    motorPresetIndex = index;
    updateSettingsPanel();
}

void DynamicPluginTemplateWindow::on_deletePresetPushButton_clicked(void)
{
    if(motorPresetIndex == 0) return;
    motorPresets.remove(motorPresetIndex);
    ui->settingsPresetComboBox->removeItem(motorPresetIndex);
    motorPresetIndex = 0;
    saveMotorPresets();
    updateSettingsPanel();
}

void DynamicPluginTemplateWindow::saveMotorPresets()
{
    QSettings settings;

    // Clear all existing presets from settings
    settings.remove("motorPresets");

    // Start writing the array
    settings.beginWriteArray("motorPresets");

    int arrayIndex = 0;
    for (int i = 0; i < motorPresets.size(); ++i) {
        const MotorPreset& preset = motorPresets[i];

        // Skip the default generated preset
        if (preset.presetName == "Default Generated") {
            continue;
        }

        settings.setArrayIndex(arrayIndex);
        settings.setValue("presetName", preset.presetName);

        // Save alt motor
        settings.beginGroup("altMotor");
        saveMotorParameters(settings, preset.altMotor);
        settings.endGroup();

        // Save az motor
        settings.beginGroup("azMotor");
        saveMotorParameters(settings, preset.azMotor);
        settings.endGroup();

        // Save fRot motor
        settings.beginGroup("fRotMotor");
        saveMotorParameters(settings, preset.fRotMotor);
        settings.endGroup();

        // Save foc motor
        settings.beginGroup("focMotor");
        saveMotorParameters(settings, preset.focMotor);
        settings.endGroup();

        arrayIndex++;
    }

    settings.endArray();
}

void DynamicPluginTemplateWindow::saveMotorParameters(QSettings& settings, const MotorParameters& params)
{
    settings.setValue("speed", params.speed);
    settings.setValue("accel", params.accel);
    settings.setValue("microstep", params.microstep);
    settings.setValue("reduction", params.reduction);
    settings.setValue("motorSteps", params.motorSteps);
    settings.setValue("positiveBacklash", params.positiveBacklash);
    settings.setValue("negativeBacklash", params.negativeBacklash);
    settings.setValue("inverted", params.inverted);
}

void DynamicPluginTemplateWindow::loadMotorPresets()
{
    ui->settingsPresetComboBox->clear();
    ui->settingsPresetComboBox->addItem(motorPresets.first().presetName);
    QSettings settings;
    int size = settings.beginReadArray("motorPresets");

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);
        MotorPreset preset;

        preset.presetName = settings.value("presetName").toString();

        // Load alt motor
        settings.beginGroup("altMotor");
        preset.altMotor = loadMotorParameters(settings);
        settings.endGroup();

        // Load az motor
        settings.beginGroup("azMotor");
        preset.azMotor = loadMotorParameters(settings);
        settings.endGroup();

        // Load fRot motor
        settings.beginGroup("fRotMotor");
        preset.fRotMotor = loadMotorParameters(settings);
        settings.endGroup();

        // Load foc motor
        settings.beginGroup("focMotor");
        preset.focMotor = loadMotorParameters(settings);
        settings.endGroup();

        motorPresets.append(preset);
        ui->settingsPresetComboBox->addItem(preset.presetName);
    }
    ui->settingsPresetComboBox->setCurrentIndex(0);
    settings.endArray();
    updateSettingsPanel();
}

MotorParameters DynamicPluginTemplateWindow::loadMotorParameters(QSettings& settings)
{
    MotorParameters params;
    params.speed = settings.value("speed", 0.0).toDouble();
    params.accel = settings.value("accel", 0.0).toDouble();
    params.microstep = settings.value("microstep", 0).toInt();
    params.reduction = settings.value("reduction", 0.0).toDouble();
    params.motorSteps = settings.value("motorSteps", 0).toInt();
    params.positiveBacklash = settings.value("positiveBacklash", 0.0).toDouble();
    params.negativeBacklash = settings.value("negativeBacklash", 0.0).toDouble();
    params.inverted = settings.value("inverted", 0).toInt();
    return params;
}
