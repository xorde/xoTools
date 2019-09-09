#ifndef NLISTBOX_H
#define NLISTBOX_H

#include <QWidget>
#include <QScrollArea>
#include <QVBoxLayout>
#include "ui/contextablewidget.h"
#include "observablecollection.h"
#include <QLabel>
#include "clickablewidget.h"
#include "ui/uihelper.h"
#include <QDebug>
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT NListBox : public ContextableWidget
{
Q_OBJECT
public:
    explicit NListBox(QWidget *parent = nullptr);
    void onContextChanged();
    virtual QWidget* createItemWidget(QObject *obj)
    {
        QWidget *itemWidget = itemCreateDelegate(obj);
        return itemWidget;
    }

    void clear();
    int count();
    QObject* at(int index);
    void setEmptyListText(QString text);
    QLabel *_uiEmptyListLabel = nullptr;
public:
    Q_PROPERTY(ObservableCollection* items READ getItems WRITE setItems MEMBER m_items NOTIFY itemsChanged)
    Q_INVOKABLE void setItems(ObservableCollection* value);
    Q_INVOKABLE ObservableCollection* getItems() { return m_items; }
    Q_PROPERTY(QObject* SelectedItem READ getSelectedItem WRITE setSelectedItem NOTIFY changedSelectedItem)
    Q_INVOKABLE void setSelectedItem(QObject* value);
    Q_INVOKABLE QObject* getSelectedItem();
    Q_PROPERTY(int SelectedIndex MEMBER m_selectedIndex READ getSelectedIndex WRITE setSelectedIndex NOTIFY changedSelectedIndex)
    Q_INVOKABLE void setSelectedIndex(int value);
    Q_INVOKABLE int getSelectedIndex();
    Q_PROPERTY(Qt::Orientation Orientation MEMBER m_orientation READ getOrientation WRITE setOrientation NOTIFY changedOrientation)
    Q_INVOKABLE void setOrientation(Qt::Orientation value);
    Q_INVOKABLE Qt::Orientation getOrientation();



    void setTransparentBackground();

    void setBackground(QColor color);

    void setBackgroundAndBorder(QColor backgroundColor, quint8 borderThickness, QColor borderColor);


signals:
    Q_SIGNAL void itemsChanged(ObservableCollection* value);
    void internalRebuildStart();
    void internalRebuildEnd();
    Q_SIGNAL void changedSelectedItem(QObject* value);
    Q_SIGNAL void selectedItemDoubleClicked(QObject* value);
    Q_SIGNAL void anythingSelected(bool selected);
    Q_SIGNAL void changedSelectedIndex(int value);
    Q_SIGNAL void changedOrientation(Qt::Orientation value);

     QWidget* itemCreateDelegate(QObject *data);

public slots:
    void rebuild();
protected:
    ObservableCollection* m_items = nullptr;
    QList<ClickableWidget*> m_widgets;
    QScrollArea *m_scroll = nullptr;
    QGridLayout *m_uiLayout = nullptr;
    QWidget* m_uiSpacer = nullptr;
    int m_selectedIndex = -1;
    Qt::Orientation m_orientation = Qt::Vertical;
protected slots:
    void itemClicked(int clicks);
    void onItemsCountChanged(int count);
};

#endif // NLISTBOX_H
