#ifndef JSON_H
#define JSON_H


#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

//使用说明


class JSON {
public:
    JSON();

    QJsonObject getJson();
    QJsonObject loadJson(const QString& filepath);
    void writeJson(const QString key, bool value);
    void writeJson(const QString key, int value);
    void writeJson(const QString key, double value);
    void writeJson(const QString key, QString value);
    void writeJson(const QString key, bool* array, int length);
    void writeJson(const QString key, int* array, int length);
    void writeJson(const QString key, double* array, int length);
    void writeJson(const QString key, QJsonObject object);
    bool saveJson(const QString& filepath);
    QString toString();

private:
    QJsonObject json;
};

#endif // JSON_H
