#ifndef APPHELPER_H
#define APPHELPER_H

#include <QApplication>
#include <QProcess>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include "xotools_global.h"

namespace AppHelper
{
    XOTOOLSSHARED_EXPORT void quit();

    XOTOOLSSHARED_EXPORT void restart(QStringList args);

    XOTOOLSSHARED_EXPORT void restart();

    XOTOOLSSHARED_EXPORT void openExplorer(QString folder);

    XOTOOLSSHARED_EXPORT bool confirmDialog(QString caption, QString text);

    XOTOOLSSHARED_EXPORT void warningMessage(QString text, QString caption = "", QMessageBox::Icon icon = QMessageBox::Warning);

    XOTOOLSSHARED_EXPORT void clearList(QList<QObject *> &list);

    const QStringList StandartImageFileTypes = { "jpg", "png", "jpeg", "bmp" };
    const QStringList StandartVideoFileTypes = { "avi", "mpg", "mpeg", "mp4" };


    XOTOOLSSHARED_EXPORT QString getFileFilterString(QString fileDescription, QStringList fileTypes);

#ifdef Q_OS_WIN
    XOTOOLSSHARED_EXPORT QStringList getRunningApps();
    XOTOOLSSHARED_EXPORT bool isAppRunnning(QString appName);
    XOTOOLSSHARED_EXPORT void closeApp(QString appName);

#endif

    XOTOOLSSHARED_EXPORT void startApp(const QString &program, const QStringList &arguments = QStringList(),
                        const QString &workingDirectory = QString());
}

#endif // APPHELPER_H
