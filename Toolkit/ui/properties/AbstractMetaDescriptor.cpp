#include "AbstractMetaDescriptor.h"
#include <QFile>
#include "fileutilities.h"

AbstractMetaDescriptor::AbstractMetaDescriptor(QObject *parent) : QObject(parent)
{
}

QList<AbstractMetaDescription *> AbstractMetaDescriptor::getProperties()
{
    return m_properties;
}

QMap<QString, QString> AbstractMetaDescriptor::getPropertiesNameAndType()
{
    QMap<QString, QString> map;
    auto list = getProperties();
    foreach (auto prop, list)
    {
        map.insert(prop->name, prop->typeName);
    }
    return map;
}

void AbstractMetaDescriptor::clear()
{
    m_properties.clear();
}

int AbstractMetaDescriptor::count() const
{
    return m_properties.count();
}

AbstractMetaDescription *AbstractMetaDescriptor::at(int index)
{
    if (index >= 0 && index < m_properties.count())
        return m_properties.at(index);

    return nullptr;
}

void AbstractMetaDescriptor::loadXmlContent(QStringRef in_nextName, QXmlStreamReader& in_stream)
{
    in_stream.readNext();
    while(!in_stream.atEnd())
    {
        if(in_stream.isStartElement())
        {
            auto name = in_stream.name();
            if (name == in_nextName)
                return;

            for(AbstractMetaDescription* property : m_properties)
            {
                if (!property->changeable)
                    continue;

                if (property->name == name)
                {
                    bool res = QString::compare(property->typeName, QString("QByteArray"), Qt::CaseInsensitive) == 0;
                    property->setValue(res ? QVariant(FileUtilities::base64_decode_as_bytes(in_stream.readElementText())) : QVariant(in_stream.readElementText()));
                    break;
                }
            }
        }
        in_stream.readNext();
    }
}

bool AbstractMetaDescriptor::saveXmlContent(QXmlStreamWriter& in_stream)
{
    in_stream.writeStartElement(getClassName());

    for(AbstractMetaDescription* property : m_properties)
    {
        if (!property->changeable)
            continue;

        in_stream.writeStartElement(property->name);

        if (QString::compare(property->typeName, QString("StringMap"), Qt::CaseInsensitive) == 0)
        {
            QMap<QString, QString> map = property->getValue().value<QMap<QString, QString>>();
            foreach (QString key, map.keys())
            {
                in_stream.writeTextElement(key, map[key]);
            }
        }
        else if (QString::compare(property->typeName, QString("QByteArray"), Qt::CaseInsensitive) == 0)
        {
            QByteArray ba = property->getValue().value<QByteArray>();
            in_stream.writeCharacters(FileUtilities::base64_encode(ba));
        }
        else
            in_stream.writeCharacters(property->getValue().toString());

        in_stream.writeEndElement();
    }
    in_stream.writeEndElement();
    return true;
}

bool AbstractMetaDescriptor::setValue(QString propertyName, QVariant propertyValue)
{
    foreach(auto prop, m_properties)
    {
        if (prop->name.compare(propertyName, Qt::CaseInsensitive) == 0)
        {
            return prop->setValue(propertyValue);
        }
    }
    return false;
}

bool AbstractMetaDescriptor::setTextValue(QString propertyName, QString text)
{
    for(auto prop : m_properties)
    {
        if (prop->name.compare(propertyName, Qt::CaseInsensitive) == 0)
        {
            return prop->setTextValue(text);
        }
    }
    return false;
}

bool saveFileContent(const QString &filename, const QString &content)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(content.toUtf8());
        file.close();
        return true;
    }
    return false;
}

bool AbstractMetaDescriptor::save(QString filename)
{
    return saveFileContent(filename, saveAsXmlString());
}

bool AbstractMetaDescriptor::saveToJsonFile(QString filename)
{
   return saveFileContent(filename, saveToJsonString());
}

QString AbstractMetaDescriptor::saveAsXmlString()
{
    QString result;
    QXmlStreamWriter stream(&result);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement(getClassName());

    for(AbstractMetaDescription* property : m_properties)
    {
        if (!property->changeable)
            continue;

        stream.writeStartElement(property->name);
        if (QString::compare(property->typeName, QString("StringMap"), Qt::CaseInsensitive) == 0)
        {
            QMap<QString, QString> map = property->getValue().value<QMap<QString, QString>>();
            foreach (QString key, map.keys())
            {
                stream.writeTextElement(key, map[key]);
            }
        }
        else if (QString::compare(property->typeName, QString("QByteArray"), Qt::CaseInsensitive) == 0)
        {
            QByteArray ba = property->getValue().value<QByteArray>();
            stream.writeCharacters(FileUtilities::base64_encode(ba));
        }
        else
            stream.writeCharacters(property->getValue().toString());

        stream.writeEndElement();
    }
    stream.writeEndElement();
    stream.writeEndDocument();

    return result;
}

