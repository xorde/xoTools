#ifndef BASEPROPERTYVIEW_H
#define BASEPROPERTYVIEW_H

#include <QWidget>
#include <QVariant>
#include <QGridLayout>
#include <QPushButton>

#include "metadescription.h"
#include "helpers/ConnectionHelper.h"

class  BasePropertyView : public QWidget
{
    Q_OBJECT
public:
    explicit BasePropertyView(AbstractMetaDescription* description, QWidget *parent = nullptr);

    void showDefaultButton(bool show);

    virtual QWidget* getEditor() { return nullptr; }

    virtual void nullifyEditor() {  }

    QGridLayout *getEditingLayout();

protected:
    QGridLayout* layout = nullptr;
    AbstractMetaDescription* description = nullptr;
    QPushButton *defaultButton = nullptr;
    ConnectionHelper m_connections;
protected:
    void setDefaultButtonChecked(bool value);

protected slots:
    virtual void metaValueChanged(QVariant value) = 0;
};

#endif // BASEPROPERTYVIEW_H
