#include "GlobalConsoleWidget.h"
#include "uihelper.h"


GlobalConsoleWidget::GlobalConsoleWidget(QWidget *parent) : QWidget(parent)
{
    m_uiText = new QPlainTextEdit();
    UIHelper::InsertToWidget(this, m_uiText);

    m_uiText->setPlainText(GlobalConsole::currentText());
    m_uiText->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_uiText->setStyleSheet("font-family: Consolas, 'Courier New', monospace");

    connect(GlobalConsole::Instance(), SIGNAL(newEntry(QString)), SLOT(newEntry(QString)));
}

void GlobalConsoleWidget::newEntry(QString text)
{
     if (!m_uiText) return;
    m_uiText->setPlainText(text + "\n" + m_uiText->toPlainText());
}

void GlobalConsoleWidget::clear()
{
    if (!m_uiText) return;
    m_uiText->clear();
}
