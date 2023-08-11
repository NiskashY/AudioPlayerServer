#pragma once
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "db_manipulation.h"


class TcpServer : public QTcpServer {
    Q_OBJECT
private:
    QTcpSocket* socket = nullptr;
    QByteArray data;
    const QHostAddress kIpAddress = QHostAddress(QHostAddress::LocalHost);
    const quint16 kPort = 9999; // TODO:
    QString working_dir;
    QSqlDatabase db;

public slots:
    void incomingConnection(qintptr handle) override;

    void startServer();
    void sockReady();
    void sockDisc();
};
