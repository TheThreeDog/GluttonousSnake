
QT += network


INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/netcontrol.h \
    $$PWD/netdao.h \
    $$PWD/sockettransceiver.h

SOURCES += \
    $$PWD/netcontrol.cpp \
    $$PWD/netdao.cpp \
    $$PWD/sockettransceiver.cpp
