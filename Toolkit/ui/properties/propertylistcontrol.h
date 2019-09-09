#ifndef PROPERTYLISTCONTROL_H
#define PROPERTYLISTCONTROL_H

#include <QDebug>
#include <QWidget>
#include "AbstractSettings.h"
#include "metadescriptor.h"
#include "PropertiesSorter.h"
#include "BasePropertyListWidget.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT PropertyListControl : public BasePropertyListWidget
{
    Q_OBJECT
public:
    enum ViewMode
    {
        ViewAll,
        ViewSingleCategory,
        ViewSingleProperty
    };

    explicit PropertyListControl(AbstractSettings* source, QString viewOption = "", ViewMode mode = ViewAll, QWidget *parent = nullptr);
    ~PropertyListControl() override;
    QSize sizeHint() const override;

    int getHeaderColumnWidth();    

    QGridLayout* getLayout() const;

    void addWidgetToFirstRow(QWidget* widget, int column = 0, Qt::Alignment alignment = Qt::Alignment());
    void addWidgetToLastRow(QWidget* widget, int column = 0, Qt::Alignment alignment = Qt::Alignment());
    void addWidgetToLayout(QWidget* widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::Alignment());

    void addLayoutToFirstRow(QLayout* layout, int column = 0, Qt::Alignment alignment = Qt::Alignment());
    void addLayoutToLastRow (QLayout* layout, int column = 0, Qt::Alignment alignment = Qt::Alignment());
    void addLayoutToLayout  (QLayout* layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment = Qt::Alignment());

    bool substituteEditor(const QString& propertyName, QWidget *editor);

private:
    QString viewOptions;
    PropertiesSorter* m_pSorter = nullptr;
    QHash<QString, int> indexByCategory;
    PropertyListControl::ViewMode viewMode;
    QGridLayout* m_widgetLayout = nullptr;
};

#endif // PROPERTYLISTCONTROL_H
