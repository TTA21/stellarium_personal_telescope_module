/****************************************************************************
** Meta object code from reading C++ file 'DynamicPluginTemplateWindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/gui/DynamicPluginTemplateWindow.hpp"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DynamicPluginTemplateWindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto DynamicPluginTemplateWindow::qt_create_metaobjectdata<qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DynamicPluginTemplateWindow",
        "retranslate",
        "",
        "on_buttonNudgeLeft_clicked",
        "on_buttonNudgeUp_clicked",
        "on_buttonNudgeDown_clicked",
        "on_buttonNudgeRight_clicked",
        "on_nudgeSlider_sliderReleased",
        "on_nudgeSlider_valueChanged",
        "value",
        "on_buttonFRotCCW_clicked",
        "on_buttonFRotCW_clicked",
        "on_listSerialPortsButton_clicked",
        "on_buttonConnectSerial_clicked",
        "on_buttonTerminalSend_clicked",
        "on_buttonClearTerminal_clicked",
        "handleSerialRead",
        "handleSerialError",
        "QSerialPort::SerialPortError",
        "error",
        "on_pushButtonTest_clicked",
        "on_buttonTrackToggle_clicked",
        "sendCoordinatesToMount",
        "on_buttonToggleSensor_clicked",
        "on_nudgeDoubleSpinBox_valueChanged",
        "arg1",
        "on_sendMotorParamsPushButton_clicked",
        "on_requestMotorUpdatePushButton_clicked",
        "updateSettingsPanel",
        "on_selectedMotorComboBox_currentIndexChanged",
        "on_updatePresetPushButton_clicked",
        "on_savePresetPushButton_clicked",
        "on_settingsPresetComboBox_currentIndexChanged",
        "on_deletePresetPushButton_clicked",
        "on_focusPushButton_clicked",
        "on_homeFocusPushButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'retranslate'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_buttonNudgeLeft_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonNudgeUp_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonNudgeDown_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonNudgeRight_clicked'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_nudgeSlider_sliderReleased'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_nudgeSlider_valueChanged'
        QtMocHelpers::SlotData<void(int)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 9 },
        }}),
        // Slot 'on_buttonFRotCCW_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonFRotCW_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_listSerialPortsButton_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonConnectSerial_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonTerminalSend_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonClearTerminal_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleSerialRead'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleSerialError'
        QtMocHelpers::SlotData<void(QSerialPort::SerialPortError)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'on_pushButtonTest_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonTrackToggle_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'sendCoordinatesToMount'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_buttonToggleSensor_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_nudgeDoubleSpinBox_valueChanged'
        QtMocHelpers::SlotData<void(double)>(24, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 25 },
        }}),
        // Slot 'on_sendMotorParamsPushButton_clicked'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_requestMotorUpdatePushButton_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateSettingsPanel'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_selectedMotorComboBox_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(29, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'on_updatePresetPushButton_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savePresetPushButton_clicked'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_settingsPresetComboBox_currentIndexChanged'
        QtMocHelpers::SlotData<void(int)>(32, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 2 },
        }}),
        // Slot 'on_deletePresetPushButton_clicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_focusPushButton_clicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_homeFocusPushButton_clicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DynamicPluginTemplateWindow, qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DynamicPluginTemplateWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<StelDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>.metaTypes,
    nullptr
} };

void DynamicPluginTemplateWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DynamicPluginTemplateWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->retranslate(); break;
        case 1: _t->on_buttonNudgeLeft_clicked(); break;
        case 2: _t->on_buttonNudgeUp_clicked(); break;
        case 3: _t->on_buttonNudgeDown_clicked(); break;
        case 4: _t->on_buttonNudgeRight_clicked(); break;
        case 5: _t->on_nudgeSlider_sliderReleased(); break;
        case 6: _t->on_nudgeSlider_valueChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->on_buttonFRotCCW_clicked(); break;
        case 8: _t->on_buttonFRotCW_clicked(); break;
        case 9: _t->on_listSerialPortsButton_clicked(); break;
        case 10: _t->on_buttonConnectSerial_clicked(); break;
        case 11: _t->on_buttonTerminalSend_clicked(); break;
        case 12: _t->on_buttonClearTerminal_clicked(); break;
        case 13: _t->handleSerialRead(); break;
        case 14: _t->handleSerialError((*reinterpret_cast<std::add_pointer_t<QSerialPort::SerialPortError>>(_a[1]))); break;
        case 15: _t->on_pushButtonTest_clicked(); break;
        case 16: _t->on_buttonTrackToggle_clicked(); break;
        case 17: _t->sendCoordinatesToMount(); break;
        case 18: _t->on_buttonToggleSensor_clicked(); break;
        case 19: _t->on_nudgeDoubleSpinBox_valueChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 20: _t->on_sendMotorParamsPushButton_clicked(); break;
        case 21: _t->on_requestMotorUpdatePushButton_clicked(); break;
        case 22: _t->updateSettingsPanel(); break;
        case 23: _t->on_selectedMotorComboBox_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 24: _t->on_updatePresetPushButton_clicked(); break;
        case 25: _t->on_savePresetPushButton_clicked(); break;
        case 26: _t->on_settingsPresetComboBox_currentIndexChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 27: _t->on_deletePresetPushButton_clicked(); break;
        case 28: _t->on_focusPushButton_clicked(); break;
        case 29: _t->on_homeFocusPushButton_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *DynamicPluginTemplateWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DynamicPluginTemplateWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN27DynamicPluginTemplateWindowE_t>.strings))
        return static_cast<void*>(this);
    return StelDialog::qt_metacast(_clname);
}

int DynamicPluginTemplateWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = StelDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 30)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 30;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 30)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 30;
    }
    return _id;
}
QT_WARNING_POP
