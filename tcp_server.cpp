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

    QString accepted_request(data);
    Log log;
    log.Write("Accepted request: " + accepted_request.toStdString());

    const QString key_word("PLAY:");
    const QStringRef accepted_req_key(&accepted_request, 0, key_word.size());

    if (key_word == accepted_req_key) {
        const QStringRef accepted_file_name(&accepted_request, key_word.size() + 1, accepted_request.size() - key_word.size() - 1);
        const QString &request_for_file_url = QString("SELECT song_file FROM songs WHERE song_title = '%1'").arg(
                accepted_file_name);
        log.Write("Accepted request: " + request_for_file_url.toStdString());
        QSqlQuery query(request_for_file_url);
        QJsonDocument jsonFormattedResponse = QJsonDocument::fromJson(GetJsonFormatQuery(query).toLatin1());  // get query in json form
        QString file_name = jsonFormattedResponse[0].toObject().value("song_file").toString();
        log.Write("found file name: " + file_name.toStdString());
        QFile file(file_name);
        file.open(QIODevice::ReadOnly);
        QByteArray file_to_write(file.readAll());

        qint64 current_progress = 0;
        while (current_progress < file_to_write.size()) {
            current_progress += socket->write(file_to_write);
            log.WriteInline(current_progress / file_to_write.size() * 100);
        }

    } else {
        QSqlQuery query(accepted_request);
        QString jsonFormattedResponse = GetJsonFormatQuery(query);  // get query in json form

        if (!jsonFormattedResponse.isEmpty()) {
            socket->write(jsonFormattedResponse.toStdString().c_str());
            log.Write("Response sent: " + jsonFormattedResponse.toStdString());
        }
    }
}

void TcpServer::sockDisc() {
    Log log;
    log.Write("Disconnected");
    socket->deleteLater();
}
