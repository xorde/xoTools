#ifndef METADESCRIPTION
#define METADESCRIPTION

#include "AbstractMetaDescription.h"
#include <QObject>
#include <QMetaProperty>
#include <QVariant>
#include <QString>

class  MetaDescription : public AbstractMetaDescription
{
Q_OBJECT
public:
    explicit MetaDescription(QObject *parent = nullptr);

public slots:
    bool setValue(const QVariant &value) override;
    const QVariant getValue() override;
    void setDefault() override;
    void valueChangedSlot() override;
    bool setTextValue(QString value) override;
    void set(int metapropertyindex, QObject *object);

public:
    QMetaEnum enumerator() override;
    bool isEnumType() override;

    int m_metaindex;
    QObject* m_object;

private:
    QMetaProperty metaprop();
    bool assignDouble(double& out, QString& source);
    QString stringForAlias(QString alias);
    QString clinfo(QString value);
    int clindex(QString value);
};

#endif // METADESCRIPTION

