#include "BoolPropertyView.h"


BoolPropertyView::BoolPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    box = new QCheckBox(this);

    connect(box, &QCheckBox::stateChanged, [description](int value)
    {
        description->setValue(QVariant(value > 0));
    });

    layout->addWidget(box, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *BoolPropertyView::getEditor()
{
    return box;
}

void BoolPropertyView::nullifyEditor()
{
    if (box)
    {
        disconnect(box, nullptr);
        layout->removeWidget(box);
        delete box;
        box = nullptr;
    }
}

void BoolPropertyView::metaValueChanged(QVariant value)
{
    if (box)
    {
        box->setChecked(value.toBool());
    }

    setDefaultButtonChecked(value.toString() == description->m_def);
}
