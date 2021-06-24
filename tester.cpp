#include "tester.h"



Tester::Tester(QObject *parent) : QObject(parent)
{
    my_socket = new QUdpSocket();

    my_socket->bind(QHostAddress::LocalHost, CONTROLLER_PORT);

    randgen = new QRandomGenerator(202020);

    bool test1 = false;

    if(test1){   //test1
        QObject::connect(my_socket, &QUdpSocket::readyRead, this, &Tester::dataReceived_test_1);
        p.z = 0;
        p.package_number = 0;
        connected = false;
    } else {   //test2
        QObject::connect(my_socket, &QUdpSocket::readyRead, this, &Tester::dataReceived_test_2);

        imax = 600;

        for(int i =0;i<20;i++){
            p_arr[i].z = 0;
            p_arr[i].package_number = 0;
            p_arr[i].x = randgen->generate() % imax;
            p_arr[i].y = randgen->generate() % imax;
        }
        p_arr[0].x = imax / 2;
        p_arr[0].y = imax / 2;

    }
}

void Tester::loop_test_1(){

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
        QTimer::singleShot(100, this, &Tester::loop_test_1);
}

void Tester::dataReceived_test_1(){
    while(my_socket->bytesAvailable()){
        MobilityProtocol p;
        if( p.decode(my_socket->readDatagram(p.buffer,MAX_PKG_LEN)) == PackageType::Information){

            uint64_t now_total = p.totalReceivedData;
            int64_t now_val = (int64_t)now_total - last_total;
            int64_t scaled = now_val * 1000.0 / timer.restart();

            switch ((StateType)p.state) {
            case Waiting:
                again = true;
                j = 0;
                loop_test_1();
                my_socket->writeDatagram(p.buffer,
                                         p.encode(PackageType::Start),
                                         QHostAddress::LocalHost, 9090);
                timer.start();
                break;
            case Running:

                if(scaled > 150){
                    connected = true;
                }

                if(scaled < 100 && connected){
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



void Tester::loop_test_2(){

    for (int k =0; k < 12; k++){
        if(k == 0){
            p.package_number++;
            my_socket->writeDatagram(p_arr[k].buffer, p_arr[k].encode(PackageType::Position), QHostAddress::LocalHost, base_port + k);
            my_socket->waitForBytesWritten();
            continue;
        }



        if(connected){
            p_arr[k].x += increase * (randgen->generateDouble() - 0.5) * 2;
            p_arr[k].y += increase * (randgen->generateDouble() - 0.5) * 2;
        } else {
            auto nearter = [&](double &a){
                if(a > imax/2){
                    a -= increase;
                } else {
                    a += increase;
                }
            };
            nearter(p_arr[k].x);
            nearter(p_arr[k].y);
        }

        p_arr[k].x =  qMin((double)imax, p_arr[k].x);
        p_arr[k].x =  qMax(0.0, p_arr[k].x);
        p_arr[k].y =  qMin((double)imax, p_arr[k].y);
        p_arr[k].y =  qMax(0.0, p_arr[k].y);

        p.package_number++;
        my_socket->writeDatagram(p_arr[k].buffer, p_arr[k].encode(PackageType::Position), QHostAddress::LocalHost, base_port + k);
        my_socket->waitForBytesWritten();
    }
    if(again)
        QTimer::singleShot(100, this, &Tester::loop_test_2);
}

void Tester::dataReceived_test_2(){
    while(my_socket->bytesAvailable()){
        MobilityProtocol p;
        if( p.decode(my_socket->readDatagram(p.buffer,MAX_PKG_LEN)) == PackageType::Information){

            uint64_t now_total = p.totalReceivedData;
            int64_t now_val = (int64_t)now_total - last_total;
            int64_t scaled = now_val * 1000.0 / timer.restart();

            switch ((StateType)p.state) {
            case Waiting:
                again = true;
                j = 0;
                loop_test_2();
                my_socket->writeDatagram(p.buffer,
                                         p.encode(PackageType::Start),
                                         QHostAddress::LocalHost, 9090);
                timer.start();
                break;
            case Running:

                qDebug() << "received info: " << scaled;

                if(scaled > 200){
                    connected = true;
                }

                if(scaled < 150 && connected){
                    connected = false;
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
