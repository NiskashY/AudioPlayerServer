#include "db_manipulation.h"

QString InputSqlQuery() {   // tmp
    const std::string& kSqlQuery = "Input Sql Query: ";

    std::string query;
    std::cout << kSqlQuery;
    std::cin >> query;

    QString q_query = QString::fromStdString(query);
    return q_query;
}

void ShowSqlQueryResult(QSqlQuery &result) {
    int columns_count = result.record().count();
    while (result.next()) {
        for (int i = 0; i < columns_count; ++i) {
            std::cout << result.value(i).toString().toStdString() << ' ';
        }
        std::cout << '\n';
    }
}

bool ConnectToDatabase(QSqlDatabase &db) {
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

QString GetJsonFormatQuery(QSqlQuery& query) {
    QJsonDocument json;
    QJsonArray recordsArray;

    while (query.next()) {
        QJsonObject recordObject;
        const int columns_count = query.record().count();
        for (int column = 0; column < columns_count; ++column) {
            recordObject.insert(query.record().fieldName(column), QJsonValue::fromVariant(query.value(column)));
        }
        recordsArray.push_back(recordObject);
    }
    json.setArray(recordsArray);

    return json.toJson();
}