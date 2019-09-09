#ifndef NLIST_H
#define NLIST_H

#include <QObject>
#include <QDebug>
#include <QList>

#include "xotools_global.h"

template <typename T>
class XOTOOLSSHARED_EXPORT NList
{
public:
    explicit NList()
    {

    }

    static NList<T> fromQList(QList<T> t)
    {
        NList<T> list;
        list.list = t;
        return list;
    }


    QList<T> list;

    const T &operator[](int i) const
    {
        if (i < 0 || i >= list.length())
        {
            qDebug() << "[" << i << "] index of out range" << list.count();
        }
        return list[i];
    }

    T &operator[](int i)
    {
        if (i < 0 || i >= count())
        {
            qDebug() << "[" << i << "] index of out range" << count();
        }

        return list[i];
    }

    void append(const T &t)
    {
        list.append(t);
    }

    NList<T> &operator << (const T &t)
    {
        list << t;
        return *this;
    }

    const T &at(int i) const
    {
        return list.at(i);
    }

    int count() const
    {
        return list.count();
    }

    int length() const
    {
        return list.count();
    }

    int size() const
    {
        return list.count();
    }

    void clear()
    {
        list.clear();
    }

    bool contains(const T &t) const
    {
        return list.contains(t);
    }
};
#endif // NLIST_H
