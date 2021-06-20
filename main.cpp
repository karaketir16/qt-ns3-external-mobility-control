#include "tester.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Tester test;

    return a.exec();
}
