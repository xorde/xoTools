#include "metadescription.h"
#include <QDebug>
#include <QCoreApplication>
#include <QRegExp>
#include <QColor>
#include "texthelper.h"

MetaDescription::MetaDescription(QObject *parent) : AbstractMetaDescription(parent)
{
    hasMin = false;
    hasMax = false;
    hasDefault = false;
    hasStep = false;
    hasSpecialType = false;
}

void MetaDescription::set(int metapropertyindex, QObject* object)
{
    m_metaindex = metapropertyindex;
    m_object = object;
    auto metaproperty = metaprop();

    hasMin = false;
    hasMax = false;
    hasStep = false;

    name = QString(metaproperty.name());

    typeName = metaproperty.typeName();
    typeId = metaproperty.type();

    QString editableString = stringForAlias("Editable");
    changeable = !editableString.isNull() && editableString == "true";
    visibleInUI = clindex("NotVisible") < 0;

    QString displayNameString = stringForAlias("DisplayName");
    if (!displayNameString.isNull()) displayName = displayNameString;
    else displayName = name;

    QString defaultString = stringForAlias("Default");
    if (!defaultString.isNull())
    {
        m_def = defaultString;

        if(metaprop().isEnumType())
        {
            m_def = defaultString.mid(defaultString.lastIndexOf("::") + 2);
            typeName = "Enum";
            typeId = QVariant::Int;
        }
        else if (typeName == "int" || typeName == "float" || typeName == "double")
        {
            m_def = defaultString.remove(QRegExp("[a-zA-Z]"));
        }
        else if (typeName == "QString")
        {
            m_def = TextHelper::trim(m_def, '"');
        }
        else if (typeName == "QColor")
        {
            QRegExp regExp("(\\#[a-fA-F0-9]+)");
            if(regExp.indexIn(m_def) >= 0)
                m_def = regExp.cap(0);
            else
            {
                QRegExp regExp("Qt::(\\w+)");
                if(regExp.indexIn(m_def) >= 0)
                    m_def = QColor(regExp.cap(1)).name();
                else
                {
                    QRegExp regExp("\\(\\\"(\\w+)\\\"\\)");
                    if(regExp.indexIn(m_def) >= 0)
                        m_def = QColor(regExp.cap(1)).name();
                }
            }
        }

        hasDefault = true;
    }   

    QString bindString = stringForAlias("VisibleBinded");
    if(!bindString.isNull())
    {
        int colonIndex = bindString.indexOf(":");

        if(colonIndex != -1)
        {
            bindedVisibilitySource = bindString.left(colonIndex);
            bindedValue = bindString.mid(colonIndex + 1);
        }
    }

    QString minString = stringForAlias("Minimum");
    if (!minString.isNull()) hasMin = assignDouble(m_min, minString);

    QString prefixString = stringForAlias("Prefix");
    if (!prefixString.isNull()) prefix = prefixString;

    QString suffixString = stringForAlias("Suffix");
    if (!suffixString.isNull()) suffix = suffixString;

    QString descString = stringForAlias("Description");
    if (!descString.isNull()) description = descString;

    QString maxString = stringForAlias("Maximum");
    if (!maxString.isNull()) hasMax = assignDouble(m_max, maxString);

    QString stepString = stringForAlias("Step");
    if (!stepString.isNull()) hasStep = assignDouble(m_step, stepString);

    //qDebug() << displayName << minString << maxString << stepString << hasStep;


    QString specialString = stringForAlias("SpecialType");
    if(!specialString.isNull())
    {
        specialType = specialString;
        hasSpecialType = true;

        int columnIndex = specialType.indexOf(":");
        if(columnIndex == -1)
            pickOption = specialType;
        else
        {
            pickOption = specialType.left(columnIndex);
            pickExtensions = specialType.mid(columnIndex + 1);
        }
    }

    QString categoryString = stringForAlias("Category");
    if (!categoryString.isNull()) category = categoryString;

    QString priorityString = stringForAlias("Priority");
    if(!priorityString.isNull()) priority = priorityString.toInt();

    QString noDefaultString = stringForAlias("NoDefault");
    if(!noDefaultString.isNull()) showDefault = false;

    QString readOnlyString = stringForAlias("ReadOnly");
    if(!readOnlyString.isNull()) isReadOnly = true;

    if (metaproperty.hasNotifySignal())
    {
        QMetaMethod signal = metaproperty.notifySignal();
        QMetaMethod updateSlot = metaObject()->method(metaObject()->indexOfSlot("valueChangedSlot()"));
        connect(m_object, signal, this, updateSlot);
    }
}

void MetaDescription::valueChangedSlot()
{
    emit valueChanged(getValue());
}

bool MetaDescription::setValue(const QVariant& value)
{
    if (metaprop().isWritable()) return metaprop().write(m_object, value);
    return false;
}

QMetaProperty MetaDescription::metaprop()
{
    return m_object->metaObject()->property(m_metaindex);
}

const QVariant MetaDescription::getValue()
{
    try
    {
        return metaprop().read(m_object);
    }
    catch (...)
    {
        qDebug() << "ERROR getValue of" << name;
    }

    return QVariant();
}


QString MetaDescription::clinfo(QString value)
{
    return QString(name + value);
}

int MetaDescription::clindex(QString value)
{
    return m_object->metaObject()->indexOfClassInfo(clinfo(value).toUtf8().constData());
}

void MetaDescription::setDefault()
{
    if (hasDefault)
    {
        setTextValue(m_def);
        emit valueChanged(getValue());
    }
}

bool MetaDescription::setTextValue(QString value)
{
    return setValue(QVariant(value));
}

QMetaEnum MetaDescription::enumerator()
{
    return metaprop().enumerator();
}

bool MetaDescription::isEnumType()
{
    return metaprop().isEnumType();
}

bool MetaDescription::assignDouble(double &out, QString &source)
{
    bool ok = false;
    source.replace("f", "");
    double result = QString(source).toDouble(&ok);

    if (ok)
    {
        out = result;
        return true;
    }

    return false;
}

QString MetaDescription::stringForAlias(QString alias)
{
    int index = clindex(alias);
    if(index != -1)
    {
        return QString(m_object->metaObject()->classInfo(index).value());
    }

    return QString();
}
