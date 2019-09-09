#include "texthelper.h"
#include <QVariant>
#include <QRandomGenerator>

QString TextHelper::FileName(QString path)
{
    QFileInfo f(path);
    return f.fileName();
}

QString TextHelper::FileExtension(QString path)
{
    QFileInfo f(path);
    return f.suffix();
}

QString TextHelper::trim(QString string, QChar trimmer)
{
    while (string.startsWith(trimmer))
    {
        string = string.right(string.length() - 1);
    }
    while (string.endsWith(trimmer))
    {
        string = string.left(string.length() - 1);
    }
    return string;
}

QString TextHelper::trim(QString string, QString trimmer)
{
    while (string.startsWith(trimmer))
    {
        string = string.right(string.length() - 1);
    }
    while (string.endsWith(trimmer))
    {
        string = string.left(string.length() - 1);
    }
    return string;
}

QString TextHelper::TimeToText(double seconds, bool showMs)
{
    int secs = seconds;

    int ms = (seconds - secs)*1000;

    secs = secs%60;
    int minutes = seconds/60;
    int hours = minutes/60;

    QString str = QString("%1").arg(minutes%60, 2, 10, QChar('0')) + ":" + QString("%1").arg(secs, 2, 10, QChar('0'));

    if (showMs)    
        str += "." + QString("%1").arg(ms, 3, 10, QChar('0'));    

    if (hours > 0)    
        str = QString("%1").arg(hours, 2, 10, QChar('0')) + ":" + str;

    return str;
}


QString TextHelper::DateTimeFileString(QDateTime dt, bool withMs)
{
    if (!dt.isValid()) dt = QDateTime::currentDateTime();
    return dt.toString(QString("yyyy.MM.dd-hh.mm.ss") + QString(withMs ? ".zzz" : ""));
}

QStringList TextHelper::reverse(QStringList source)
{
    QStringList result;

    for(int i=source.length()-1; i>=0; i--)    
        result << source.at(i);    

    return result;
}

QString TextHelper::printLikeTable(QMap<QString, QString> values, QString headerSeparator)
{
    if (values.count() == 0) return "";

    QStringList keys = values.keys();
    int headerLength = 0;

    for (int i = 0, sz = keys.count(); i < sz; i++)
    {
        QString key = keys[i];
        int len = key.length();
        if (headerLength < len)
            headerLength = len;
    }

    headerLength ++;

    QString result = "";
    for (int i = 0, sz = keys.count(); i < sz; i++)
    {
        QString key = keys[i];
        int len = key.length() + headerSeparator.length();
        result.append(key + headerSeparator);
        while (len <= headerLength)
        {
            result.append(" ");
            len++;
        }
        result.append(values[key] + "\r\n");
    }
    return result.trimmed();
}

QString TextHelper::QRectToString(QRect rect)
{
    return
            QString::number(rect.x()) + ";" +
            QString::number(rect.y()) + ";" +
            QString::number(rect.width()) + ";" +
            QString::number(rect.height());
}

QRect TextHelper::QRectFromString(QString str)
{
    QRect r;
    if (str.length() >= 7)
    {
        QStringList vals = str.split(";", QString::SkipEmptyParts);
        if (vals.count() == 4)
        {
            bool ok;
            int x = QVariant(vals[0]).toInt(&ok);
            //if (!ok) x = 0;
            int y = QVariant(vals[1]).toInt(&ok);
            int w = QVariant(vals[2]).toInt(&ok);
            int h = QVariant(vals[3]).toInt(&ok);
            r = QRect(x, y, w, h);
        }
    }
    return r;
}

QList<QPoint> TextHelper::QListPointsFromString(QString str)
{
    QList<QPoint> list;
    if (str.length() >= 3)
    {
        QStringList vals = str.split(";", QString::SkipEmptyParts);
        foreach (QString val, vals)
        {
            if (val.length()>=3)
            {
                QStringList l = val.split(",", QString::SkipEmptyParts);
                if (l.count()==2)
                {
                    int x = QVariant(l[0]).toInt();
                    int y = QVariant(l[1]).toInt();

                    list << QPoint(x,y);
                }
            }
        }
    }
    return list;
}

QString TextHelper::toJsonString(QJsonObject obj, QJsonDocument::JsonFormat /*format*/)
{
    return QJsonDocument(obj).toJson(QJsonDocument::Compact);
}

QString TextHelper::QListPointsToString(QList<QPoint> &list)
{
    QString t = "";
    foreach (QPoint p, list)
    {
         t += QString::number(p.x()) + "," + QString::number(p.y()) + ";";
    }
    return t;
}

