QT -= gui
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        App.cpp \
        Arguments.cpp \
        RPC.cpp \
        Session.cpp \
        main.cpp

HEADERS += \
    App.h \
    Arguments.h \
    RPC.h \
    Session.h

DISTFILES += \
    README.md

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

