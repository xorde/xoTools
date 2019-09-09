#ifndef BOOLPROPERTYVIEW_H
#define BOOLPROPERTYVIEW_H

#include <QCheckBox>
#include "BasePropertyView.h"

class BoolPropertyView : public BasePropertyView
{
public:
    BoolPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;

    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QCheckBox* box = nullptr;
};

#endif // BOOLPROPERTYVIEW_H
