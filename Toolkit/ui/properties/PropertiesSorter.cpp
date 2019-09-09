#include "PropertiesSorter.h"
#include "AbstractSettings.h"

PropertiesSorter::PropertiesSorter(AbstractMetaDescriptor* descriptor)
{
    parseProperties(descriptor);

    categories = propertiesByCategory.keys();

    sortCategories(categories);
    sortProperties(propertiesByCategory, categories);
}

void PropertiesSorter::parseProperties(AbstractMetaDescriptor *descriptor)
{
    for(int i = 0; i < descriptor->count(); i++)
    {
        AbstractMetaDescription *property = descriptor->at(i);
        if (!property->changeable || !property->visibleInUI) continue;

        propertiesByCategory[property->category].push_back(property);
        propertiesByName[property->name] = property;
    }
}

void PropertiesSorter::sortCategories(QList<QString> &categories)
{
    qSort(categories.begin(), categories.end(), [](QString a, QString b)->bool
    {
        if(a.isNull()) return false;
        if(b.isNull()) return true;

        return a < b;
    });
}

void PropertiesSorter::sortProperties(QHash<QString, QList<AbstractMetaDescription *> > &propertiesByCategory, QList<QString> &categories)
{
    for(const auto& category : categories)
    {
        QList<AbstractMetaDescription*> properties = propertiesByCategory[category];
        qSort(properties.begin(), properties.end(), [](AbstractMetaDescription* a, AbstractMetaDescription* b)->bool
        {
            if(a->priority == b->priority) return a->displayName < b->displayName;

            return a->priority < b->priority;
        });
    }
}
