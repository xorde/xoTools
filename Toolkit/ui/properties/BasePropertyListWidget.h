#ifndef BASEPROPERTYLISTWIDGET_H
#define BASEPROPERTYLISTWIDGET_H

#include <QTabWidget>
#include <QWidget>
#include <QGridLayout>

#include "BasePropertyView.h"
#include "helpers/ConnectionHelper.h"

class BasePropertyListWidget : public QWidget
{
    Q_OBJECT
public:

    struct PropertyWidgetInfo
    {
        QString propertyName = "";
        QWidget *headerWidget = nullptr;
        BasePropertyView *editorWidget = nullptr;
        int gridRow = 0;
    };

    explicit BasePropertyListWidget(QWidget *parent = nullptr);

public:
    void showHeaders(bool show);
    void showDefaultButtons(bool show);

    void hideItems(QStringList propertyNames);
    void showItems(QStringList propertyNames, bool show);

    void setEditorsEnabled(bool enabled);

    PropertyWidgetInfo getWidgetInfo(QString propertyName);
    PropertyWidgetInfo getWidgetInfo(int index);

    void setFocusAt(int index);
    void setFocusAt(QString propertyName);
    void setFocusAt(PropertyWidgetInfo propertyInfo);
    void setFocusAtFirst();

protected:
    void addPropertyToLayout(AbstractMetaDescription *property, QGridLayout *layout);
    BasePropertyView* getPropertyView(AbstractMetaDescription* property, int &columnSpan);
    QGridLayout *m_grid = nullptr;

    QList<PropertyWidgetInfo> m_items;
    ConnectionHelper m_connections;

};

#endif // BASEPROPERTYLISTWIDGET_H
