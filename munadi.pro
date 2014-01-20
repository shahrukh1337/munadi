# Add more folders to ship with the application, here
#folder_01.source = ../audio
#folder_01.target = audio
#DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = ./

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

CONFIG += qt
DEFINES += ARABIC

win32 {
QT += core gui multimedia widgets network
}

android {
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
QT += core gui multimedia sensors
}

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += cpp/main.cpp     \
    cpp/munadiengine.cpp    \
    cpp/libitl/astro.c      \
    cpp/libitl/hijri.c      \
    cpp/libitl/prayer.c     \
    cpp/libitl/umm_alqura.c

!android:SOURCES += cpp/updater.cpp

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
    android/AndroidManifest.xml

lupdate_only{
SOURCES = qml/*.qml
}

HEADERS +=                  \
    cpp/munadiengine.h      \
    cpp/libitl/astro.h      \
    cpp/libitl/hijri.h      \
    cpp/libitl/prayer.h \
    cpp/settingscache.h

!android:HEADERS += cpp/updater.h

TRANSLATIONS += data/ar.ts
