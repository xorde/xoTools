#include "apphelper.h"
#include "texthelper.h"

#include <QDebug>
#include <QMessageBox>

void AppHelper::quit()
{
    qApp->quit();
}

void AppHelper::restart(QStringList args)
{
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], args);
}

void AppHelper::restart()
{
    restart(qApp->arguments());
}

void AppHelper::openExplorer(QString folder)
{
    QProcess::startDetached("explorer.exe \""+folder.replace("/", "\\")+"\"");
}

void AppHelper::warningMessage(QString text, QString caption, QMessageBox::Icon icon)
{
    QMessageBox warningBox(icon, caption, text);
    warningBox.setWindowFlags(warningBox.windowFlags() | Qt::WindowStaysOnTopHint);
    warningBox.exec();
}

bool AppHelper::confirmDialog(QString caption, QString text)
{
    QMessageBox warningBox(QMessageBox::Warning, caption, text, QMessageBox::Yes | QMessageBox::Cancel);
    warningBox.setWindowFlags(warningBox.windowFlags() | Qt::WindowStaysOnTopHint);
    return warningBox.exec() == QMessageBox::Yes;
}

void AppHelper::clearList(QList<QObject *> &list)
{
    foreach(auto item, list)
    {
        delete item;
    }
    list.clear();
}

QString AppHelper::getFileFilterString(QString fileDescription, QStringList fileTypes)
{
    QString result = "";
    if (!fileDescription.isEmpty())
    {
        result += fileDescription + " ";
    }

    if (fileTypes.count() > 0)
    {
        result += "(";

        QStringList resultingList;

        foreach (QString fileType, fileTypes)
        {
            if (fileType.startsWith("*."))
            {
                fileType = fileType.mid(2);
            }
            else if (fileType.startsWith("."))
            {
                fileType = fileType.mid(1);
            }

            if (!fileType.isEmpty())
            {
                resultingList.append(fileType);
            }
        }
        result += TextHelper::join(resultingList, " ", "*.");


        result += ");";
    }

    return result;

}

#ifdef Q_OS_WIN
QStringList AppHelper::getRunningApps()
{
    QProcess process;
    process.setReadChannel(QProcess::StandardOutput);
    process.setReadChannelMode(QProcess::MergedChannels);
    process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");

    process.waitForStarted(1000);
    process.waitForFinished(1000);

    QByteArray listBytes = process.readAll();
    QString listStr = QString(listBytes);
    QStringList list = listStr.split("\n", QString::SkipEmptyParts);
    QStringList result;

    foreach(auto pr, list)
    {
        if (pr.contains("."))
        {
            result << pr.trimmed();
        }
    }
    //qDebug() << result;
    return result;
}

bool AppHelper::isAppRunnning(QString appName)
{
    return getRunningApps().contains(appName, Qt::CaseInsensitive);
}

void AppHelper::closeApp(QString appName)
{
    QProcess p;
    //qDebug() << "taskkill /im "+appName+" /f";
    p.start("taskkill /im "+appName+" /f");
    p.waitForFinished();
}
#endif

void AppHelper::startApp(const QString &program, const QStringList &arguments, const QString &workingDirectory)
{
    QProcess::startDetached(program, arguments, workingDirectory);
}
