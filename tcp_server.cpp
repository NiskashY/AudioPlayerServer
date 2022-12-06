#include "tcp_server.h"

void TcpServer::startServer() {
    bool isConnected = ConnectToDatabase(db);
    if (!isConnected) {
        qDebug() << "NOT CONNECTED TO DB";
    } else if (!this->listen(kIpAddress, kPort)) {
        qDebug() << "NOT LISTENING";
    } else {
        Log log;
        log.Write("Listening to port " + std::to_string(kPort) + " on ip address "
                  + kIpAddress.toString().toStdString());

        qDebug() << "LISTENING!!!";
    }
}

void TcpServer::incomingConnection(qintptr handle) {
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(handle);

    Log log;
    log.Write("Connected" + std::to_string(handle));

    connect(socket, SIGNAL(readyRead()), this, SLOT(sockReady()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(sockDisc()));
}

void TcpServer::sockReady() {
    data = socket->readAll();

    Log log;
    log.Write("Accepted request: " + QString(data).toStdString());
    // execute query. There necessary double (( )) <-> without double Parentheses query(QString(data)) == query(QString data)
    // but with double Parentheses query((QString(data))) -> constructs an object of QString from data -> pass this object into function
    QSqlQuery query((QString(data)));
    QString jsonFormattedResponse = GetJsonFormatQuery(query);  // get query in json form

    if (!jsonFormattedResponse.isEmpty()) {
        socket->write(jsonFormattedResponse.toStdString().c_str());
        log.Write("Response sent: " + jsonFormattedResponse.toStdString());
    }
}

void TcpServer::sockDisc() {
    Log log;
    log.Write("Disconnected");
    socket->deleteLater();
}