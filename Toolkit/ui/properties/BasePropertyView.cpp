#include "BasePropertyView.h"

BasePropertyView::BasePropertyView(AbstractMetaDescription *description, QWidget *parent) : QWidget(parent)
{
    this->description = description;

    m_connections << connect(description, SIGNAL(valueChanged(QVariant)), this, SLOT(metaValueChanged(QVariant)), Qt::QueuedConnection);

    layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);
    layout->setColumnStretch(0, 100);

    if (description->hasDefault && description->showDefault)
    {
         defaultButton = new QPushButton(this);
         defaultButton->setFixedSize(10, 10);
         defaultButton->setFocusPolicy(Qt::ClickFocus);

         layout->addWidget(defaultButton, 0, 1, Qt::AlignLeft);
         connect(defaultButton, &QPushButton::clicked, [description](){ description->setDefault(); });
    }
}

void BasePropertyView::showDefaultButton(bool show)
{
     if(defaultButton == nullptr) return;

     defaultButton->setVisible(show);
}

QGridLayout *BasePropertyView::getEditingLayout() { return layout; }

void BasePropertyView::setDefaultButtonChecked(bool value)
{
    if(defaultButton == nullptr) return;

    defaultButton->setStyleSheet( value
                                  ? "QPushButton { background-color: #999; border-width: 0px; border-style: outset; } QPushButton:hover { background-color: #555; }"
                                  : "QPushButton { background-color: #39F; border-width: 0px; border-style: outset; } QPushButton:hover { background-color: #39F; }");
}
