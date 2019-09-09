#include "StringPropertyView.h"
#include <QDebug>

StringPropertyView::StringPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    if (!description) return;

    lineEdit = new QLineEdit();

    m_connections << connect(lineEdit, &QLineEdit::textChanged, this, [=]()
    {
        if (lineEdit)
        description->setValue(QVariant(lineEdit->text()));
    }, Qt::QueuedConnection);

    layout->addWidget(lineEdit, 0, 0);

    QString val = description->getValue().toString();
    lineEdit->setText(val);
    setDefaultButtonChecked(description->getValue().toString() == description->m_def);
}

QWidget *StringPropertyView::getEditor() { return lineEdit; }

void StringPropertyView::nullifyEditor()
{
    if (lineEdit)
    {
        m_connections.clear();
        layout->removeWidget(lineEdit);
        delete lineEdit;
        lineEdit = nullptr;
    }
}

void StringPropertyView::metaValueChanged(QVariant value)
{
    QString valStr = value.toString();
    if (lineEdit && lineEdit->text() != valStr)
    {
        lineEdit->blockSignals(true);
        lineEdit->setText(valStr);
        lineEdit->blockSignals(false);
    }
    setDefaultButtonChecked(valStr == description->m_def);
}
