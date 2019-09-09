#ifndef PROPERTYTABWIDGET_H
#define PROPERTYTABWIDGET_H

#include <QTabWidget>
#include <QWidget>

#include "ui/properties/metadescriptor.h"
#include "PropertiesSorter.h"
#include "BasePropertyListWidget.h"
#include "AbstractSettings.h"

class PropertyTabWidget : public BasePropertyListWidget
{
    Q_OBJECT
public:
    explicit PropertyTabWidget(AbstractSettings *source, QWidget *parent = nullptr);
    ~PropertyTabWidget();

    QTabWidget *getTabWidget();

    void moveTab(QString categoryName, int index, bool focus = true);

private:
    PropertiesSorter* sorter = nullptr;
    QHash<QString, int> indexByCategory;
    QList<bool> visiblities;
    QList<QWidget*> m_tabWidgets;
    QTabWidget *m_tabs = nullptr;
};

#endif // PROPERTYTABWIDGET_H
