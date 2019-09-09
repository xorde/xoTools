#include "nlistbox.h"

//TODO: deselect option on free field
//TODO: System defined background

NListBox::NListBox(QWidget *parent) : ContextableWidget(parent)
{
    QWidget *container = new QWidget();
    QGridLayout *g = new QGridLayout(this);

    m_scroll = new QScrollArea(this);
    m_scroll->setWidgetResizable(true);
    m_scroll->setWidget(container);

    m_uiLayout = new QGridLayout(container);

    g->setContentsMargins(0,0,0,0);
    m_uiLayout->setContentsMargins(0,0,0,0);
    m_uiLayout->setVerticalSpacing(0);
    m_uiLayout->setHorizontalSpacing(0);
    container->setLayout(m_uiLayout);

    g->addWidget(m_scroll, 0,0);

    setItems(new ObservableCollection());

    UIHelper::ChangeBackground(m_scroll, Qt::white);
    m_selectedIndex = -1;

    _uiEmptyListLabel = new QLabel();
    _uiEmptyListLabel->setAlignment(Qt::AlignCenter);
    _uiEmptyListLabel->setWordWrap(true);
    _uiEmptyListLabel->setContentsMargins(10,10,10,10);
    _uiEmptyListLabel->setStyleSheet("QLabel { color : #999; }");
    g->addWidget(_uiEmptyListLabel, 0, 0, Qt::AlignCenter);

    connect(m_items, SIGNAL(countChanged(int)), SLOT(onItemsCountChanged(int)), Qt::QueuedConnection);

    onItemsCountChanged(0);
}

void NListBox::onItemsCountChanged(int count)
{
    _uiEmptyListLabel->setVisible(count <= 0);
}

void NListBox::onContextChanged()
{
    auto coll = dynamic_cast<ObservableCollection*>(getContext());
    if (coll != 0)
    {
        setSelectedIndex(-1);
        setItems(coll);
    }
}

void NListBox::clear()
{
    m_items->clear();
}

int NListBox::count()
{
    return m_items->count();
}

QObject *NListBox::at(int index)
{
    if (index < 0 || index >= count())
        return nullptr;

    return m_items->at(index);
}

void NListBox::setEmptyListText(QString text)
{
    if (_uiEmptyListLabel)    
        _uiEmptyListLabel->setText(text);    
}

void NListBox::setOrientation(Qt::Orientation value)
{
    if (value == m_orientation)    
        return;   

    m_orientation = value;
    emit changedOrientation(value);
    rebuild();
}

Qt::Orientation NListBox::getOrientation()
{
    return m_orientation;
}

void NListBox::setSelectedIndex(int value)
{
    if (value < 0 && m_items->count() <= value)    
        value = -1;    

    if (value == m_selectedIndex)
        return;

    m_selectedIndex = value;
    emit changedSelectedIndex(value);
    emit changedSelectedItem(getSelectedItem());
    emit anythingSelected(m_selectedIndex >= 0);

    //Updating visuals
    for(int i =0; i<m_widgets.count(); i++)
    {
        ClickableWidget *w = m_widgets.at(i);
        if (i != m_selectedIndex)        
            w->unhighlight();        
        else        
            w->highlight();        
    }
}

int NListBox::getSelectedIndex()
{
    return m_selectedIndex;
}

void NListBox::setSelectedItem(QObject *value)
{
    int index = m_items->indexOf(value);
    if (index >= 0)    
        setSelectedIndex(index);    
}

QObject *NListBox::getSelectedItem()
{
    if (m_selectedIndex >= 0 && m_items->count() > m_selectedIndex)    
        return m_items->at(m_selectedIndex);

    return nullptr;
}

void NListBox::setItems(ObservableCollection *value)
{
    if (m_items)    
        disconnect(m_items, SIGNAL(countChanged(int)), this, SLOT(rebuild()));    

    m_items = value;
    emit itemsChanged(m_items);

    if (m_items)    
        connect(m_items, SIGNAL(countChanged(int)), this, SLOT(rebuild()));    

    rebuild();
}

void NListBox::itemClicked(int clicks)
{
    foreach(ClickableWidget* cl, m_widgets)
    {
        cl->unhighlight();
    }

    ClickableWidget* clicker = dynamic_cast<ClickableWidget*>(QObject::sender());
    clicker->highlight();

    int index = m_widgets.indexOf(clicker);

    if (index >= 0)
    {
        setSelectedIndex(index);
        if (clicks > 1)        
            selectedItemDoubleClicked(getSelectedItem());        
    }
}

void NListBox::rebuild()
{
    emit internalRebuildStart();
    m_selectedIndex = -1;
    foreach(ClickableWidget* w, m_widgets)
    {
        m_uiLayout->removeWidget(w);
        delete w;
    }
    bool vertical = m_orientation == Qt::Vertical;

    if (m_uiSpacer)
    {
        m_uiLayout->removeWidget(m_uiSpacer);
        delete m_uiSpacer;
        m_uiSpacer = nullptr;
    }
    m_widgets.clear();
    if (m_items)
    {
        int count = m_items->count();
        for (int i = 0; i<count; i++)
        {
            QObject* obj = m_items->at(i);

            if (!obj) continue;

            QWidget* w = createItemWidget(obj);

            ClickableWidget *k = new ClickableWidget();

            UIHelper::InsertToWidget(k, w, 1);

            connect(k, SIGNAL(clicked(int)), this, SLOT(itemClicked(int)));

            m_widgets << k;

            if (vertical)
            {
                m_uiLayout->addWidget(k, i, 0);
                m_uiLayout->setRowStretch(i, 0);
            }
            else
            {
                m_uiLayout->addWidget(k, 0, i);
                m_uiLayout->setColumnStretch(i, 0);
            }
        }

        m_uiSpacer = new QWidget();
        m_uiLayout->addWidget(m_uiSpacer, vertical ? count : 0, vertical ? 0 : count);
        if (vertical)
            m_uiLayout->setRowStretch(count, 100);
        else
            m_uiLayout->setColumnStretch(count, 100);

    }
    emit internalRebuildEnd();
    emit changedSelectedIndex(m_selectedIndex);
}

void NListBox::setTransparentBackground()
{
    setBackground(Qt::transparent);
}

void NListBox::setBackground(QColor color)
{
    setBackgroundAndBorder(color, 0, Qt::black);
}

void NListBox::setBackgroundAndBorder(QColor backgroundColor, quint8 borderThickness, QColor borderColor)
{
    UIHelper::ChangeBackground(m_scroll, backgroundColor);
    QString colorName = backgroundColor.name();
    if (backgroundColor == Qt::transparent) colorName = "transparent";

    QString borderStyle = "";

    if (borderThickness == 0) borderStyle = "0";
    else borderStyle = QString::number(borderThickness) +"px solid "+borderColor.name() + "";

    m_scroll->setStyleSheet("QScrollArea { background: "+colorName+"; border: "+borderStyle+"; } QScrollArea > QWidget > QWidget { background: "+colorName+"; } \
                            QScrollArea > QWidget > QScrollBar { background: palette(base); }");
}
