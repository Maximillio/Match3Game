TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    match3model.cpp

RESOURCES +=

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    match3model.h \
    ball.h

DISTFILES += \
    main.qml \
    Delegate.qml
