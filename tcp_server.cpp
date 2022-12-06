#include "tcp_server.h"

void TcpServer::startServer() {
    bool isConnected = ConnectToDatabase(db);
    if (!isConnected) {
        qDebug() << "NOT CONNECTED TO DB";
    } else if (!this->listen(kIpAddress, kPort)) {
        qDebug() << "NOT LISTENING";
    } else {
        qDebug() << "LISTENING!!!!";
    }
}

void TcpServer::incomingConnection(qintptr handle) {
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);

    qDebug() << "connected " << handle;

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
}

void TcpServer::sockReady() {
    data = socket->readAll();

    // execute query. There necessary double (( )) <-> without double Parentheses query(QString(data)) == query(QString data)
    // but with double Parentheses query((QString(data))) -> constructs an object of QString from data -> pass this object into function
    QSqlQuery query((QString(data)));
    QString jsonFormattedResponse = GetJsonFormatQuery(query);  // get query in json form

    if (!jsonFormattedResponse.isEmpty()) {
        socket->write(jsonFormattedResponse.toStdString().c_str());
    }
}

void TcpServer::sockDisc() {
    qDebug() << "Disconnected";
    socket->deleteLater();
}