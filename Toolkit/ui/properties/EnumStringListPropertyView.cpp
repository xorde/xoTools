#include "EnumStringListPropertyView.h"
#include <QCoreApplication>
#include <QListView>

EnumStringListPropertyView::EnumStringListPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    auto list = description->m_enumStringList;

    m_intOrStringMode = !(description->typeName == "QString");

    box = new QComboBox(this);
    box->setView(new QListView());
    bool ok;

    int comboIndex = -1;

    if (m_intOrStringMode)
    {
        comboIndex = description->getValue().toInt(&ok);
        if (!ok || list.count() < comboIndex || comboIndex < -1)
        {
            comboIndex = -1;
        }
    }
    else
    {
        comboIndex = list.indexOf(description->getValue().toString());
    }

    for (int i = 0, length = list.count(); i < length; i++)
    {
        box->addItem(QCoreApplication::translate("Generic", qPrintable(list.at(i))), i);
    }
    box->setCurrentIndex(comboIndex);


    if (m_intOrStringMode)
    {
        connect(box, QOverload<int>::of(&QComboBox::currentIndexChanged), [description](int index)
        {

            description->setValue(index);
        });
    }
    else
    {
        connect(box, &QComboBox::currentTextChanged, [description](const QString &text)
        {
            description->setValue(text);
        });
    }

    layout->addWidget(box, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *EnumStringListPropertyView::getEditor()
{
    return box;
}

void EnumStringListPropertyView::nullifyEditor()
{
    if (box)
    {
        disconnect(box, nullptr);
        layout->removeWidget(box);
        delete box;
        box = nullptr;
    }
}

void EnumStringListPropertyView::metaValueChanged(QVariant value)
{
    if (m_intOrStringMode)
    {
        int index = value.toInt();
        if (box)
        {
            box->blockSignals(true);
            if (box->count() > index && index >= -1)
            {
                box->setCurrentIndex(index);
            }
            box->blockSignals(false);
        }

        if (description->hasDefault)
        {
            setDefaultButtonChecked(value.value<int>() == description->m_def.toInt());
        }
    }
    else
    {
        QString val = value.toString();
        if (box)
        {
            box->blockSignals(true);
            box->setCurrentText(val);
            box->blockSignals(false);
        }

        if (description->hasDefault)
        {
            setDefaultButtonChecked(value.value<int>() == description->m_def.toInt());
        }
    }
}
