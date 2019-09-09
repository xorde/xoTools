#ifndef ENUMPROPERTYVIEW_H
#define ENUMPROPERTYVIEW_H

#include <QWidget>
#include <QComboBox>

#include "BasePropertyView.h"

class EnumPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit EnumPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QComboBox* box = nullptr;

};

#endif // ENUMPROPERTYVIEW_H
