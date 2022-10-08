QT       += core gui
QT       += core gui serialport
QT       += charts


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += core5compat

CONFIG += c++11


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Middle/SendReceiveArea.cpp \
    Middle/SerialPortBasicSetting.cpp \
    MyFormatTrans.cpp \
    Setting.cpp \
    Surface/ATModeEdit.cpp \
    Surface/AtMode.cpp \
    Surface/GraphWindow.cpp \
    Surface/MoreBasicSettings.cpp \
    Surface/SaveOptionString.cpp \
    Surface/mainwindow.cpp \
    Underlying/mySerialPort.cpp \
    Underlying/qlibfile.cpp \
    main.cpp \

HEADERS += \
    Middle/SendReceiveArea.h \
    Middle/SerialPortBasicSetting.h \
    MyFormatTrans.h \
    Setting.h \
    Surface/ATModeEdit.h \
    Surface/AtMode.h \
    Surface/GraphWindow.h \
    Surface/MoreBasicSettings.h \
    Surface/SaveOptionString.h \
    Surface/mainwindow.h \
    Underlying/mySerialPort.h \
    Underlying/qlibfile.h \

FORMS += \
    UI/GraphWindow.ui \
    UI/ATModeEdit.ui \
    UI/AtMode.ui \
    UI/MoreBasicSettings.ui \
    UI/SaveOptionString.ui \
    UI/mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons/Icons.qrc \
    Icons_2.qrc

DISTFILES += \
    desktop_icon.ico

RC_ICONS = desktop_icon.ico
