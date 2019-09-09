#ifndef ABSTRACTMETADESCRIPTOR_H
#define ABSTRACTMETADESCRIPTOR_H

#include <QObject>

#include "AbstractMetaDescription.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT AbstractMetaDescriptor : public QObject
{
    Q_OBJECT
public:
    AbstractMetaDescriptor(QObject* parent = nullptr);

    virtual void set(QObject *object) = 0;
    virtual QString getClassName() = 0;

    Q_INVOKABLE bool save(QString file);
    Q_INVOKABLE QString saveAsXmlString();
    bool saveToJsonFile(QString filename);
    Q_INVOKABLE virtual QJsonObject saveToJson();
    Q_INVOKABLE QString saveToJsonString(QJsonDocument::JsonFormat format = QJsonDocument::Compact);
    Q_INVOKABLE QByteArray saveToJsonByteArray(QJsonDocument::JsonFormat format = QJsonDocument::Compact);

    Q_INVOKABLE bool load(QString file);
    bool loadFromXmlString(QString xml);
    bool loadFromJson(QString jsonString);
    bool loadFromJson(QByteArray jsonByteArray);
    bool loadFromJson(QJsonDocument jsonDocument);
    bool loadFromJson(QJsonObject jsonObject);
    bool loadFromJsonFile(QString jsonFile);
    virtual bool loadFromQVariantMap(const QVariantMap &map);

    QVariantMap getQVariantMap();

    void defaults();

    int count() const;
    AbstractMetaDescription *at(int index);
    QList<AbstractMetaDescription *> getProperties();
    QMap<QString, QString> getPropertiesNameAndType();

    void loadXmlContent(QStringRef in_nextName, QXmlStreamReader &in_stream);
    bool saveXmlContent(QXmlStreamWriter &in_stream);

    bool setValue(QString propertyName, QVariant propertyValue);
    bool setTextValue(QString propertyName, QString text);

public slots:
    void clear();

protected:
    void parse(QObject *object);

    QList<AbstractMetaDescription*> m_properties;
};

#endif // ABSTRACTMETADESCRIPTOR_H
