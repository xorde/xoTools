#include "nstackpanel.h"
#include <QGridLayout>
#include "uihelper.h"

NStackPanel::NStackPanel(Qt::Orientation orientation, QWidget *parent) : QWidget(parent)
{
   mOrientation = orientation;
   clear();
   UIHelper::ChangeBackground(this, Qt::transparent);

}

NStackPanel::~NStackPanel()
{
    UIHelper::ClearLayout(grid);
}

void NStackPanel::add(QWidget *w)
{
    if (w == nullptr) return;

    int row = grid->rowCount();
    int col = grid->columnCount();

    Qt::Alignment align = Qt::AlignLeft;
    if (mOrientation == Qt::Horizontal)
    {
        row = 0;
    }
    else
    {
        align = Qt::AlignTop;
        col = 0;
    }

    if (spacer)
    {
        grid->removeWidget(spacer);
        delete spacer;
        spacer = nullptr;
    }

    grid->addWidget(w, row, col, 1, 1, align);

    if (mOrientation == Qt::Horizontal)
    {
        for (int i = 0; i<grid->columnCount(); i++)        
            grid->setColumnStretch(i, 0);       
    }
    else
    {
        for (int i = 0; i<grid->rowCount(); i++)        
            grid->setRowStretch(i, 0);        
    }

    spacer = new QWidget();
    grid->addWidget(spacer, row + (mOrientation == Qt::Horizontal ? 0 : 1), col + (mOrientation == Qt::Horizontal ? 1 : 0), 1, 1);


    if (mOrientation == Qt::Horizontal)
        grid->setColumnStretch(grid->columnCount()-1, 100);    
    else    
        grid->setRowStretch(grid->rowCount()-1, 100);    
}

void NStackPanel::setPadding(int padding)
{
    setPadding(padding, padding, padding, padding);
}

void NStackPanel::clear()
{
    if (grid)
    {
        UIHelper::ClearLayout(grid);
        delete grid;
        spacer = nullptr;
    }
    grid = new QGridLayout();
    setLayout(grid);

    grid->setContentsMargins(m_padding);
    grid->setVerticalSpacing(0);
    grid->setHorizontalSpacing(0);
}

QWidget *NStackPanel::at(int index)
{
    if (index >= 0 && index < count())           
        return grid->itemAt(index)->widget();

    return nullptr;
}

void NStackPanel::setPadding(int left, int top, int right, int bottom)
{
    m_padding = QMargins(left, top, right, bottom);
    grid->setContentsMargins(m_padding);
}

void NStackPanel::setItemPadding(int margin)
{
    setItemPadding(margin, margin, margin, margin);
}

void NStackPanel::setItemPadding(int left, int top, int right, int bottom)
{
    //TODO: Not really working setItemPadding
    QLayoutItem* item;
    while ( ( item = grid->takeAt( 0 ) ) != nullptr )
    {
       item->widget()->setContentsMargins(left, top, right, bottom);
    }
}

void NStackPanel::setItemAlignment(QWidget *w, Qt::Alignment align)
{
    if (w) grid->setAlignment(w, align);
}

void NStackPanel::setItemAlignmentByIndex(int index, Qt::Alignment align)
{
    if (index >= 0 && index < count())
    {
        setItemAlignment(at(index), align);
    }
}

NStackPanel *NStackPanel::operator <<(QWidget *w)
{
    add(w);
    return this;
}
