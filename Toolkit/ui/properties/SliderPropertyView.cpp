#include "SliderPropertyView.h"
#include <QCoreApplication>
#include <QDebug>

SliderPropertyView::SliderPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    widgetHolder = new QWidget();
    auto vbox = new QHBoxLayout(widgetHolder);

    slider = new QSlider(Qt::Horizontal);
    slider->setMinimum(int(description->m_min));
    slider->setMaximum(int(description->m_max));
    slider->setSingleStep(int(description->m_step));

    spinBox = new QSpinBox();
    spinBox->setMinimum(int(description->m_min));
    spinBox->setMaximum(int(description->m_max));
    spinBox->setSingleStep(int(description->m_step));

    if (!description->suffix.isEmpty())
        spinBox->setSuffix(QCoreApplication::translate("Generic", description->suffix.toLatin1().data()));

    if (!description->prefix.isEmpty())
        spinBox->setPrefix(QCoreApplication::translate("Generic", description->prefix.toLatin1().data()));

    vbox->addWidget(slider);
    vbox->addWidget(spinBox, 100, Qt::AlignLeft);

    layout->addWidget(widgetHolder, 0, 0, 1, 1, Qt::AlignLeft);

    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [description](int value){ description->setValue(value); });
    connect(slider, &QSlider::valueChanged, [description](int value) { description->setValue(value); });

    metaValueChanged(description->getValue());
}

QWidget *SliderPropertyView::getEditor()
{
    return widgetHolder;
}

void SliderPropertyView::nullifyEditor()
{
    if (widgetHolder)
    {
        disconnect(slider, nullptr);
        disconnect(spinBox, nullptr);
        disconnect(widgetHolder, nullptr);

        layout->removeWidget(widgetHolder);
        delete widgetHolder;

        widgetHolder = nullptr;
        slider = nullptr;
        spinBox = nullptr;
        slider = nullptr;
    }
}

void SliderPropertyView::metaValueChanged(QVariant value)
{
    if (slider && spinBox)
    {
        slider->blockSignals(true);
        spinBox->blockSignals(true);

        slider->setValue(value.toInt());
        spinBox->setValue(value.toInt());

        slider->blockSignals(false);
        spinBox->blockSignals(false);
    }

    setDefaultButtonChecked(value.toString() == description->m_def);
}
