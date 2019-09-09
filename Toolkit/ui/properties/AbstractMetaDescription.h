#ifndef ABSTRACTMETADESCRIPTION_H
#define ABSTRACTMETADESCRIPTION_H

#include <QObject>
#include <QVariant>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT AbstractMetaDescription : public QObject
{
    Q_OBJECT
public:
    explicit AbstractMetaDescription(QObject *parent = nullptr);

    virtual QMetaEnum enumerator() = 0;
    virtual bool isEnumType() = 0;

    QString m_def;
    double m_min;
    double m_max;
    double m_step;

    QString typeName;
    QString displayName;
    QString bindedVisibilitySource;
    QString bindedValue;
    QString name;
    QString category;
    QString pickOption;
    QString pickExtensions;
    QString specialType;

    QString suffix = "";
    QString prefix = "";
    QString description = "";
    QStringList m_enumStringList;

    QVariant::Type typeId;

    int priority = INT_MAX;

    bool showDefault = true;
    bool isReadOnly = false;
    bool changeable = false;
    bool visibleInUI = false;
    bool hasMin = false;
    bool hasMax = false;
    bool hasDefault = false;
    bool hasStep = false;
    bool hasSpecialType = false;

public slots:
    virtual bool setValue(const QVariant &value) = 0;
    virtual const QVariant getValue() = 0;
    virtual void setDefault() = 0;
    virtual void valueChangedSlot() = 0;
    virtual bool setTextValue(QString value) = 0;

signals:
    void valueChanged(QVariant value);
};

#endif // ABSTRACTMETADESCRIPTION_H
