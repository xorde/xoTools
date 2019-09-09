#ifndef ENUMSTRINGLISTPROPERTYVIEW_H
#define ENUMSTRINGLISTPROPERTYVIEW_H

#include <QWidget>
#include <QComboBox>
#include "BasePropertyView.h"

class EnumStringListPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit EnumStringListPropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QComboBox* box = nullptr;
    bool m_intOrStringMode = false;
};

#endif // ENUMSTRINGLISTPROPERTYVIEW_H
