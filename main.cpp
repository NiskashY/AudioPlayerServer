#include <iostream>
#include <fstream>


#include <QCoreApplication>
#include <QFile>
#include <QtSql/QtSql>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "db_manipulation.h"

#include "logging.h"


bool StartServer();
void Success();
bool ConnectToDatabase(QSqlDatabase&);


class Server {
public:
    explicit Server(const QSqlDatabase& database) : db(database) {

    }

    bool Start() {
        const bool &kFailure = false;
        const bool &kSuccess = true;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Failed to create socket. errno" << errno << std::endl;
            return kFailure;
        }

        sockaddr_in sockaddr{};
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = INADDR_ANY;
        sockaddr.sin_port = htons(9999);

        bind_status = bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

        if (bind_status < 0) {
            std::cerr << "BIND: " << errno << std::endl;
            return kFailure;
        }

        listen_status = listen(sockfd, 10);
        if (listen_status == -1) {
            std::cerr << "LISTEN: " << errno << std::endl;
            return kFailure;
        }

        std::size_t addrlen = sizeof(sockaddr);
        connection = accept(sockfd, (struct sockaddr *) &sockaddr, (socklen_t *) &addrlen);

        if (connection == -1) {
            std::cerr << "ACCEPT: " << errno << std::endl;
            return kFailure;
        }


        while (true) {
            char buffer[100];
            auto bytesRead = read(connection, buffer, 100);

            std::string str_buffer(buffer);

            const std::string& exitStr = "someShit";
            if (str_buffer == exitStr) {
                break;
            }
            std::cout << "The SqlQuery was: ";
            std::cout << str_buffer << std::endl;

            QSqlQuery query(QString::fromStdString(str_buffer));
            ShowSqlQueryResult(query);

            for (int i = 0; i < bytesRead - 1; ++i) {
                buffer[i] = '\0';
            }
        }

        std::string response = "Good talking to you\n";
        send(connection, response.c_str(), response.size(), 0);

        return kSuccess;
    }

    ~Server() {
        close(connection);
        close(sockfd);
        close(bind_status);
    }

private:
    QSqlDatabase db;
    int connection = -1;
    int bind_status = -1;
    int listen_status = -1;
    int sockfd = -1;
};

int main() {
    QSqlDatabase db;
    bool db_status = ConnectToDatabase(db);

    if (db_status) {
        Server server(db);
        bool server_status = server.Start();
    } else {
        const std::string& kErrorOpenDB = "Cant connect to database";
        Log log;
        log.Write(kErrorOpenDB);
        std::cerr << kErrorOpenDB << std::endl;
    }

    return 0;
}

void Success() {
    QString request = InputSqlQuery();
    QSqlQuery query(request);
    ShowSqlQueryResult(query);
}

bool ConnectToDatabase(QSqlDatabase& db) {
    const QString& file_with_db_parametrs = ".db_parametrs";
    QFile file_stream(file_with_db_parametrs);

    if (!file_stream.open(QIODevice::ReadOnly | QIODevice::Text)) {
        const std::string& kErrorMessage = "Cant open file with database parametrs";
        std::cerr << kErrorMessage << std::endl;
        return false;   // cant connect to database
    }

    QList wholeFile = file_stream.readLine().split(' ');
    const QString& driver_name = wholeFile[0],
                    db_name = wholeFile[1],
                    user_name = wholeFile[2],
                    pass = wholeFile[3],
                    host_name = wholeFile[4];


    db = QSqlDatabase::addDatabase(driver_name);
    db.setDatabaseName(db_name);
    db.setHostName(host_name);
    db.setUserName(user_name);
    db.setPassword(pass);


    Log log;
    log.Write(driver_name.toStdString());
    log.Write(db_name.toStdString());
    log.Write(host_name.toStdString());
    log.Write(user_name.toStdString());
    log.Write(pass.toStdString());

    return db.open();
}

bool StartServer() {
    return 1;
}