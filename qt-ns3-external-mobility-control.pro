QT -= gui
QT += network

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

NS3PATH = ../ns-allinone-3.33/ns-3.33

SOURCES += \
        $${NS3PATH}/scratch/external-mobility-test/protocol.cc \
        $${NS3PATH}/scratch/external-mobility-test/udpsockethelper.cc \
        main.cpp \
        tester.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    $${NS3PATH}/scratch/my_ns3_proj/protocol.h \
    $${NS3PATH}/ns-3.33/scratch/my_ns3_proj/udpsockethelper.h \
    tester.h

INCLUDEPATH += $${NS3PATH}/scratch/external-mobility-test/

STATECHARTS += \
    test.scxml
