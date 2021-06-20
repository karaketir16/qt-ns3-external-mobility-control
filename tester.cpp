#include "tester.h"


Tester::Tester(QObject *parent) : QObject(parent)
{
    my_socket = new QUdpSocket();

    my_socket->bind(QHostAddress::LocalHost, CONTROLLER_PORT);

    QObject::connect(my_socket, &QUdpSocket::readyRead, this, &Tester::dataReceived);

    p.z = 0;
    p.package_number = 0;
}

void Tester::loop(){

    j += increase;
    j  = ((j % imax) + imax) % imax;

    for (int k =0; k < 3; k++){
        int i = j + k*25;
        p.x = i;
        p.y = i;
        p.package_number++;
        my_socket->writeDatagram(p.buffer, p.encode(PackageType::Position), QHostAddress::LocalHost, base_port + 0 + k * 4);
        my_socket->waitForBytesWritten();

        p.x = i;
        p.y = imax - i;
        p.package_number++;
        my_socket->writeDatagram(p.buffer, p.encode(PackageType::Position), QHostAddress::LocalHost, base_port + 1 + k * 4);
        my_socket->waitForBytesWritten();

        p.x = imax - i;
        p.y = i;
        p.package_number++;
        my_socket->writeDatagram(p.buffer, p.encode(PackageType::Position), QHostAddress::LocalHost, base_port + 2 + k * 4);
        my_socket->waitForBytesWritten();

        p.x = imax - i;
        p.y = imax - i;
        p.package_number++;
        my_socket->writeDatagram(p.buffer, p.encode(PackageType::Position), QHostAddress::LocalHost, base_port + 3 + k * 4);
        my_socket->waitForBytesWritten();
    }
    if(again)
        QTimer::singleShot(100, this, &Tester::loop);
}

void Tester::dataReceived(){
    while(my_socket->bytesAvailable()){
        Protocol p;
        if( p.decode(my_socket->readDatagram(p.buffer,MAX_PKG_LEN)) == PackageType::Information){

            uint64_t now_total = p.totalReceivedData;
            int64_t now_val = (int64_t)now_total - last_total;
            int64_t scaled = now_val * 1000.0 / timer.restart();

            switch ((StateType)p.state) {
            case Waiting:
                again = true;
                j = 0;
                loop();
                my_socket->writeDatagram(p.buffer,
                                         p.encode(PackageType::Start),
                                         QHostAddress::LocalHost, 9090);
                timer.start();
                break;
            case Running:

                if(scaled > 100){
                    connected = true;
                }

                if(scaled < 50 && connected){
                    connected = false;
                    increase *= -1;
                }

                last_val = now_val;
                last_total = now_total;
                break;
            case End:
                again = false;
                qDebug() << "Stop";
                break;
            case __MAX__STATE__:
                break;

            }

            if(p.state == StateType::Waiting){
            } else if(p.state == StateType::End){

            }
        }
    }
}
