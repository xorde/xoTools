#ifndef UIHELPER
#define UIHELPER

#include <QPushButton>
#include <QSvgWidget>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>

#include "NPushButton.h"
#include "xotools_global.h"

class XOTOOLSSHARED_EXPORT UIHelper
{
public:
    static QPushButton* GenerateSvgButton(QString imagePath, QSize imageSize = QSize(16,16), QSize buttonSize = QSize(24,24), QString style = "", QString newColor = "");
    static QPushButton* GenerateSvgButton(QString text, QString imagePath, QSize imageSize = QSize(16,16), QSize buttonSize = QSize(24,24), QString style = "", QString newColor = "");
    static NPushButton *GenerateSmartSvgButton(QString imagePath, QSize imageSize = QSize(16,16), QSize buttonSize = QSize(24,24), QString colorNormal = "#159", QString colorHover = "#37B", QString colorDisabled = "#999");
    static QSvgWidget* FixedSizeSvg(QString imagePath, QSize imageSize = QSize(16,16), QString newColor = "");
    static QPixmap SvgToPixmap(QString svgPath, QSize size);

    static QIcon IconFromSvg(QString svgPath, QString newColor = "", QSize sizeOfIcon = QSize());
    static QPixmap PixmapFromSvg(QString svgPath, QString newColor = "", QSize sizeOfIcon = QSize());

    static void ChangeBackground(QWidget *w, QColor color);
    static void ChangeBorders(QFrame *w, int width, QColor color = Qt::black);
    static void InsertToWidget(QWidget *parent, QWidget *child, int margin = 0, Qt::Alignment alignment = Qt::Alignment());
    static void SetGridMargins(QGridLayout *grid, int contentsMargins = 0, int verticalSpacing = 0, int horizontalSpacing = 0);
    static QLabel *GenerateLabelWithName(QString text, QString objectName, QWidget* parent =0);
    static QLabel *GenerateLabel(QString text, int fontSize = -1, QColor foreground = Qt::black, bool bold = false, QString family = "");
    static QLabel *GenerateBoldLabel(QString text,int fontSize = -1, QColor foreground = Qt::black, QString family = "");
    static void ClearLayout(QLayout *layout, bool deleteLayout = false);
    static QPushButton* GenerateLinkButton(QString link, QString alternateText = "", QString tooltip = "", QString objectName = "link");
    static void Delete(QLayoutItem* item);
    static void Delete(QWidget* widget);

};

#endif // UIHELPER

