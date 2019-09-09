#include "PropertyTabWidget.h"

#include <QLayout>
#include <QCoreApplication>
#include <QTabBar>

PropertyTabWidget::PropertyTabWidget(AbstractSettings* source, QWidget *parent) : BasePropertyListWidget(parent)
{
    if (!source) return;

    m_tabs = new QTabWidget();

    m_grid->addWidget(m_tabs, 0, 0, 1, 1);

    m_tabs->setStyleSheet("QTabBar::tab:disabled {width: 0px; color: transparent; background: transparent;}");

    sorter = new PropertiesSorter(source->metaDescriptor());

    int index = 0;

    for(const auto& category : sorter->categories)
    {
        auto tab = new QWidget();
        int tabIndex = m_tabs->addTab(tab, QCoreApplication::translate("Generic", category.isNull() ? "Other" : qPrintable(category)));
        m_tabs->tabBar()->setTabData(tabIndex, category);

        m_tabWidgets.push_back(tab);
        visiblities.push_back(true);
        indexByCategory[category] = index++;

        QGridLayout* layout = new QGridLayout(tab);

        QList<AbstractMetaDescription*> properties = sorter->propertiesByCategory[category];
        for(const auto& property : properties)
        {
            addPropertyToLayout(property, layout);
            if(!property->bindedVisibilitySource.isNull())
            {
                int index = layout->rowCount() - 1;
                AbstractMetaDescription* bindedProperty = sorter->propertiesByName[property->bindedVisibilitySource];
                auto checkVisibility = [=](QVariant value)
                {
                    bool isVisible = value.toString() == property->bindedValue;
                    property->visibleInUI = isVisible;
                    layout->itemAtPosition(index, 0)->widget()->setVisible(isVisible);
                    layout->itemAtPosition(index, 1)->widget()->setVisible(isVisible);

                    bool categoryIsVisible = false;
                    for(const auto& property : properties)
                        if(property->visibleInUI)
                        {
                            categoryIsVisible = true;
                            break;
                        }

                    int categoryIndex = indexByCategory[category];
                    if(visiblities[categoryIndex] != categoryIsVisible)
                    {
                        visiblities[categoryIndex] = categoryIsVisible;

                        while(m_tabs->tabBar()->count()) m_tabs->removeTab(0);

                        for(int i = 0, sz = m_tabWidgets.size(); i < sz; ++i)
                            if(visiblities[i])
                            {
                                QString category = sorter->categories[i];
                                m_tabs->addTab(m_tabWidgets[i], category.isNull() ? QCoreApplication::translate("Generic", "Other") : category);
                            }

                        QWidget* bindedTab = m_tabWidgets[indexByCategory[bindedProperty->category]];
                        for(int i = 0, sz = m_tabs->tabBar()->count(); i < sz; ++i)
                            if(bindedTab == m_tabs->widget(i))
                            {
                                m_tabs->setCurrentIndex(i);
                                break;
                            }
                    }
                };
                m_connections << connect(bindedProperty, &AbstractMetaDescription::valueChanged, checkVisibility);
                checkVisibility(bindedProperty->getValue());
            }
        }

        layout->setRowStretch(layout->rowCount(), 100);
        layout->setColumnStretch(2, 100);
        tab->setLayout(layout);
    }
}

PropertyTabWidget::~PropertyTabWidget()
{
    delete sorter;
}

QTabWidget *PropertyTabWidget::getTabWidget()
{
    return m_tabs;
}

void PropertyTabWidget::moveTab(QString categoryName, int index, bool focus)
{
    int from = -1;
    auto tabBar = m_tabs->tabBar();

    for (int i = 0; i<tabBar->count(); i++)
    {
        auto tabData = tabBar->tabData(i).toString();
        if (tabData.compare(categoryName, Qt::CaseInsensitive) == 0)
        {
            from = i;
            break;
        }
    }

    if (from >= 0 && from != index)
    {
        tabBar->moveTab(from, index);
    }

    if (from >= 0 && focus)
    {
        m_tabs->setCurrentIndex(from);
    }
}
