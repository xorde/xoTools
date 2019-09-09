#ifndef DATEPROPERTYVIEW_H
#define DATEPROPERTYVIEW_H

#include <QWidget>
#include "BasePropertyView.h"
#include <QDateEdit>
#include <QCalendarWidget>

class DatePropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit DatePropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;

    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QDateEdit* box = nullptr;
};

#endif // DATEPROPERTYVIEW_H
