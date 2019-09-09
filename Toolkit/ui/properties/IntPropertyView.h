#ifndef INTPROPERTYVIEW_H
#define INTPROPERTYVIEW_H

#include <QSpinBox>

#include "BasePropertyView.h"

class IntPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit IntPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QSpinBox* spinBox = nullptr;
};

#endif // INTPROPERTYVIEW_H
