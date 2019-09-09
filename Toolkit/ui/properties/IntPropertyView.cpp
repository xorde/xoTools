#include "IntPropertyView.h"
#include <QCoreApplication>

IntPropertyView::IntPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    spinBox = new QSpinBox();
    spinBox->setMinimum(description->hasMin ? (int)description->m_min : INT_MIN);
    spinBox->setMaximum(description->hasMax ? (int)description->m_max : INT_MAX);
    if (description->hasStep) spinBox->setSingleStep(description->m_step);
    spinBox->setValue(description->getValue().toInt());

    if (!description->suffix.isEmpty())
    spinBox->setSuffix(QCoreApplication::translate("Generic", description->suffix.toLatin1().data()));

    if (!description->prefix.isEmpty())
    spinBox->setPrefix(QCoreApplication::translate("Generic",description->prefix.toLatin1().data()));

    connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [description](int value){ description->setValue(QVariant(value)); });

    layout->addWidget(spinBox, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *IntPropertyView::getEditor()
{
    return spinBox;
}

void IntPropertyView::nullifyEditor()
{
    if (spinBox)
    {
        disconnect(spinBox, nullptr);
        layout->removeWidget(spinBox);
        delete spinBox;
        spinBox = nullptr;
    }
}

void IntPropertyView::metaValueChanged(QVariant value)
{
    if (spinBox)
    {
        spinBox->setValue(value.toInt());
    }

    setDefaultButtonChecked(value.toInt() == description->m_def.toInt());
}
