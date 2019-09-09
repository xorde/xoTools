#ifndef ABSTRACTSETTINGS_H
#define ABSTRACTSETTINGS_H

#include <QObject>
#include "ui/properties/metadescriptor.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT AbstractSettings : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSettings(QObject *parent = nullptr);
    virtual AbstractMetaDescriptor* metaDescriptor() = 0;

signals:
    void propertyChanged(const QString name);
};

#endif // ABSTRACTSETTINGS_H
