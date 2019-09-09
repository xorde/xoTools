#ifndef STRINGPROPERTYVIEW_H
#define STRINGPROPERTYVIEW_H

#include <QWidget>
#include <QLineEdit>

#include "BasePropertyView.h"

class StringPropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit StringPropertyView(AbstractMetaDescription* description, QWidget *parent = nullptr);

    QWidget *getEditor() override;
    void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

private:
    QLineEdit* lineEdit = nullptr;
    bool canChange = true;
};

#endif // STRINGPROPERTYVIEW_H
