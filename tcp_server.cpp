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

    working_dir = QCoreApplication::applicationDirPath() + "/server_tracks/";
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
    data.clear();
    while (socket->bytesAvailable()) {
        data.append(socket->readAll());
        socket->waitForReadyRead(100);
    }

    QString accepted_request(data);
    Log log;

    const QString key_word_play("PLAY:");
    const QString key_word_upload("UPLOAD:");
    const QStringList list = accepted_request.split(' ');

    for (auto& req_split : list) {
        log.WriteInline(req_split.toStdString());
    }
    const QString& accepted_req_key = list[0];

    if (accepted_req_key == key_word_play) {
        const QString& accepted_file_name = list[1];
        const QString &request_for_file_url = QString("SELECT song_file FROM songs WHERE song_title = '%1'").arg(
                accepted_file_name);
        log.Write("Accepted request: " + request_for_file_url.toStdString());
        QSqlQuery query(request_for_file_url);
        QJsonDocument jsonFormattedResponse = QJsonDocument::fromJson(
                GetJsonFormatQuery(query).toLatin1());  // get query in json form
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

    } else if (accepted_req_key == key_word_upload) {
        const int account_name_pos = 1, file_name_pos = 2, file_data_pos = 3;
        const QString &accepted_account_name = list.at(account_name_pos);
        const QString &file_name = list.at(file_name_pos);
        const QString &file_path = working_dir + file_name;

        QByteArray file_data;
        int pos = accepted_req_key.size() + accepted_account_name.size() + file_name.size() + 3;
        for (int i = pos; i < data.size(); ++i) {
            file_data.append(data[i]);
        }

        QFile source_file(file_path);

        if (!source_file.exists()) {
            if (source_file.open(QIODevice::WriteOnly)) {
                source_file.write(file_data);
            }
        }

        const QString &sql_query_get_user_id = QString("SELECT id FROM users WHERE user_nickname='%1'").arg(
                accepted_account_name);
        QSqlQuery id_query(sql_query_get_user_id);
        id_query.next();
        QString user_id = id_query.value(0).toString();
        log.Write("SQL ID QUERY: " + sql_query_get_user_id.toStdString() + "Getted id: " + user_id.toStdString());

        // DELETE FROM TABLE IF EXISTS
        const QString &sql_query_delete_song = QString(
                "DELETE FROM songs WHERE id_user=%1 and song_title='%2';"
        ).arg(user_id, file_name);
        QSqlQuery delete_query(sql_query_delete_song);

        // INSERT INTO TABLE
        const QString &sql_query_insert_song = QString(
                "INSERT INTO songs (id_user, song_title, song_file) VALUES (%1, '%2', '%3');"
        ).arg(user_id, file_name, file_path);

        log.Write("SQL INSERT QUERY: " + sql_query_insert_song.toStdString());
        QSqlQuery insert_query(sql_query_insert_song);

    } else {
        log.Write("Accepted request: " + accepted_request.toStdString());
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
