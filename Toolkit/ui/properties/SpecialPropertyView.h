#ifndef SPECIALPROPERTYVIEW_H
#define SPECIALPROPERTYVIEW_H

#include "BasePropertyView.h"
#include <QLineEdit>

class SpecialPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit SpecialPropertyView(AbstractMetaDescription* description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QPushButton* button = nullptr;
    QLineEdit* lineEdit = nullptr;
};

#endif // SPECIALPROPERTYVIEW_H
