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

class Tester : public QObject
{
    Q_OBJECT
public:

    explicit Tester(QObject *parent = nullptr);
    void loop();
    void dataReceived();
private:
    int j =0;
    int base_port = NS3_NODE_PORT_BASE;
    int imax = 750;
    Protocol p;
    QUdpSocket* my_socket;
    int increase = 5;
    int64_t last_val = 0;
    uint64_t last_total =0;
    bool switced = false;
    bool again = false;
    bool connected = false;
    QElapsedTimer timer;
};

#endif // TESTER_H
