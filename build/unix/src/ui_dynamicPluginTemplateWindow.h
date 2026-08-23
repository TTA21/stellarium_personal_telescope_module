/********************************************************************************
** Form generated from reading UI file 'dynamicPluginTemplateWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DYNAMICPLUGINTEMPLATEWINDOW_H
#define UI_DYNAMICPLUGINTEMPLATEWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "Dialog.hpp"

QT_BEGIN_NAMESPACE

class Ui_dynamicPluginTemplateWindowForm
{
public:
    TitleBar *titleBar;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QPushButton *buttonFRotCCW;
    QPushButton *buttonNudgeLeft;
    QLabel *objectiveSensorDiffLabel;
    QPushButton *buttonNudgeRight;
    QLabel *selectedObjLabel;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *buttonNudgeDown;
    QPushButton *buttonToggleSensor;
    QPushButton *buttonNudgeUp;
    QDoubleSpinBox *nudgeDoubleSpinBox;
    QPushButton *buttonFRotCW;
    QLabel *label_4;
    QLabel *lastAltAzLabel;
    QPushButton *buttonTrackToggle;
    QLabel *sensorAltAzLabel;
    QLabel *label_3;
    QDoubleSpinBox *focuserDoubleSpinBox;
    QLabel *label_15;
    QPushButton *focusPushButton;
    QPushButton *homeFocusPushButton;
    QWidget *tab_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QTextBrowser *serialTerminalTextBrowser;
    QFrame *line;
    QLineEdit *terminalSendLineEdit;
    QPushButton *buttonTerminalSend;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *listSerialPortsButton;
    QComboBox *serialPortComboBox;
    QPushButton *buttonConnectSerial;
    QPushButton *buttonClearTerminal;
    QWidget *tab_3;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QDoubleSpinBox *accelerationDoubleSpinBox;
    QLabel *label_6;
    QDoubleSpinBox *speedDoubleSpinBox;
    QGridLayout *gridLayout_3;
    QLabel *label_7;
    QComboBox *microStepsComboBox;
    QLabel *label_8;
    QComboBox *motorStepsComboBox;
    QGridLayout *gridLayout_4;
    QLabel *label_9;
    QDoubleSpinBox *reductionDoubleSpinBox;
    QLabel *label_10;
    QComboBox *rotationComboBox;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *CWBacklashDoubleSpinBox;
    QLabel *label_11;
    QLabel *label_12;
    QDoubleSpinBox *CCWBacklashDoubleSpinBox;
    QPushButton *sendMotorParamsPushButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_13;
    QComboBox *selectedMotorComboBox;
    QFrame *line_2;
    QPushButton *requestMotorUpdatePushButton;
    QGridLayout *gridLayout_7;
    QComboBox *settingsPresetComboBox;
    QLabel *label_14;
    QPushButton *savePresetPushButton;
    QLineEdit *newPresetNameLineEdit;
    QPushButton *deletePresetPushButton;
    QPushButton *updatePresetPushButton;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QPushButton *pushButtonTest;

    void setupUi(QWidget *dynamicPluginTemplateWindowForm)
    {
        if (dynamicPluginTemplateWindowForm->objectName().isEmpty())
            dynamicPluginTemplateWindowForm->setObjectName("dynamicPluginTemplateWindowForm");
        dynamicPluginTemplateWindowForm->setWindowModality(Qt::WindowModality::ApplicationModal);
        dynamicPluginTemplateWindowForm->resize(793, 439);
        dynamicPluginTemplateWindowForm->setMinimumSize(QSize(793, 439));
        dynamicPluginTemplateWindowForm->setMaximumSize(QSize(793, 439));
        titleBar = new TitleBar(dynamicPluginTemplateWindowForm);
        titleBar->setObjectName("titleBar");
        titleBar->setGeometry(QRect(0, 0, 791, 25));
        tabWidget = new QTabWidget(dynamicPluginTemplateWindowForm);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(10, 30, 771, 401));
        tab = new QWidget();
        tab->setObjectName("tab");
        gridLayoutWidget_2 = new QWidget(tab);
        gridLayoutWidget_2->setObjectName("gridLayoutWidget_2");
        gridLayoutWidget_2->setGeometry(QRect(0, 0, 761, 147));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(gridLayoutWidget_2);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 1, 4, 1, 1);

        buttonFRotCCW = new QPushButton(gridLayoutWidget_2);
        buttonFRotCCW->setObjectName("buttonFRotCCW");

        gridLayout_2->addWidget(buttonFRotCCW, 0, 0, 1, 1);

        buttonNudgeLeft = new QPushButton(gridLayoutWidget_2);
        buttonNudgeLeft->setObjectName("buttonNudgeLeft");

        gridLayout_2->addWidget(buttonNudgeLeft, 1, 0, 1, 1);

        objectiveSensorDiffLabel = new QLabel(gridLayoutWidget_2);
        objectiveSensorDiffLabel->setObjectName("objectiveSensorDiffLabel");

        gridLayout_2->addWidget(objectiveSensorDiffLabel, 3, 5, 1, 1);

        buttonNudgeRight = new QPushButton(gridLayoutWidget_2);
        buttonNudgeRight->setObjectName("buttonNudgeRight");

        gridLayout_2->addWidget(buttonNudgeRight, 1, 2, 1, 1);

        selectedObjLabel = new QLabel(gridLayoutWidget_2);
        selectedObjLabel->setObjectName("selectedObjLabel");

        gridLayout_2->addWidget(selectedObjLabel, 4, 5, 1, 1);

        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName("label_2");

        gridLayout_2->addWidget(label_2, 2, 4, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 5, 4, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 1, 3, 1, 1);

        buttonNudgeDown = new QPushButton(gridLayoutWidget_2);
        buttonNudgeDown->setObjectName("buttonNudgeDown");

        gridLayout_2->addWidget(buttonNudgeDown, 1, 1, 1, 1);

        buttonToggleSensor = new QPushButton(gridLayoutWidget_2);
        buttonToggleSensor->setObjectName("buttonToggleSensor");

        gridLayout_2->addWidget(buttonToggleSensor, 0, 4, 1, 1);

        buttonNudgeUp = new QPushButton(gridLayoutWidget_2);
        buttonNudgeUp->setObjectName("buttonNudgeUp");

        gridLayout_2->addWidget(buttonNudgeUp, 0, 1, 1, 1);

        nudgeDoubleSpinBox = new QDoubleSpinBox(gridLayoutWidget_2);
        nudgeDoubleSpinBox->setObjectName("nudgeDoubleSpinBox");
        nudgeDoubleSpinBox->setSingleStep(0.050000000000000);

        gridLayout_2->addWidget(nudgeDoubleSpinBox, 2, 0, 1, 1);

        buttonFRotCW = new QPushButton(gridLayoutWidget_2);
        buttonFRotCW->setObjectName("buttonFRotCW");

        gridLayout_2->addWidget(buttonFRotCW, 0, 2, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 4, 4, 1, 1);

        lastAltAzLabel = new QLabel(gridLayoutWidget_2);
        lastAltAzLabel->setObjectName("lastAltAzLabel");

        gridLayout_2->addWidget(lastAltAzLabel, 1, 5, 1, 1);

        buttonTrackToggle = new QPushButton(gridLayoutWidget_2);
        buttonTrackToggle->setObjectName("buttonTrackToggle");

        gridLayout_2->addWidget(buttonTrackToggle, 0, 5, 1, 1);

        sensorAltAzLabel = new QLabel(gridLayoutWidget_2);
        sensorAltAzLabel->setObjectName("sensorAltAzLabel");

        gridLayout_2->addWidget(sensorAltAzLabel, 2, 5, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_2);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 3, 4, 1, 1);

        focuserDoubleSpinBox = new QDoubleSpinBox(gridLayoutWidget_2);
        focuserDoubleSpinBox->setObjectName("focuserDoubleSpinBox");
        focuserDoubleSpinBox->setMaximum(82.000000000000000);
        focuserDoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_2->addWidget(focuserDoubleSpinBox, 4, 1, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_2);
        label_15->setObjectName("label_15");

        gridLayout_2->addWidget(label_15, 4, 0, 1, 1);

        focusPushButton = new QPushButton(gridLayoutWidget_2);
        focusPushButton->setObjectName("focusPushButton");

        gridLayout_2->addWidget(focusPushButton, 4, 2, 1, 1);

        homeFocusPushButton = new QPushButton(gridLayoutWidget_2);
        homeFocusPushButton->setObjectName("homeFocusPushButton");

        gridLayout_2->addWidget(homeFocusPushButton, 3, 2, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        verticalLayoutWidget = new QWidget(tab_2);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 34, 761, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        serialTerminalTextBrowser = new QTextBrowser(verticalLayoutWidget);
        serialTerminalTextBrowser->setObjectName("serialTerminalTextBrowser");

        verticalLayout->addWidget(serialTerminalTextBrowser);

        line = new QFrame(verticalLayoutWidget);
        line->setObjectName("line");
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);

        verticalLayout->addWidget(line);

        terminalSendLineEdit = new QLineEdit(verticalLayoutWidget);
        terminalSendLineEdit->setObjectName("terminalSendLineEdit");

        verticalLayout->addWidget(terminalSendLineEdit);

        buttonTerminalSend = new QPushButton(verticalLayoutWidget);
        buttonTerminalSend->setObjectName("buttonTerminalSend");

        verticalLayout->addWidget(buttonTerminalSend);

        horizontalLayoutWidget = new QWidget(tab_2);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 761, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        listSerialPortsButton = new QPushButton(horizontalLayoutWidget);
        listSerialPortsButton->setObjectName("listSerialPortsButton");

        horizontalLayout->addWidget(listSerialPortsButton);

        serialPortComboBox = new QComboBox(horizontalLayoutWidget);
        serialPortComboBox->setObjectName("serialPortComboBox");
        serialPortComboBox->setMinimumSize(QSize(140, 0));

        horizontalLayout->addWidget(serialPortComboBox);

        buttonConnectSerial = new QPushButton(horizontalLayoutWidget);
        buttonConnectSerial->setObjectName("buttonConnectSerial");

        horizontalLayout->addWidget(buttonConnectSerial);

        buttonClearTerminal = new QPushButton(horizontalLayoutWidget);
        buttonClearTerminal->setObjectName("buttonClearTerminal");

        horizontalLayout->addWidget(buttonClearTerminal);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        verticalLayoutWidget_2 = new QWidget(tab_3);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(10, 10, 751, 201));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName("gridLayout");
        label_5 = new QLabel(verticalLayoutWidget_2);
        label_5->setObjectName("label_5");

        gridLayout->addWidget(label_5, 0, 0, 1, 1);

        accelerationDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        accelerationDoubleSpinBox->setObjectName("accelerationDoubleSpinBox");
        accelerationDoubleSpinBox->setMaximum(50000.000000000000000);

        gridLayout->addWidget(accelerationDoubleSpinBox, 0, 1, 1, 1);

        label_6 = new QLabel(verticalLayoutWidget_2);
        label_6->setObjectName("label_6");

        gridLayout->addWidget(label_6, 1, 0, 1, 1);

        speedDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        speedDoubleSpinBox->setObjectName("speedDoubleSpinBox");
        speedDoubleSpinBox->setMaximum(50000.000000000000000);

        gridLayout->addWidget(speedDoubleSpinBox, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName("gridLayout_3");
        label_7 = new QLabel(verticalLayoutWidget_2);
        label_7->setObjectName("label_7");

        gridLayout_3->addWidget(label_7, 0, 0, 1, 1);

        microStepsComboBox = new QComboBox(verticalLayoutWidget_2);
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->addItem(QString());
        microStepsComboBox->setObjectName("microStepsComboBox");

        gridLayout_3->addWidget(microStepsComboBox, 0, 1, 1, 1);

        label_8 = new QLabel(verticalLayoutWidget_2);
        label_8->setObjectName("label_8");

        gridLayout_3->addWidget(label_8, 1, 0, 1, 1);

        motorStepsComboBox = new QComboBox(verticalLayoutWidget_2);
        motorStepsComboBox->addItem(QString());
        motorStepsComboBox->addItem(QString());
        motorStepsComboBox->setObjectName("motorStepsComboBox");

        gridLayout_3->addWidget(motorStepsComboBox, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_3);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName("gridLayout_4");
        label_9 = new QLabel(verticalLayoutWidget_2);
        label_9->setObjectName("label_9");

        gridLayout_4->addWidget(label_9, 0, 0, 1, 1);

        reductionDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        reductionDoubleSpinBox->setObjectName("reductionDoubleSpinBox");
        reductionDoubleSpinBox->setMinimum(1.000000000000000);
        reductionDoubleSpinBox->setMaximum(1000.000000000000000);
        reductionDoubleSpinBox->setSingleStep(0.010000000000000);

        gridLayout_4->addWidget(reductionDoubleSpinBox, 0, 1, 1, 1);

        label_10 = new QLabel(verticalLayoutWidget_2);
        label_10->setObjectName("label_10");

        gridLayout_4->addWidget(label_10, 1, 0, 1, 1);

        rotationComboBox = new QComboBox(verticalLayoutWidget_2);
        rotationComboBox->addItem(QString());
        rotationComboBox->addItem(QString());
        rotationComboBox->setObjectName("rotationComboBox");

        gridLayout_4->addWidget(rotationComboBox, 1, 1, 1, 1);


        horizontalLayout_2->addLayout(gridLayout_4);


        verticalLayout_2->addLayout(horizontalLayout_2);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setSpacing(6);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName("gridLayout_5");
        CWBacklashDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        CWBacklashDoubleSpinBox->setObjectName("CWBacklashDoubleSpinBox");
        CWBacklashDoubleSpinBox->setDecimals(5);
        CWBacklashDoubleSpinBox->setSingleStep(0.000100000000000);

        gridLayout_5->addWidget(CWBacklashDoubleSpinBox, 0, 1, 1, 1);

        label_11 = new QLabel(verticalLayoutWidget_2);
        label_11->setObjectName("label_11");

        gridLayout_5->addWidget(label_11, 0, 0, 1, 1);

        label_12 = new QLabel(verticalLayoutWidget_2);
        label_12->setObjectName("label_12");

        gridLayout_5->addWidget(label_12, 1, 0, 1, 1);

        CCWBacklashDoubleSpinBox = new QDoubleSpinBox(verticalLayoutWidget_2);
        CCWBacklashDoubleSpinBox->setObjectName("CCWBacklashDoubleSpinBox");
        CCWBacklashDoubleSpinBox->setDecimals(5);
        CCWBacklashDoubleSpinBox->setSingleStep(0.000100000000000);

        gridLayout_5->addWidget(CCWBacklashDoubleSpinBox, 1, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        sendMotorParamsPushButton = new QPushButton(verticalLayoutWidget_2);
        sendMotorParamsPushButton->setObjectName("sendMotorParamsPushButton");

        gridLayout_6->addWidget(sendMotorParamsPushButton, 0, 2, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label_13 = new QLabel(verticalLayoutWidget_2);
        label_13->setObjectName("label_13");

        horizontalLayout_3->addWidget(label_13);

        selectedMotorComboBox = new QComboBox(verticalLayoutWidget_2);
        selectedMotorComboBox->addItem(QString());
        selectedMotorComboBox->addItem(QString());
        selectedMotorComboBox->addItem(QString());
        selectedMotorComboBox->addItem(QString());
        selectedMotorComboBox->setObjectName("selectedMotorComboBox");

        horizontalLayout_3->addWidget(selectedMotorComboBox);


        gridLayout_6->addLayout(horizontalLayout_3, 0, 1, 1, 1);

        line_2 = new QFrame(verticalLayoutWidget_2);
        line_2->setObjectName("line_2");
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_6->addWidget(line_2, 1, 1, 1, 1);

        requestMotorUpdatePushButton = new QPushButton(verticalLayoutWidget_2);
        requestMotorUpdatePushButton->setObjectName("requestMotorUpdatePushButton");

        gridLayout_6->addWidget(requestMotorUpdatePushButton, 0, 3, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName("gridLayout_7");
        settingsPresetComboBox = new QComboBox(verticalLayoutWidget_2);
        settingsPresetComboBox->setObjectName("settingsPresetComboBox");

        gridLayout_7->addWidget(settingsPresetComboBox, 0, 1, 1, 1);

        label_14 = new QLabel(verticalLayoutWidget_2);
        label_14->setObjectName("label_14");

        gridLayout_7->addWidget(label_14, 0, 0, 1, 1);

        savePresetPushButton = new QPushButton(verticalLayoutWidget_2);
        savePresetPushButton->setObjectName("savePresetPushButton");

        gridLayout_7->addWidget(savePresetPushButton, 1, 0, 1, 1);

        newPresetNameLineEdit = new QLineEdit(verticalLayoutWidget_2);
        newPresetNameLineEdit->setObjectName("newPresetNameLineEdit");

        gridLayout_7->addWidget(newPresetNameLineEdit, 1, 1, 1, 1);

        deletePresetPushButton = new QPushButton(verticalLayoutWidget_2);
        deletePresetPushButton->setObjectName("deletePresetPushButton");

        gridLayout_7->addWidget(deletePresetPushButton, 1, 2, 1, 1);

        updatePresetPushButton = new QPushButton(verticalLayoutWidget_2);
        updatePresetPushButton->setObjectName("updatePresetPushButton");

        gridLayout_7->addWidget(updatePresetPushButton, 0, 2, 1, 1);


        gridLayout_6->addLayout(gridLayout_7, 2, 0, 1, 2);

        line_3 = new QFrame(verticalLayoutWidget_2);
        line_3->setObjectName("line_3");
        line_3->setFrameShape(QFrame::Shape::HLine);
        line_3->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_6->addWidget(line_3, 1, 0, 1, 1);

        line_4 = new QFrame(verticalLayoutWidget_2);
        line_4->setObjectName("line_4");
        line_4->setFrameShape(QFrame::Shape::HLine);
        line_4->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_6->addWidget(line_4, 1, 2, 1, 1);

        line_5 = new QFrame(verticalLayoutWidget_2);
        line_5->setObjectName("line_5");
        line_5->setFrameShape(QFrame::Shape::HLine);
        line_5->setFrameShadow(QFrame::Shadow::Sunken);

        gridLayout_6->addWidget(line_5, 1, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout_6);

        pushButtonTest = new QPushButton(tab_3);
        pushButtonTest->setObjectName("pushButtonTest");
        pushButtonTest->setGeometry(QRect(390, 260, 171, 23));
        tabWidget->addTab(tab_3, QString());

        retranslateUi(dynamicPluginTemplateWindowForm);

        tabWidget->setCurrentIndex(0);
        microStepsComboBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(dynamicPluginTemplateWindowForm);
    } // setupUi

    void retranslateUi(QWidget *dynamicPluginTemplateWindowForm)
    {
        dynamicPluginTemplateWindowForm->setWindowTitle(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Dialog", nullptr));
        titleBar->setTitle(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Serial Telescope Control", nullptr));
        label->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Objective:", nullptr));
        buttonFRotCCW->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "FRot CCW", nullptr));
        buttonNudgeLeft->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Left", nullptr));
        objectiveSensorDiffLabel->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Alt: 0  Az: 0", nullptr));
        buttonNudgeRight->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Right", nullptr));
        selectedObjLabel->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Alt: 0 Az: 0", nullptr));
        label_2->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Sensors:", nullptr));
        buttonNudgeDown->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Down", nullptr));
        buttonToggleSensor->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "ToggleSensor", nullptr));
        buttonNudgeUp->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Up", nullptr));
        buttonFRotCW->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "FRot CW", nullptr));
        label_4->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Selected:", nullptr));
        lastAltAzLabel->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Alt: 0  Az: 0", nullptr));
        buttonTrackToggle->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Track", nullptr));
        sensorAltAzLabel->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Alt: 0  Az: 0", nullptr));
        label_3->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Diff:", nullptr));
        label_15->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Focuser: ", nullptr));
        focusPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Send Focus", nullptr));
        homeFocusPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Home Focus", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Nudge Controls", nullptr));
        serialTerminalTextBrowser->setHtml(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Ubuntu'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Serial Terminal Waiting Connection</p></body></html>", nullptr));
        buttonTerminalSend->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Send", nullptr));
        listSerialPortsButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "List Serial Ports", nullptr));
        buttonConnectSerial->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Connect", nullptr));
        buttonClearTerminal->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Clear Terminal", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Serial Ports", nullptr));
        label_5->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Acceleration:", nullptr));
        label_6->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Speed:", nullptr));
        label_7->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Micro Steps:", nullptr));
        microStepsComboBox->setItemText(0, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "2", nullptr));
        microStepsComboBox->setItemText(1, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "4", nullptr));
        microStepsComboBox->setItemText(2, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "8", nullptr));
        microStepsComboBox->setItemText(3, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "16", nullptr));
        microStepsComboBox->setItemText(4, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "32", nullptr));
        microStepsComboBox->setItemText(5, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "64", nullptr));
        microStepsComboBox->setItemText(6, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "128", nullptr));
        microStepsComboBox->setItemText(7, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "256", nullptr));

        label_8->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Motor Steps:", nullptr));
        motorStepsComboBox->setItemText(0, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "200", nullptr));
        motorStepsComboBox->setItemText(1, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "400", nullptr));

        label_9->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Reduction:", nullptr));
        label_10->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Rotation:", nullptr));
        rotationComboBox->setItemText(0, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "CW", nullptr));
        rotationComboBox->setItemText(1, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "CCW", nullptr));

        label_11->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "CW Backlash", nullptr));
        label_12->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "CCW Backlash", nullptr));
        sendMotorParamsPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Send", nullptr));
        label_13->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Selected Motor:", nullptr));
        selectedMotorComboBox->setItemText(0, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Azimuth", nullptr));
        selectedMotorComboBox->setItemText(1, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Altitude", nullptr));
        selectedMotorComboBox->setItemText(2, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Field Rotation", nullptr));
        selectedMotorComboBox->setItemText(3, QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Focuser", nullptr));

        requestMotorUpdatePushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Request Update", nullptr));
        label_14->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Settings Preset:", nullptr));
        savePresetPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Save current preset", nullptr));
        newPresetNameLineEdit->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "New Preset Name", nullptr));
        deletePresetPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Delete Preset", nullptr));
        updatePresetPushButton->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Update Preset", nullptr));
        pushButtonTest->setText(QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Test", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("dynamicPluginTemplateWindowForm", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class dynamicPluginTemplateWindowForm: public Ui_dynamicPluginTemplateWindowForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DYNAMICPLUGINTEMPLATEWINDOW_H
