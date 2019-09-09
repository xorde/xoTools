#ifndef COLORPROPERTYVIEW_H
#define COLORPROPERTYVIEW_H

#include "BasePropertyView.h"
#include <QLabel>
#include <QColorDialog>

class ColorPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit ColorPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;

    void nullifyEditor() override;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPushButton* button = nullptr;
    QLabel* label = nullptr;
    QColorDialog* dialog = nullptr;

protected slots:
    void metaValueChanged(QVariant value) override;
};

#endif // COLORPROPERTYVIEW_H
