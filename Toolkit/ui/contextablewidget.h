#ifndef CONTEXTABLEWIDGET_H
#define CONTEXTABLEWIDGET_H

#include <QWidget>

class ContextableWidget : public QWidget
{
    Q_OBJECT   
public:
    Q_PROPERTY(QObject* context READ getContext WRITE setContext MEMBER m_context NOTIFY contextChanged)
    Q_INVOKABLE void setContext(QObject* value)
    {
        m_context = value;
        emit contextChanged(m_context);
        onContextChanged();
    }
    Q_INVOKABLE QObject* getContext() { return m_context; }
    ContextableWidget(QWidget *parent = nullptr);
    virtual ~ContextableWidget();
private:
    QObject* m_context;
    virtual void onContextChanged() = 0;
signals:
    Q_SIGNAL void contextChanged(QObject* value);

};

#endif // CONTEXTABLEWIDGET_H
