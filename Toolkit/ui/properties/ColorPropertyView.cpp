#include "ColorPropertyView.h"
#include <QPainter>
#include <QDebug>

ColorPropertyView::ColorPropertyView(AbstractMetaDescription *description, QWidget *parent) : BasePropertyView(description, parent)
{
    button = new QPushButton(this);
    button->setFixedSize(20, 20);
    button->setAutoFillBackground(true);

    connect(button, &QPushButton::clicked, [this, description](){

        dialog = new QColorDialog(this);
        dialog->show();

        connect(dialog, &QColorDialog::colorSelected, [this, description](QColor color){ description->setValue(QVariant(color)); metaValueChanged(QVariant(color)); });

        if(description->specialType.contains("Alpha")) dialog->setOption(QColorDialog::ShowAlphaChannel);
        dialog->setCurrentColor(description->getValue().value<QColor>());

    });

    label = new QLabel(this);

    auto subLayout = new QGridLayout();
    subLayout->addWidget(button, 0, 0);
    subLayout->addWidget(label, 0, 1);

    layout->addLayout(subLayout, 0, 0);

    metaValueChanged(description->getValue());
}

QWidget *ColorPropertyView::getEditor()
{
    return button;
}

void ColorPropertyView::nullifyEditor()
{
    if (button)
    {
        disconnect(button, nullptr);
        layout->removeWidget(button);
        delete button;
        button = nullptr;
    }
}

void ColorPropertyView::paintEvent(QPaintEvent *)
{
    if (!button) return;

    auto origin = button->pos();
    int x = origin.x();
    int y = origin.y();
    int width = button->width();
    int height = button->height();
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    QPainter painter;
    painter.begin(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::white);
    painter.drawRect(x, y, width, height);
    painter.setBrush(QColor("#CCC"));
    painter.drawRect(x, y, halfWidth, halfHeight);
    painter.drawRect(x + halfWidth, y + halfHeight, halfWidth, halfHeight);
    painter.end();
}

void ColorPropertyView::metaValueChanged(QVariant value)
{
    if (button)
    {
        button->update();
        button->setStyleSheet("border: 1px solid #000000; background-color:" + value.toString() + ";");
    }
    if (label)
    {
        label->setText(value.toString().toUpper());
    }
    setDefaultButtonChecked(value.value<QColor>() == QColor(description->m_def));
}
