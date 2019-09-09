#include "SpecialPropertyView.h"
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QFileDialog>

SpecialPropertyView::SpecialPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    button = new QPushButton(QIcon(":/icons/folder.svg"), QString());
    button->setMaximumSize(30, 30);
    connect(button, &QPushButton::clicked, [this, description](){
        if(description->pickOption == "PickFolder")
        {
            QString folder = QFileDialog::getExistingDirectory(this, tr("Open"), QString(), QFileDialog::ShowDirsOnly);
            if (!folder.isEmpty())
            {
                description->setValue(folder);
            }
        }
        else
        {
            if(description->pickExtensions.isEmpty() || description->pickExtensions == "*.*")
                description->setValue(QVariant(QFileDialog::getOpenFileName(this, tr("Open"), QString(), tr("Любой файл") + "(*.*)")));
            else
                description->setValue(QVariant(QFileDialog::getOpenFileName(this, tr("Open"), QString(), "("+ description->pickExtensions + ")")));
        }
    });

    lineEdit = new QLineEdit();
    connect(lineEdit, &QLineEdit::textChanged, [description](QString value) { description->setValue(QVariant(value)); });

    auto subLayout = new QHBoxLayout();
    subLayout->addWidget(button);
    subLayout->addWidget(lineEdit);

    layout->addLayout(subLayout, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *SpecialPropertyView::getEditor()
{
    return lineEdit;
}

void SpecialPropertyView::nullifyEditor()
{
    if (lineEdit)
    {
        disconnect(lineEdit, nullptr);
        layout->removeWidget(lineEdit);
        delete lineEdit;
        lineEdit = nullptr;
    }
}

void SpecialPropertyView::metaValueChanged(QVariant value)
{
    if (lineEdit)
    {
        lineEdit->setText(value.toString());
    }

    setDefaultButtonChecked(value.toString() == description->m_def);
}
