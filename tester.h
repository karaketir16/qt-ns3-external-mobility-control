#ifndef TESTER_H
#define TESTER_H


#include <QCoreApplication>
#include <QUdpSocket>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <cmath>
#include <QObject>
#include <QElapsedTimer>
#include "udpsockethelper.h"
#include "protocol.h"

#include <QRandomGenerator>


class Tester : public QObject
{
    Q_OBJECT
public:

    explicit Tester(QObject *parent = nullptr);
    void loop_test_1();
    void dataReceived_test_1();

    void loop_test_2();
    void dataReceived_test_2();
private:
    int j =0;
    int base_port = NS3_NODE_PORT_BASE;
    int imax = 750;
    MobilityProtocol p;
    MobilityProtocol p_arr[20];
    QUdpSocket* my_socket;
    int increase = 5;
    int64_t last_val = 0;
    uint64_t last_total =0;
    bool switced = false;
    bool again = false;
    bool connected = true;
    QElapsedTimer timer;
    QRandomGenerator *randgen;
};

#endif // TESTER_H
