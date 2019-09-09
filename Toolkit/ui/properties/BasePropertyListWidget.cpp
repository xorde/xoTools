#include "BasePropertyListWidget.h"
#include "BoolPropertyView.h"
#include "EnumPropertyView.h"
#include "IntPropertyView.h"
#include "DoublePropertyView.h"
#include "DatePropertyView.h"
#include "StringPropertyView.h"
#include "SpecialPropertyView.h"
#include "ColorPropertyView.h"
#include "MultilinePropertyView.h"
#include "SliderPropertyView.h"
#include "EnumStringListPropertyView.h"
#include <QCoreApplication>

BasePropertyListWidget::BasePropertyListWidget(QWidget *parent) : QWidget(parent)
{
    setContentsMargins(0,0,0,0);
    m_grid = new QGridLayout(this);
    m_grid->setContentsMargins(0,0,0,0);

}

void BasePropertyListWidget::showHeaders(bool show)
{
    foreach (PropertyWidgetInfo info, m_items)
    {
        if (info.headerWidget != nullptr)
        {
            //TODO: if show and the property should be hidden - it should stay hidden
            info.headerWidget->setVisible(show);
        }
    }
}

void BasePropertyListWidget::showDefaultButtons(bool show)
{
    foreach (PropertyWidgetInfo info, m_items)
    {
        if (info.editorWidget != nullptr)
        {
            info.editorWidget->showDefaultButton(show);
        }
    }
}

void BasePropertyListWidget::hideItems(QStringList propertyNames)
{
    showItems(propertyNames, false);
}

void BasePropertyListWidget::showItems(QStringList propertyNames, bool show)
{
    foreach (PropertyWidgetInfo info, m_items)
    {
        if (propertyNames.contains(info.propertyName, Qt::CaseInsensitive))
        {
            if (info.editorWidget) info.editorWidget->setVisible(show);
            if (info.headerWidget) info.headerWidget->setVisible(show);
        }
    }
}

void BasePropertyListWidget::setEditorsEnabled(bool enabled)
{
    foreach (auto item, m_items)
    {
        auto editor = item.editorWidget;
        if (editor) editor->setEnabled(enabled);
    }
}

BasePropertyListWidget::PropertyWidgetInfo BasePropertyListWidget::getWidgetInfo(QString propertyName)
{
    foreach (PropertyWidgetInfo info, m_items)
    {
        if (propertyName.compare(info.propertyName, Qt::CaseInsensitive) == 0)
        {
            return info;
        }
    }
    return PropertyWidgetInfo();
}

BasePropertyListWidget::PropertyWidgetInfo BasePropertyListWidget::getWidgetInfo(int index)
{
    if (index >= 0 && index < m_items.count())
    {
        return m_items[index];
    }
    return PropertyWidgetInfo();
}

void BasePropertyListWidget::setFocusAt(int index)
{
    setFocusAt(getWidgetInfo(index));
}

void BasePropertyListWidget::setFocusAt(QString propertyName)
{
     setFocusAt(getWidgetInfo(propertyName));
}

#include <QTimer>
void BasePropertyListWidget::setFocusAt(BasePropertyListWidget::PropertyWidgetInfo propertyInfo)
{
    if (propertyInfo.editorWidget)
    {
        auto editor = propertyInfo.editorWidget->getEditor();
        if (editor)
        {
            QTimer::singleShot(0, editor, SLOT(setFocus()));
        }
    }
}

void BasePropertyListWidget::setFocusAtFirst()
{
    setFocusAt(0);
}

void BasePropertyListWidget::addPropertyToLayout(AbstractMetaDescription *property, QGridLayout *layout)
{
    auto label = new QLabel(QCoreApplication::translate("Generic", qPrintable(property->displayName)));
    label->setContentsMargins(0, 0, 0, 0);
    label->setObjectName("propertyHeaderLabel");
    if (!property->description.isEmpty())
    {
        label->setToolTip(QCoreApplication::translate("Generic", qPrintable(property->description)));
    }

    int columnSpan = 1;
    auto parser = getPropertyView(property, columnSpan);
    parser->setContentsMargins(0, 0, 5, 0);

    if (property->isReadOnly)
    {
        parser->setEnabled(false);
    }

    int index = layout->rowCount();
    layout->addWidget(label, index, 0);
    layout->addWidget(parser, index, 1, 1, columnSpan);

    PropertyWidgetInfo info;
    info.propertyName = property->name;
    info.headerWidget = label;
    info.editorWidget = parser;
    info.gridRow = index;

    m_items << info;
}

BasePropertyView *BasePropertyListWidget::getPropertyView(AbstractMetaDescription *property, int &columnSpan)
{
    auto type = property->typeName;

    columnSpan = 1;

    if (property->hasSpecialType)
    {
        if ((property->specialType.contains("PickFile", Qt::CaseInsensitive) || property->specialType.contains("PickFolder",Qt::CaseInsensitive)))
        {
            columnSpan = 2;
            return new SpecialPropertyView(property, this);
        }
        else if (property->specialType.contains("Multiline", Qt::CaseInsensitive))
        {
            columnSpan = 2;
           return new MultilinePropertyView(property, this);
        }
        else if (property->specialType.compare("Slider", Qt::CaseInsensitive) == 0)
        {
            return new SliderPropertyView(property, this);
        }
        else if (property->specialType.compare("EnumStringList", Qt::CaseInsensitive) == 0)
        {
            return new EnumStringListPropertyView(property, this);
        }
        else if (property->typeName.compare("QString", Qt::CaseInsensitive) == 0 && property->specialType.compare("StringList", Qt::CaseInsensitive) == 0)
        {
            return new EnumStringListPropertyView(property, this);
        }
    }
    if (property->isEnumType())
        return new EnumPropertyView(property, this);
    if (type.contains("bool")) return new BoolPropertyView(property, this);
    if (type.contains("int")) return new IntPropertyView(property, this);
    if (type.contains("QDate")) return new DatePropertyView(property, this);
    if (type.contains("QDateTime")) return new DatePropertyView(property, this);
    if (type.contains("double") || type.contains("float")) return new DoublePropertyView(property, this);
    if (type.contains("QColor")) return new ColorPropertyView(property, this);


    columnSpan = 2;
    return new StringPropertyView(property, this);
}
