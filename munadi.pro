# Add more folders to ship with the application, here
#folder_01.source = qml/munadi_ng
#folder_01.target = qml
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = ./

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

QT += core gui widgets network multimedia
CONFIG += qt

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += cpp/main.cpp     \
    cpp/munadiengine.cpp    \
    cpp/updater.cpp         \
    cpp/libitl/astro.c      \
    cpp/libitl/hijri.c      \
    cpp/libitl/prayer.c     \
    cpp/libitl/umm_alqura.c

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    rc.qrc

OTHER_FILES +=              \
    qml/main.qml            \
    qml/Tuple.qml           \
    qml/Table.qml           \
    qml/Extras.qml          \
    qml/Prayer.qml          \
    qml/Button.qml          \
    qml/Info.qml            \
    qml/Compass.qml         \
    qml/Settings.qml        \
    qml/SettingTitle.qml    \
    qml/SettingInfo.qml     \
    qml/SettingEntry.qml

HEADERS +=                  \
    cpp/munadiengine.h      \
    cpp/updater.h           \
    cpp/libitl/astro.h      \
    cpp/libitl/hijri.h      \
    cpp/libitl/prayer.h \
    cpp/settingscache.h
