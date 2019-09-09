#ifndef TEXTHELPER_H
#define TEXTHELPER_H

#include <QString>
#include <QFileInfo>
#include <QDateTime>
#include <QMap>
#include <QRect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaEnum>

namespace TextHelper
{
    QString FileName(QString path);

    QString FileExtension(QString path);

    QString trim(QString string, QChar trimmer);
    QString trim(QString string, QString trimmer);

    QString TimeToText(double seconds, bool showMs = true);

    QString DateTimeFileString(QDateTime dt = QDateTime(), bool withMs = true);

    QString printLikeTable(QMap<QString, QString> values, QString headerSeparator = "");

    QStringList reverse(QStringList source);

    QString randomString(QStringList source);

    QString QRectToString(QRect rect);

    QRect QRectFromString(QString str);

    QString QListPointsToString(QList<QPoint> &list);

    QList<QPoint> QListPointsFromString(QString str);

    QString toJsonString(QJsonObject obj, QJsonDocument::JsonFormat format = QJsonDocument::Compact);

    QString translit(QString text);

    QString russianAgeSuffix(int years);

    QString join(QStringList &list, QString separator, QString leftwrapper = "", QString rightwrapper = "");

    int LevenshteinDistance(QString string1,QString string2);

    int LevenshteinDistanceNoMarksAndCases(QString string1, QString string2);

    template <typename T> QString EnumToString(T value)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        return QString(metaEnum.valueToKey(value));
    }

    template <typename T> T EnumFromString(QString value, bool *ok = nullptr)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        bool okay = false;
        int val = metaEnum.keyToValue(value.toUtf8().data(), &okay);
        if (!okay) val = metaEnum.keyToValue(metaEnum.key(0));
        if (ok) *ok = okay;
        return (T)val;
    }

    template <typename T> QStringList EnumTextValues()
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<T>();
        QStringList values;
        for (int i = 0, sz = metaEnum.keyCount(); i<sz; i++)
        {
            values << QString(metaEnum.key(i));
        }
        return values;
    }
}


QString boolify(bool value);
#endif // TEXTHELPER_H
