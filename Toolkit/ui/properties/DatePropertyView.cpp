#include "DatePropertyView.h"

DatePropertyView::DatePropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    box = new QDateEdit();
    box->setDate(description->getValue().toDate());

    box->setCalendarPopup(true);

    connect(box, &QDateEdit::dateChanged, [=](QDate date)
    {
        box->blockSignals(true);
        description->setValue(QVariant(date));
        box->blockSignals(false);
    });

    layout->addWidget(box, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *DatePropertyView::getEditor()
{
    return box;
}

void DatePropertyView::nullifyEditor()
{
    if (box)
    {
        disconnect(box, nullptr);
        layout->removeWidget(box);
        delete box;
        box = nullptr;
    }
}

void DatePropertyView::metaValueChanged(QVariant value)
{
    if (box)
    {
        box->blockSignals(true);
        box->setDate(value.toDate());
        box->blockSignals(false);
    }
}
