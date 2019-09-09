#include "DoublePropertyView.h"
#include <QCoreApplication>

DoublePropertyView::DoublePropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    spinBox = new QDoubleSpinBox(this);
    spinBox->setMinimum((description->hasMin) ? description->m_min : INT_MIN);
    spinBox->setMaximum((description->hasMax) ? description->m_max : INT_MAX);
    if (description->hasStep) spinBox->setSingleStep(description->m_step);
    spinBox->setValue(description->getValue().toDouble());

    if (!description->suffix.isEmpty())
    spinBox->setSuffix(QCoreApplication::translate("Generic", description->suffix.toLatin1().data()));

    if (!description->prefix.isEmpty())
    spinBox->setPrefix(QCoreApplication::translate("Generic",description->prefix.toLatin1().data()));

    connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [description](double value){ description->setValue(QVariant(value)); });

    layout->addWidget(spinBox, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *DoublePropertyView::getEditor()
{
    return spinBox;
}

void DoublePropertyView::nullifyEditor()
{
    if (spinBox)
    {
        disconnect(spinBox, nullptr);
        layout->removeWidget(spinBox);
        delete spinBox;
        spinBox = nullptr;
    }
}

void DoublePropertyView::metaValueChanged(QVariant value)
{
    if (spinBox)
    {
        spinBox->setValue(value.toDouble());
    }
    setDefaultButtonChecked(value.toDouble() == description->m_def.toDouble());
}
