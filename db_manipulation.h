#pragma once
#include <cstring>

QString InputSqlQuery() {   // tmp
    const std::string& kSqlQuery = "Input Sql Query: ";

    std::string query;
    std::cout << kSqlQuery;
    std::cin >> query;

    QString q_query = QString::fromStdString(query);
    return q_query;
}


void ShowSqlQueryResult(QSqlQuery& result) {
    int columns_count = result.record().count();
    while (result.next()) {
        for (int i = 0; i < columns_count; ++i) {
            std::cout << result.value(i).toString().toStdString() << ' ';
        }
        std::cout << '\n';
    }
}