QJsonObject AbstractMetaDescriptor::saveToJson()
{
    return QJsonObject::fromVariantMap(getQVariantMap());
}

QString AbstractMetaDescriptor::saveToJsonString(QJsonDocument::JsonFormat format)
{
    return QString(saveToJsonByteArray(format));
}

QByteArray AbstractMetaDescriptor::saveToJsonByteArray(QJsonDocument::JsonFormat format)
{
    QJsonDocument doc(saveToJson());
    return doc.toJson(format);
}

bool AbstractMetaDescriptor::load(QString filename)
{
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QString fileContent = QString(file.readAll());
        bool result = loadFromXmlString(fileContent);
        file.close();
        return result;
    }
    return false;
}

bool AbstractMetaDescriptor::loadFromXmlString(QString xml)
{
    QXmlStreamReader stream(xml);
    stream.readNext();
    while(!stream.atEnd())
    {
        if(stream.isStartElement())
        {
            auto name = stream.name();
            for(AbstractMetaDescription* property : m_properties)
            {
                if (!property->changeable)
                    continue;

                if (property->name == name)
                {
                    bool res = QString::compare(property->typeName, QString("QByteArray"), Qt::CaseInsensitive)==0;
                    property->setValue(res ? QVariant(FileUtilities::base64_decode_as_bytes(stream.readElementText())) : QVariant(stream.readElementText()));
                    break;
                }
            }
        }
        stream.readNext();
    }
    return true;
}

bool AbstractMetaDescriptor::loadFromJson(QString jsonString)
{
    if (jsonString.isEmpty()) return false;
    return loadFromJson(jsonString.toUtf8());
}

bool AbstractMetaDescriptor::loadFromJson(QByteArray jsonByteArray)
{
    if (jsonByteArray.isEmpty() || jsonByteArray.isNull()) return false;

    auto json_doc = QJsonDocument::fromJson(jsonByteArray);
    return loadFromJson(json_doc);
}

bool AbstractMetaDescriptor::loadFromJson(QJsonDocument jsonDocument)
{
    if (jsonDocument.isEmpty()) return false;

    if (jsonDocument.isNull()) {
        qDebug() << "Failed to create JSON doc." << Q_FUNC_INFO;
        return false;
    }
    if (!jsonDocument.isObject()) {
        qDebug() << "JSON is not an object." << Q_FUNC_INFO;
        return false;
    }

    QJsonObject json_obj = jsonDocument.object();
    return loadFromJson(json_obj);
}

bool AbstractMetaDescriptor::loadFromJson(QJsonObject jsonObject)
{
    if (jsonObject.isEmpty()) {
        qDebug() << "JSON object is empty" << Q_FUNC_INFO;
        return false;
    }
    return loadFromQVariantMap(jsonObject.toVariantMap());
}

bool AbstractMetaDescriptor::loadFromQVariantMap(const QVariantMap& map)
{
    for(AbstractMetaDescription* property : m_properties)
    {
        if (!property->changeable)
            continue;

        if (map.contains(property->name))
            property->setValue(map[property->name]);
    }
    return true;
}

bool AbstractMetaDescriptor::loadFromJsonFile(QString jsonFile)
{
    QFile file(jsonFile);
    if (file.open(QIODevice::ReadOnly))
    {
        QString fileContent = QString(file.readAll());
        bool result = loadFromJson(fileContent);
        file.close();
        return result;
    }
    return false;
}

QVariantMap AbstractMetaDescriptor::getQVariantMap()
{
    QVariantMap map;
    for(AbstractMetaDescription* property : m_properties)
    {
        if (property->changeable)
        map.insert(property->name, property->getValue());
    }
    return map;
}

void AbstractMetaDescriptor::defaults()
{
    for(AbstractMetaDescription *property : m_properties)
    {
        if (!property->changeable)
            continue;

        if (property->hasDefault)
            property->setDefault();
    }
}

