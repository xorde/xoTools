#include "uihelper.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QSvgRenderer>

QPushButton* UIHelper::GenerateSvgButton(QString imagePath, QSize imageSize, QSize buttonSize, QString style, QString newColor)
{
    QPushButton *btn = new QPushButton();
    btn->setFixedSize(buttonSize);
    QGridLayout *lay = new QGridLayout(btn);
    lay->addWidget(UIHelper::FixedSizeSvg(imagePath, imageSize, newColor));
    lay->setContentsMargins(1,1,1,1);

    if (!style.isEmpty())
        btn->setStyleSheet(style);

    return btn;
}

QPushButton* UIHelper::GenerateSvgButton(QString text, QString imagePath, QSize imageSize, QSize buttonSize, QString style, QString newColor)
{
    QPushButton *btn = new QPushButton();    
    QLabel* pLabel = new QLabel(text);

    btn->setFixedSize(buttonSize);
    btn->setFlat(true);
    QHBoxLayout *lay = new QHBoxLayout(btn);
    lay->addWidget(UIHelper::FixedSizeSvg(imagePath, imageSize, newColor));
    lay->setSpacing(0);
    lay->addWidget(pLabel);
    lay->setContentsMargins(1,1,1,1);

    if (!style.isEmpty())
        btn->setStyleSheet(style);

    return btn;
}

NPushButton *UIHelper::GenerateSmartSvgButton(QString imagePath, QSize imageSize, QSize buttonSize, QString colorNormal, QString colorHover, QString colorDisabled)
{
    NPushButton *btn = new NPushButton();
    btn->setFixedSize(buttonSize);
    btn->setContentsMargins(0,0,0,0);
    QGridLayout *lay = new QGridLayout(btn);
    lay->setContentsMargins(0,0,0,0);

    auto iconNormal = UIHelper::FixedSizeSvg(imagePath, imageSize, colorNormal);
    auto iconHover = UIHelper::FixedSizeSvg(imagePath, imageSize, colorHover);
    auto iconDisabled = UIHelper::FixedSizeSvg(imagePath, imageSize, colorDisabled);

    lay->addWidget(iconNormal, 0,0);
    lay->addWidget(iconHover, 0,0);
    lay->addWidget(iconDisabled, 0,0);

    auto func_btnViewFunc = [=]()
    {
        bool enabled = btn->isEnabled();
        bool hovered = btn->isHovered();
        bool showNormal = false, showHover = false, showDisabled = false;
        if (enabled)
        {
            if (hovered)
            {
                showHover = true;
            }
            else
            {
                showNormal = true;
            }
        }
        else
        {
            showDisabled = true;
        }

        iconNormal->setVisible(showNormal);
        iconHover->setVisible(showHover);
        iconDisabled->setVisible(showDisabled);
    };

    QObject::connect(btn, &NPushButton::mouseHoverChanged, func_btnViewFunc);
    QObject::connect(btn, &NPushButton::enabledStateChanged, func_btnViewFunc);

    func_btnViewFunc();

    return btn;
}

QString getColoredSvgContent(QString &imagePath, QString newColor)
{
    QFile svgFile(imagePath);

    if (svgFile.open(QIODevice::ReadOnly))
    {
        QString svgContent = svgFile.readAll();

        if (!newColor.isEmpty())
        {
            int startSvgFill = svgContent.indexOf("svg fill=");
            if (startSvgFill > 0)
            {
                startSvgFill += 10;
                int endSvgFill = svgContent.indexOf("\"", startSvgFill);

                //qDebug() << svgContent.mid(startSvgFill, endSvgFill-startSvgFill);

                svgContent.replace(startSvgFill, endSvgFill-startSvgFill, newColor);
            }
        }

        //qDebug() << svgContent;
        return svgContent;
    }
    return "";
}

#include <QFile>

QSvgWidget *UIHelper::FixedSizeSvg(QString imagePath, QSize imageSize, QString newColor)
{
    QSvgWidget *icon = new QSvgWidget();

    if (!newColor.isEmpty())
    {
        QString svgContent = getColoredSvgContent(imagePath, newColor);
        if (!svgContent.isEmpty())
            icon->load(svgContent.toUtf8());
    }
    else
    {
        icon->load(imagePath);
    }

    if (imageSize.width() > 0 && imageSize.height() > 0)
    {
        icon->setFixedSize(imageSize);
    }
    return icon;
}

