#ifndef DOUBLEPROPERTYVIEW_H
#define DOUBLEPROPERTYVIEW_H

#include <QDoubleSpinBox>

#include "BasePropertyView.h"

class DoublePropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit DoublePropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

private:
    QDoubleSpinBox* spinBox = nullptr;

protected slots:
    void metaValueChanged(QVariant value) override;
};

#endif // DOUBLEPROPERTYVIEW_H
