#include "AbstractMetaDescription.h"

AbstractMetaDescription::AbstractMetaDescription(QObject *parent) : QObject(parent)
{
}

void AbstractMetaDescription::setIsReadOnly(bool isReadOnly)
{
    this->isReadOnly = isReadOnly;
}