QPixmap UIHelper::SvgToPixmap(QString svgPath, QSize size)
{
    QIcon pix(svgPath);

    return pix.pixmap(size.width(), size.height());
}

#include <QPainter>
QIcon UIHelper::IconFromSvg(QString svgPath, QString newColor, QSize sizeOfIcon)
{
    QIcon icon;
    icon.addPixmap(PixmapFromSvg(svgPath, newColor, sizeOfIcon));
    return icon;
}

QPixmap UIHelper::PixmapFromSvg(QString svgPath, QString newColor, QSize sizeOfIcon)
{
    QString svgContent = getColoredSvgContent(svgPath, newColor);

    QSvgRenderer renderer(svgContent.toUtf8());

    QSize size = renderer.defaultSize();

    if (sizeOfIcon.width() > 0 && sizeOfIcon.height() > 0)
    {
        size = sizeOfIcon;
    }

    QPixmap pm(size.width(), size.height());
    pm.fill(Qt::transparent);
    QPainter painter(&pm);
    renderer.render(&painter, pm.rect());

    return pm;
}

void UIHelper::ChangeBackground(QWidget *w, QColor color)
{
    QPalette pal(w->palette());
    pal.setColor(QPalette::Background, color);
    w->setAutoFillBackground(true);
    w->setPalette(pal);
}

QLabel* UIHelper::GenerateLabel(QString text, int fontSize, QColor foreground, bool bold, QString family)
{
    QLabel *l = new QLabel(text);

    QString style = "QLabel { ";

    if (fontSize > 0) style += "font-size: " + QString::number(fontSize) + "px; ";
    style += QString("color: ") + foreground.name(QColor::HexRgb) + "; ";
    if (bold) style += "font-weight: bold";
    if (family.length() > 0) style += "font-family: " + family + "; ";

    style += "}";
    l->setStyleSheet(style);
    return l;
}

QLabel *UIHelper::GenerateBoldLabel(QString text, int fontSize, QColor foreground, QString family)
{
    return GenerateLabel(text, fontSize, foreground, true, family);
}

void UIHelper::ClearLayout(QLayout *layout, bool deleteLayout)
{
    if (!layout)
        return;

    QLayoutItem* item;
    while (item = layout->takeAt(0))
    {
        Delete(item);
    }
    if (deleteLayout)
    {
        delete layout;
        layout = nullptr;
    }
}

QPushButton *UIHelper::GenerateLinkButton(QString link, QString alternateText, QString tooltip, QString objectName)
{
    QPushButton* btn = new QPushButton(alternateText.length() > 0 ? alternateText : link);
    btn->connect(btn, &QPushButton::clicked, [=]() { QDesktopServices::openUrl(QUrl(link)); });
    if (objectName.length() > 0) btn->setObjectName(objectName);
    btn->setToolTip(tooltip.length() > 0 ? tooltip : link);
    return btn;
}

void UIHelper::Delete(QLayoutItem *item)
{
    if (item)
    {
        if (item->layout()) ClearLayout(item->layout(), true);

        if (item->widget())
            delete item->widget();

        delete item;
    }
}

void UIHelper::Delete(QWidget *widget)
{
    if (widget)
    {
        if (widget->layout()) ClearLayout(widget->layout(), true);
        delete widget;
    }
}

void UIHelper::ChangeBorders(QFrame *w, int width, QColor color)
{
    QPalette pal(w->palette());
    pal.setColor(QPalette::WindowText, color);
    w->setFrameStyle(QFrame::Box|QFrame::Plain);
    w->setLineWidth(width);
}

void UIHelper::InsertToWidget(QWidget *parent, QWidget *child, int margin, Qt::Alignment alignment)
{
    QGridLayout *g = new QGridLayout();
    parent->setLayout(g);
    g->addWidget(child, 0,0, 1, 1, alignment);
    UIHelper::SetGridMargins(g, margin, 0, 0);
}

void UIHelper::SetGridMargins(QGridLayout *grid, int contentsMargins, int verticalSpacing, int horizontalSpacing)
{
    grid->setContentsMargins(contentsMargins, contentsMargins, contentsMargins, contentsMargins);
    grid->setVerticalSpacing(verticalSpacing);
    grid->setHorizontalSpacing(horizontalSpacing);
}

QLabel *UIHelper::GenerateLabelWithName(QString text, QString objectName, QWidget *parent)
{
    QLabel* lbl = new QLabel(text, parent);
    lbl->setObjectName(objectName);
    return lbl;
}
