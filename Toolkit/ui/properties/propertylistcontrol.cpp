#include <QCoreApplication>
#include <QDebug>

#include "propertylistcontrol.h"

#include "ui/uihelper.h"
#include <QScrollArea>
#include <QElapsedTimer>

PropertyListControl::PropertyListControl(AbstractSettings *source, QString viewOption, PropertyListControl::ViewMode mode, QWidget *parent) : BasePropertyListWidget(parent),  m_widgetLayout(nullptr)
{
    m_pSorter = new PropertiesSorter(source->metaDescriptor());

    viewMode = mode;
    viewOptions = viewOption;    

    auto widget = new QWidget();
    m_widgetLayout = new QGridLayout(widget);
    m_widgetLayout->setContentsMargins(0,0,0,0);
    m_widgetLayout->setSpacing(4);

    switch(viewMode)
    {
        case ViewAll:
            for(const auto& category : m_pSorter->categories)
            {
                if(m_pSorter->categories.length() > 1)
                {
                    int row = m_widgetLayout->rowCount();

                    //Make sure the first row is empty (for headers and stuff)
                    if (row == 0) row = 1;

                    auto label = new QLabel(QCoreApplication::translate("Generic", category.isNull() ? "Other" : qPrintable(category)));
                    label->setContentsMargins(0, 10, 0, 0);
                    label->setObjectName("propertyCategoryLabel");
                    label->setStyleSheet("font-weight: bold");
                    m_widgetLayout->addWidget(label, row, 0);

                    indexByCategory[category] = row;
                }

                QList<AbstractMetaDescription*> properties = m_pSorter->propertiesByCategory[category];
                for(auto property : properties)
                {                    
                    addPropertyToLayout(property, m_widgetLayout);
                    if(!property->bindedVisibilitySource.isNull())
                    {
                        int index = m_widgetLayout->rowCount() - 1;
                        AbstractMetaDescription* bindedProperty = m_pSorter->propertiesByName[property->bindedVisibilitySource];
                        auto checkVisibility = [=](QVariant value)
                        {                            
                            bool isVisible = value.toString() == property->bindedValue;
                            property->visibleInUI = isVisible;
                            m_widgetLayout->itemAtPosition(index, 0)->widget()->setVisible(isVisible);
                            m_widgetLayout->itemAtPosition(index, 1)->widget()->setVisible(isVisible);

                            bool categoryIsVisible = false;
                            for(const auto& property : properties)
                                if(property->visibleInUI)
                                {
                                    categoryIsVisible = true;
                                    break;
                                }

                            if(m_pSorter->categories.length() > 1)
                            {
                                int categoryIndex = indexByCategory[property->category];
                                m_widgetLayout->itemAtPosition(categoryIndex, 0)->widget()->setVisible(categoryIsVisible);
                            }
                        };
                        connect(bindedProperty, &AbstractMetaDescription::valueChanged, checkVisibility);
                        checkVisibility(bindedProperty->getValue());
                    }
                }


            }

            m_widgetLayout->setRowStretch(m_widgetLayout->rowCount(), 100);
        break;

        case ViewSingleProperty:
            addPropertyToLayout(m_pSorter->propertiesByName[viewOptions], m_widgetLayout);
        break;

        case ViewSingleCategory:
            for(const auto& property : m_pSorter->propertiesByCategory[viewOptions]) addPropertyToLayout(property, m_widgetLayout);
        break;
    }

    m_widgetLayout->setColumnStretch(2, 100);

    UIHelper::ChangeBackground(this, Qt::transparent);

    auto scrollArea = new QScrollArea();
    scrollArea->setMinimumSize(30,20);
    scrollArea->setWidget(widget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setContentsMargins(0,0,0,0);

    UIHelper::ChangeBackground(scrollArea, Qt::transparent);
    scrollArea->setStyleSheet("QScrollArea { background: transparent; border: 0; } QScrollArea > QWidget > QWidget { background: transparent; } \
                              QScrollArea > QWidget > QScrollBar { background: palette(base); }");

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    m_grid->addWidget(scrollArea, 0, 0, 1, 1);
    setContentsMargins(0,0,0,0);
}

PropertyListControl::~PropertyListControl()
{
    for(const auto& category : m_pSorter->categories)
    {
        QList<AbstractMetaDescription*> properties = m_pSorter->propertiesByCategory[category];
        for(const auto& property : properties)
        {
            if (!property->bindedVisibilitySource.isNull())
                disconnect(m_pSorter->propertiesByName[property->bindedVisibilitySource], nullptr, nullptr, nullptr);
        }
    }
    disconnect(this, nullptr, nullptr, nullptr);

    delete m_pSorter;
}

QGridLayout* PropertyListControl::getLayout() const
{
    return m_widgetLayout;
}

void PropertyListControl::addWidgetToFirstRow(QWidget *widget, int column, Qt::Alignment alignment)
{
    addWidgetToLayout(widget, 0, column, 1, 1, alignment);
}

void PropertyListControl::addWidgetToLastRow(QWidget *widget, int column, Qt::Alignment alignment)
{
    addWidgetToLayout(widget, m_widgetLayout->rowCount(), column, 1, 1, alignment);
}

void PropertyListControl::addWidgetToLayout(QWidget *widget, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_widgetLayout->addWidget(widget, row, column, rowSpan, columnSpan, alignment);
}

void PropertyListControl::addLayoutToFirstRow(QLayout *layout, int column, Qt::Alignment alignment)
{
    addLayoutToLayout(layout, 0, column, 1, 1, alignment);
}

void PropertyListControl::addLayoutToLastRow(QLayout *layout, int column, Qt::Alignment alignment)
{
    addLayoutToLayout(layout, m_widgetLayout->rowCount(), column, 1, 1, alignment);
}

void PropertyListControl::addLayoutToLayout(QLayout *layout, int row, int column, int rowSpan, int columnSpan, Qt::Alignment alignment)
{
    m_widgetLayout->addLayout(layout, row, column, rowSpan, columnSpan, alignment);
}

bool PropertyListControl::substituteEditor(const QString &propertyName, QWidget *editor)
{
    PropertyWidgetInfo info = getWidgetInfo(propertyName);

    if (info.propertyName.isEmpty()) return false;

    auto previousEditor = info.editorWidget;
    auto prevEditor = previousEditor->getEditor();

    if (prevEditor) previousEditor->nullifyEditor();

    previousEditor->getEditingLayout()->addWidget(editor, 0, 0);

    return true;
}

QSize PropertyListControl::sizeHint() const
{
    return layout()->sizeHint();
}

int PropertyListControl::getHeaderColumnWidth()
{
    int max = 0;
    if (m_widgetLayout && m_widgetLayout->count() > 0)
    {
        for (int i = m_widgetLayout->rowCount(); i >= 0; i--)
        {
            auto item = m_widgetLayout->itemAtPosition(i, 0);
            if (item)
            {
                int width = (item->sizeHint().width());
                if (width > max) max = width;
            }
        }
    }
    return max;
}
