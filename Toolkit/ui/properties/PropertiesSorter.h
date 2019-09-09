#ifndef PROPERTIESSORTER_H
#define PROPERTIESSORTER_H

#include "ui/properties/metadescriptor.h"

class PropertiesSorter
{
public:
    PropertiesSorter(AbstractMetaDescriptor *descriptor);

public:
    QHash<QString, QList<AbstractMetaDescription*>> propertiesByCategory;
    QHash<QString, AbstractMetaDescription*> propertiesByName;
    QList<QString> categories;
    QList<QString> categoriesOriginal;

private:
    void parseProperties(AbstractMetaDescriptor* descriptor);
    void sortCategories(QList<QString>& categories);
    void sortProperties(QHash<QString, QList<AbstractMetaDescription*>>& propertiesByCategory, QList<QString>& categories);
};

#endif // PROPERTIESSORTER_H
