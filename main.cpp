#include "tcp_server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    TcpServer server;
    server.startServer();

    return QCoreApplication::exec();
}