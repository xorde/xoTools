#include "observablecollection.h"

ObservableCollection::ObservableCollection(QObject *parent) : QObject(parent)
{

}

QObject *ObservableCollection::at(int index)
{
    if (index >= 0 && index < m_items.count())
    {
        return m_items[index];
    }
    return nullptr;
}

void ObservableCollection::add(QObject *item)
{
    if (item)
    {
        m_items.append(item);
        QList<QObject*> newItems;
        newItems << item;
        changeCollectionEvents(newItems, QList<QObject*>());
    }
}

void ObservableCollection::add(QList<QObject *> items)
{
    foreach (QObject* item, items)
    {
        m_items.append(item);
    }
    changeCollectionEvents(items, QList<QObject*>());
}

void ObservableCollection::add(NList<QObject *> items)
{
    QList<QObject*> qlistitems;
    foreach (QObject* item, items.list)
    {
        m_items.append(item);
        qlistitems.append(item);
    }
    changeCollectionEvents(qlistitems, QList<QObject*>());
}

void ObservableCollection::insertAt(QObject *item, int index)
{
    if (item)
    {
        m_items.insert(index, item);
        QList<QObject*> newItems;
        newItems << item;
        changeCollectionEvents(newItems, QList<QObject*>());
    }
}

void ObservableCollection::removeAt(int index)
{
    if (index >= 0 && index < m_items.count())
    {
        QList<QObject*> oldItems;
        oldItems << m_items.at(index);
        m_items.removeAt(index);
        changeCollectionEvents(QList<QObject*>(), oldItems);
    }
}

void ObservableCollection::remove(QObject *item)
{
    int i = indexOf(item);
    if (i>=0) removeAt(i);
}

int ObservableCollection::indexOf(QObject *item)
{
    return m_items.indexOf(item);
}

bool ObservableCollection::contains(QObject *item)
{
    return indexOf(item) >= 0;
}

int ObservableCollection::count()
{
    return m_items.count();
}

void ObservableCollection::clear()
{
    m_items.clear();
    changeCollectionEvents(QList<QObject*>(), QList<QObject*>());
}

void ObservableCollection::changeCollectionEvents(QList<QObject *> newItems, QList<QObject *> oldItems)
{
    emit collectionChanged(newItems, oldItems);
    emit countChanged(count());
}
