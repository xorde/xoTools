#ifndef MULTILINEPROPERTYVIEW_H
#define MULTILINEPROPERTYVIEW_H

#include <QWidget>
#include "BasePropertyView.h"
#include <QPlainTextEdit>

class MultilinePropertyView : public BasePropertyView
{
    Q_OBJECT
public:
    explicit MultilinePropertyView(AbstractMetaDescription *description, QWidget *parent = nullptr);

   QPlainTextEdit *txt = nullptr;

   QWidget *getEditor() override;

   void nullifyEditor() override;

protected slots:
    void metaValueChanged(QVariant value) override;

};

#endif // MULTILINEPROPERTYVIEW_H
