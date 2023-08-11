#pragma once
#include <cstring>
#include "logging.h"

#include <QSqlDatabase>
#include <QtSql/QtSql>


QString InputSqlQuery();
void ShowSqlQueryResult(QSqlQuery& result);
bool ConnectToDatabase(QSqlDatabase& db);
QString GetJsonFormatQuery(QSqlQuery& query);

