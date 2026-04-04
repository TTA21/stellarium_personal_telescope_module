/****************************************************************************
** Meta object code from reading C++ file 'DynamicPluginTemplate.hpp'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/DynamicPluginTemplate.hpp"
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DynamicPluginTemplate.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.0. It"
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
struct qt_meta_tag_ZN21DynamicPluginTemplateE_t {};
} // unnamed namespace

template <> constexpr inline auto DynamicPluginTemplate::qt_create_metaobjectdata<qt_meta_tag_ZN21DynamicPluginTemplateE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DynamicPluginTemplate"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DynamicPluginTemplate, qt_meta_tag_ZN21DynamicPluginTemplateE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DynamicPluginTemplate::staticMetaObject = { {
    QMetaObject::SuperData::link<StelModule::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DynamicPluginTemplateE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DynamicPluginTemplateE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN21DynamicPluginTemplateE_t>.metaTypes,
    nullptr
} };

void DynamicPluginTemplate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DynamicPluginTemplate *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *DynamicPluginTemplate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DynamicPluginTemplate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN21DynamicPluginTemplateE_t>.strings))
        return static_cast<void*>(this);
    return StelModule::qt_metacast(_clname);
}

int DynamicPluginTemplate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = StelModule::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t {};
} // unnamed namespace

template <> constexpr inline auto DynamicPluginTemplatePluginInterface::qt_create_metaobjectdata<qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DynamicPluginTemplatePluginInterface"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DynamicPluginTemplatePluginInterface, qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DynamicPluginTemplatePluginInterface::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>.metaTypes,
    nullptr
} };

void DynamicPluginTemplatePluginInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DynamicPluginTemplatePluginInterface *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *DynamicPluginTemplatePluginInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DynamicPluginTemplatePluginInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN36DynamicPluginTemplatePluginInterfaceE_t>.strings))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "StelPluginInterface"))
        return static_cast< StelPluginInterface*>(this);
    if (!strcmp(_clname, "org.stellarium.StelPluginInterface/2.0"))
        return static_cast< StelPluginInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int DynamicPluginTemplatePluginInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
static constexpr unsigned char qt_pluginMetaDataV2_DynamicPluginTemplatePluginInterface[] = {
    0xbf, 
    // "IID"
    0x02,  0x78,  0x26,  'o',  'r',  'g',  '.',  's', 
    't',  'e',  'l',  'l',  'a',  'r',  'i',  'u', 
    'm',  '.',  'S',  't',  'e',  'l',  'P',  'l', 
    'u',  'g',  'i',  'n',  'I',  'n',  't',  'e', 
    'r',  'f',  'a',  'c',  'e',  '/',  '2',  '.', 
    '0', 
    // "className"
    0x03,  0x78,  0x24,  'D',  'y',  'n',  'a',  'm', 
    'i',  'c',  'P',  'l',  'u',  'g',  'i',  'n', 
    'T',  'e',  'm',  'p',  'l',  'a',  't',  'e', 
    'P',  'l',  'u',  'g',  'i',  'n',  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN_V2(DynamicPluginTemplatePluginInterface, DynamicPluginTemplatePluginInterface, qt_pluginMetaDataV2_DynamicPluginTemplatePluginInterface)
#else
QT_PLUGIN_METADATA_SECTION
Q_CONSTINIT static constexpr unsigned char qt_pluginMetaData_DynamicPluginTemplatePluginInterface[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x26,  'o',  'r',  'g',  '.',  's', 
    't',  'e',  'l',  'l',  'a',  'r',  'i',  'u', 
    'm',  '.',  'S',  't',  'e',  'l',  'P',  'l', 
    'u',  'g',  'i',  'n',  'I',  'n',  't',  'e', 
    'r',  'f',  'a',  'c',  'e',  '/',  '2',  '.', 
    '0', 
    // "className"
    0x03,  0x78,  0x24,  'D',  'y',  'n',  'a',  'm', 
    'i',  'c',  'P',  'l',  'u',  'g',  'i',  'n', 
    'T',  'e',  'm',  'p',  'l',  'a',  't',  'e', 
    'P',  'l',  'u',  'g',  'i',  'n',  'I',  'n', 
    't',  'e',  'r',  'f',  'a',  'c',  'e', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(DynamicPluginTemplatePluginInterface, DynamicPluginTemplatePluginInterface)
#endif  // QT_MOC_EXPORT_PLUGIN_V2

QT_WARNING_POP
