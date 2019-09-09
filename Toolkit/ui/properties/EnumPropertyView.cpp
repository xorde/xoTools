#include "EnumPropertyView.h"

#include <QCoreApplication>
#include <QListView>

EnumPropertyView::EnumPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    QMetaEnum enumeration = description->enumerator();

    box = new QComboBox(this);
    box->setView(new QListView());
    int comboIndex = 0;
    for (int i = 0, length = enumeration.keyCount(); i < length; i++)
    {
        const char* key = enumeration.key(i);
        int value = enumeration.value(i);
        box->addItem(QCoreApplication::translate(enumeration.name(), key), value);

        if (description->getValue().value<int>() == value) comboIndex = i;
    }
    box->setCurrentIndex(comboIndex);
    connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), [description](int index){ description->setValue(QVariant(description->enumerator().value(index))); });

    layout->addWidget(box, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *EnumPropertyView::getEditor()
{
    return box;
}

void EnumPropertyView::nullifyEditor()
{
    if (box)
    {
        disconnect(box, nullptr);
        layout->removeWidget(box);
        delete box;
        box = nullptr;
    }
}

void EnumPropertyView::metaValueChanged(QVariant value)
{
    QMetaEnum enumerator = description->enumerator();
    if (box)
    {
        for (int i = 0, length = enumerator.keyCount(); i < length; ++i)
        {
            if (value.value<int>() == enumerator.value(i))
            {
                if (box->currentIndex() != i)
                {
                    box->blockSignals(true);
                    box->setCurrentIndex(i);
                    box->blockSignals(false);
                    //break;
                }
            }
        }
    }

    setDefaultButtonChecked(value.value<int>() == enumerator.keyToValue(description->m_def.toLatin1().data()));
}
