#ifndef SLIDERPROPERTYVIEW_H
#define SLIDERPROPERTYVIEW_H

#include <QSlider>
#include <QSpinBox>
#include "BasePropertyView.h"

class SliderPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit SliderPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;

    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QWidget *widgetHolder = nullptr;
    QSpinBox* spinBox = nullptr;
    QSlider* slider = nullptr;
};

#endif // SLIDERPROPERTYVIEW_H
