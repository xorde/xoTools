#include "MultilinePropertyView.h"

MultilinePropertyView::MultilinePropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    txt = new QPlainTextEdit();

    connect(txt, &QPlainTextEdit::textChanged, this, [description, this]()
    {
        description->setValue(QVariant(txt->toPlainText()));
    }, Qt::QueuedConnection);

    layout->addWidget(txt, 0, 0);
    metaValueChanged(description->getValue());
}

QWidget *MultilinePropertyView::getEditor()
{
    return txt;
}

void MultilinePropertyView::nullifyEditor()
{
    if (txt)
    {
        disconnect(txt, nullptr);
        layout->removeWidget(txt);
        delete txt;
        txt = nullptr;
    }
}

void MultilinePropertyView::metaValueChanged(QVariant value)
{

    QString textValue = value.toString();

    if (txt && txt->toPlainText() != textValue)
    {        
        txt->blockSignals(true);
        txt->setPlainText(value.toString());
        txt->blockSignals(false);
    }

}
