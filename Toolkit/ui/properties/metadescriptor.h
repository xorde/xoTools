#ifndef METADESCRIPTOR_H
#define METADESCRIPTOR_H

#include <QString>
#include <QList>
#include <QObject>
#include <QDebug>

#include <QAbstractListModel>
#include "metadescription.h"
#include "commontypes.h"
#include "AbstractMetaDescriptor.h"

class MetaDescriptor : public AbstractMetaDescriptor
{
    Q_OBJECT
public:
    explicit MetaDescriptor(QObject *parent = nullptr);

    Q_PROPERTY(QMetaProperty* item READ getItem WRITE setItem MEMBER m_item NOTIFY itemChanged)
    Q_INVOKABLE void setItem(QMetaProperty* value);
    QMetaProperty* getItem();

    QString getClassName() override;

public slots:
    void set(QObject *object) override;

signals:
    void itemChanged(QMetaProperty* value);

protected:
    QMetaProperty* m_item;
    QObject* m_object;
    void parse(QObject *object);    
};

#endif // METADESCRIPTOR_H
