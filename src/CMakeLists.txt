# Copyright (C) 2007 Fabien Chereau
# Copyright (C) 2015 Kirill Snezhko
# Copyright (C) 2018 Alexander Wolf
#
# Based on the HelloStelModule

INCLUDE_DIRECTORIES(.)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

SET(DynamicPluginTemplate_SRCS
     # Source code
     DynamicPluginTemplate.hpp
     DynamicPluginTemplate.cpp
     gui/DynamicPluginTemplateWindow.cpp
     gui/DynamicPluginTemplateWindow.hpp
     gui/DynamicPluginSerialHelper.cpp
     gui/DynamicPluginNudgeControls.cpp
     gui/DynamicPluginMotorSettings.cpp

)

SET(DynamicPluginTemplate_UIS
     # Forms
     gui/dynamicPluginTemplateWindow.ui
)

SET(DynamicPluginTemplate_RES_SXX
     # Resources
     # ../resources/DynamicPluginTemplate.qrc
)

# Wrapping UI
QT6_WRAP_UI(DynamicPluginTemplate_UIS_H ${DynamicPluginTemplate_UIS})

# compiles resources files
# QT5_ADD_RESOURCES(DynamicPluginTemplate_RES_CXX ${DynamicPluginTemplate_RES})

# Library properties
ADD_LIBRARY(DynamicPluginTemplate-dynamic MODULE ${DynamicPluginTemplate_SRCS} ${DynamicPluginTemplate_RES_SXX} ${DynamicPluginTemplate_UIS_H}
     gui/DynamicPluginSerialHelper.cpp
     gui/DynamicPluginNudgeControls.cpp
     gui/DynamicPluginMotorSettings.cpp
)

TARGET_LINK_LIBRARIES(
     DynamicPluginTemplate-dynamic
     Qt6::Core Qt6::Gui
     Qt6::Widgets
     Qt6::OpenGL
     Qt6::SerialPort
     ${LIBVLC_LIBRARIES}
)
SET_TARGET_PROPERTIES(DynamicPluginTemplate-dynamic PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
SET_TARGET_PROPERTIES(DynamicPluginTemplate-dynamic PROPERTIES COMPILE_FLAGS "-DQT_DYNAMICPLUGIN")
SET_TARGET_PROPERTIES(DynamicPluginTemplate-dynamic PROPERTIES AUTOMOC TRUE)
SET_TARGET_PROPERTIES(DynamicPluginTemplate-dynamic PROPERTIES FOLDER "plugins/${PROJECT_NAME}")