QString boolify(bool value)
{
    if (value) return "1"; return "0";
}

QString TextHelper::translit(QString text)
{
    const QMap<QString, QString> ruEN
    {
{"а","a" },   {"б","b" },   {"в","v"  },
{"г","g" },   {"д","d" },   {"е","e"  },
{"ё","e" },   {"ж","zh"},   {"з","z"  },
{"и","i" },   {"й","y" },   {"к","k"  },
{"л","l" },   {"м","m" },   {"н","n"  },
{"о","o" },   {"п","p" },   {"р","r"  },
{"с","s" },   {"т","t" },   {"у","u"  },
{"ч","ch"},   {"ш","sh"},   {"щ","sch"},
{"ф","f" },   {"х","h" },   {"ц","ts" },
{"ь","", },   {"ы","y" },   {"ъ",""   },
{"э","e" },   {"ю","yu"},   {"я","ya" },
{"А","A" },   {"Б","B" },   {"В","V"  },
{"Г","G" },   {"Д","D" },   {"Е","E"  },
{"Ё","E" },   {"Ж","Zh"},   {"З","Z"  },
{"И","I" },   {"Й","Y" },   {"К","K"  },
{"Л","L" },   {"М","M" },   {"Н","N"  },
{"О","O" },   {"П","P" },   {"Р","R"  },
{"С","S" },   {"Т","T" },   {"У","U"  },
{"Ч","Ch"},   {"Ш","Sh"},   {"Щ","Sch"},
{"Ф","F" },   {"Х","H" },   {"Ц","Ts" },
{"Ь",""  },   {"Ы","Y" },   {"Ъ",""   },
{"Э","E" },   {"Ю","Yu"},   {"Я","Ya" }
    };

    foreach (QString key, ruEN.keys())
    {
        text = text.replace(key, ruEN[key]);
    }
    return text;
}


QString TextHelper::join(QStringList &list, QString separator, QString leftwrapper, QString rightwrapper)
{
    QString output = "";
    int count = list.count();
    for (int i = 0; i<count; i++)
    {
        output += leftwrapper + list.at(i) + rightwrapper;
        if (i < count-1)
        {
            output += separator;
        }
    }
    return output;
}

QString TextHelper::russianAgeSuffix(int years)
{
    years = qAbs(years);
    int lastDigit = years%10;
    int lastTwoDigits = 0;
    if (years > 10)
    {
        lastTwoDigits = years%100;
    }

    if (lastDigit == 1 && lastTwoDigits != 11) return "год"; //1, 21, 31, 101, 10001
    if (lastDigit >= 2 && lastDigit <= 4 && (lastTwoDigits < 11 || lastTwoDigits > 14)) return "года";
    return "лет";
}

int TextHelper::LevenshteinDistance(QString string1, QString string2)
{
    int l1 = string1.length();
    int l2 = string2.length();

    if (l1 == 0 || l2 == 0) return qMax(l1, l2);

    int diff;
    //TODO: OPTIMIZE LEVENSHTEIN, NOW IT USES A LENGTH OF 100!
    //int m[l1][l2]
    l1 = qMin(l1, 99);
    l2 = qMin(l2, 99);
    int m[100][100];

    for (int i=0;i<=l1;i++)
    {
        m[i][0]=i;
    }

    for (int j=0;j<=l2;j++)
    {
        m[0][j]=j;
    }

    for (int i=1;i<=string1.length();i++)
    {
        for (int j=1;j<=string2.length();j++)
        {
            diff = (string1[i-1]==string2[j-1])?0:1;

            m[i][j]=qMin(qMin(m[i-1][j]+1, m[i][j-1]+1), m[i-1][j-1]+diff);
        }
    }
    return m[l1][l2];
}

int TextHelper::LevenshteinDistanceNoMarksAndCases(QString string1, QString string2)
{
    string1 = string1.replace(".", "").replace("!", "").replace(",", "").replace("?", "").replace("-", "").replace("  ", " ").toLower().trimmed();
    string2 = string2.replace(".", "").replace("!", "").replace(",", "").replace("?", "").replace("-", "").replace("  ", " ").toLower().trimmed();

    return LevenshteinDistance(string1, string2);
}

QString TextHelper::randomString(QStringList source)
{
    auto sourceLength = source.length();
    if (sourceLength == 0) return "";
    if (sourceLength == 1) return source[0];

    return source[QRandomGenerator::global()->bounded(0, sourceLength)];
}


