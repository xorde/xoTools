#ifndef OBSERVABLECOLLECTION_H
#define OBSERVABLECOLLECTION_H

#include <QObject>
#include <QList>
#include "nlist.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT ObservableCollection : public QObject
{
Q_OBJECT
public:
    explicit ObservableCollection(QObject *parent = nullptr);

    QObject *at(int index);

    void add(QObject* item);
    void add(QList<QObject*> items);
    void add(NList<QObject*> items);
    void insertAt(QObject* item, int index);

    void removeAt(int index);
    void remove(QObject* item);

    int indexOf(QObject* item);
    bool contains(QObject* item);

    int count();

    void clear();

private:
    QList<QObject*> m_items;
    void changeCollectionEvents(QList<QObject*> newItems, QList<QObject*> oldItems);

signals:
    void collectionChanged(QList<QObject*> newItems, QList<QObject*> oldItems);
    void countChanged(int count);

};

#endif // OBSERVABLECOLLECTION_H
