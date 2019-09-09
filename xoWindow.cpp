#include "xoWindow.h"
#include "ui/GlobalConsoleWidget.h"
#include "ui/nrectangle.h"

#include <QPushButton>
#include <QLineEdit>
#include <QLayout>

xoWindow::xoWindow(ModuleBaseAppONB *module, QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle(module->getName());
    setWindowIcon(QIcon(module->getIcon()));

    m_module = module;
    m_module->tryConnect();

    static QColor colorRed = QColor("#C30");
    static QColor colorGreen = QColor("#3C0");

    auto status = new NRectangle(colorRed);
    status->setFixedSize(16, 16);

    auto hostEdit = new QLineEdit();
    hostEdit->setText(module->getHost());

    auto portEdit = new QLineEdit();
    portEdit->setText(module->getPort());

    auto button = new QPushButton(tr("Соединить"));
    auto console = new GlobalConsoleWidget();

    auto horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(status);
    horizontalLayout->addWidget(hostEdit);
    horizontalLayout->addWidget(portEdit);
    horizontalLayout->addWidget(button);

    auto layout = new QVBoxLayout();
    layout->addLayout(horizontalLayout);
    layout->addWidget(console);

    auto widget = new QWidget();
    widget->setLayout(layout);

    setCentralWidget(widget);

    connections << connect(module, &ModuleBaseAppONB::connectionDone, [=]() { status->setBackground(colorGreen); });
    connections << connect(module, &ModuleBaseAppONB::error, [=]() { status->setBackground(colorRed); });

    connections << connect(button, &QPushButton::clicked, [=]()
    {
        m_module->setHost(hostEdit->text());
        m_module->setPort(portEdit->text());
        m_module->tryConnect();
    });
}

xoWindow::~xoWindow()
{
    for(auto& connection : connections) disconnect(connection);

    connections.clear();
}
