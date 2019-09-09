#ifndef ONBOBJECT_H
#define ONBOBJECT_H

#include "ObjectInfo.h"
#include <chrono>
#include <functional>
#include <QStringList>

using namespace std::chrono_literals;

//! @brief Component-side ONB Object.
//!
//! It used for data exchange between components.
//!
//! ONBObject contains variable binding and meta-information about it
//! such as name, type, size, access flags and meta-values.
//! Assigning meta-values is provided by methods min(), max() etc.
//! that return a reference to the object,
//! so you can write meta-description of the object in one line.
//! All meta-values are optional.
//!
//! To create ONB objects use convenient methods of ComponentBase class.
//! @code
//!  int foo = 0;
//!  createSetting("foo", foo).min(-10).max(10).hint("The amount of foo");
//! @endcode
//! @see ComponentBase::createInput(), ComponentBase::createOutput(), ComponentBase::createSetting()
template<typename T>
class ONBObject : public ObjectInfo
{
public:
    //! @brief Default constructor.
    //! @details Sets ONB Object to contain ExtInfo since ONB protocol v2.
    //! You shouln't create ONBObject explicitly.
    //! Use more convenient methods of ComponentBase class instead.
    //! @see ComponentBase::createInput(), ComponentBase::createOutput(), ComponentBase::createSetting()
    explicit ONBObject()
    {
        mDesc.extFlags |= ExtInfo;
    }

    //! @brief Set the minimum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The minimum value to be set.
    //! @return reference to the object.
    ONBObject<T> &min(const T &value)
    {
        delete m_min;
        m_min = new T(value);
        mDesc.extFlags |= Minimum;
        return *this;
    }

    //! @brief Set the maximum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The maximum value to be set.
    //! @return reference to the object.
    ONBObject<T> &max(const T &value)
    {
        delete m_max;
        m_max = new T(value);
        mDesc.extFlags |= Maximum;
        return *this;
    }

    //! @brief Set the default value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The default value to be set.
    //! @return reference to the object.
    ONBObject<T> &def(const T &value)
    {
        delete m_def;
        m_def = new T(value);
        mDesc.extFlags |= Default;
        return *this;
    }

    //! @brief Set the step value of the object.
    //! @details Step is used commonly for changing the value in discrete steps.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The step value to be set.
    //! @return reference to the object.
    ONBObject<T> &step(const T &value)
    {
        delete m_step;
        m_step = new T(value);
        mDesc.extFlags |= Step;
        return *this;
    }

    //! @brief Set the minimum, maximum and default value of the object.
    //! @details The method is implemented as consequent call to min(), max() and def()
    //! and can be used to simplify the code.
    //! @param minValue The minimum value to be set.
    //! @param maxValue The maximum value to be set.
    //! @param defValue The default value to be set.
    //! @return reference to the object.
    //! @see min(), max(), def().
    ONBObject<T> &MMD(const T &minValue, const T &maxValue, const T &defValue)
    {
        return min(minValue).max(maxValue).def(defValue);
    }

    //! @brief Set the mime-type of the object.
    //! @details Mime-type is arbitrary text used for specifying content of the object.
    //! \n For example:
    //! @code
    //!  QByteArray frameBuffer;
    //!  createOutput("frame", frameBuffer).mime("image/png");
    //! @endcode
    //! @param value The string containing mime-type to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createOutput()
    ONBObject<T> &mime(const QString &value)
    {
        m_mimeType = value;
        mDesc.extFlags |= MimeType;
        return *this;
    }

    //! @brief Set the human-readable description of the object.
    //! @details The description helps users to understand purpose of the object.\n
    //! For example, if you'd develop some kind of PID controller
    //! you probably should write like this:
    //! @code
    //!  createSetting("Kp", Kp).hint("The coefficient for the proportional term");
    //! @endcode
    //! @param value The string containing description to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createSetting()
    ONBObject<T> &hint(const QString &value)
    {
        m_hint = value;
        mDesc.extFlags |= Hint;
        return *this;
    }

    //! @brief Set the unit of the object.
    //! @details Unit is arbitrary text used for specifying measure of the value of the object.
    //! @code
    //!  createInput("frequency", freq).unit("Hz");
    //! @endcode
    //! @param value The string containing unit to be set.
    //! @return reference to the object.
    //! @see ComponentBase::createInput()
    ONBObject<T> &unit(const QString &value)
    {
        m_unit = value;
        mDesc.extFlags |= Unit;
        return *this;
    }

    //! @brief Set the optional text associated to the object.
    //! @details Can contain arbitrary text, but some special words are used for UI enhancement.
    //! @param value The string of optional text to be set.
    //! @return reference to the object.
    ONBObject<T> &opt(const QString &value)
    {
        m_options = value;
        mDesc.extFlags |= Options;
        return *this;
    }

    //! @brief Set the default sampling interval of the object.
    //! @details It is applicable to inputs or outputs and denotes the recommended time interval
    //! to request or send the value of the object.
    //! If the sampling interval is set, the object will include timestamp.
    //! and can be used in real-time dataflow.
    //! @param value The sampling interval. If the special value of zero is set, the object will be sent as soon as it is touched.
    //! @return reference to the object.
    //! @see ComponentBase::touchOutput().
    ONBObject<T> &sampling(std::chrono::milliseconds value)
    {
        m_extInfo.RMIP = value.count();
        m_extInfo.needTimestamp = m_extInfo.RMIP > 0;
        return *this;
    }

    //! @brief Specify the list of the enumeration object values.
    //! @details The object can be bound to integer variable, the string or the native enumeration.
    //! If the object bound to integer variable, it defines the minimum and maximum values
    //! as 0 and (count-1) respectively.
    //! If the object bound to the string, the given names are the possible values of the string.
    //! @code
    //!  createSetting("Shape", shape).enumeration("Square", "Circle", "Triangle");
    //! @endcode
    //! @param s The first string to be set.
    //! @param a The next strings, you can provide arbitrary argument count.
    //! @return reference to the object.
    //! @see ComponentBase::createSetting()
    template<typename... Args>
    ONBObject<T> &enumeration(QString s, Args... a)
    {
        m_enum << s;
        return enumeration(a...);
    }

    //! @brief Specify the names of the enumeration object with given string list.
    //! @details This is overloaded function.
    //! @param list The string list to be set.
    //! @return reference to the object.
    //! @see ONBObject::enumeration(QString s, Args... a)
    template<typename... Args>
    ONBObject<T> &enumeration(QStringList list)
    {
        for (auto item: list)
            m_enum << item;
        return enumeration();
    }

protected:
    //! @private
    virtual bool readMeta(QByteArray &ba, MetaValue id) const override
    {
        switch (id)
        {
        case MV_Min: return doRead(ba, m_min);
        case MV_Max: return doRead(ba, m_max);
        case MV_Def: return doRead(ba, m_def);
        case MV_Step: return doRead(ba, m_step);
        case MV_Mime: ba.append(m_mimeType.toUtf8()); return true;
        case MV_Hint: ba.append(m_hint.toUtf8()); return true;
        case MV_Unit: ba.append(m_unit.toUtf8()); return true;
        case MV_Options: ba.append(m_options.toUtf8()); return true;
        case MV_ExtInfo: ba.append(reinterpret_cast<const char*>(&m_extInfo), sizeof(ExtendedInfo)); return true;
        case MV_Enum: ba.append(m_enum.join(",").toUtf8()); return true;
        }
        return false;
    }

private:
    T *m_min = nullptr;
    T *m_max = nullptr;
    T *m_def = nullptr;
    T *m_step = nullptr;
    QString m_mimeType;
    QString m_hint;
    QString m_unit;
    QString m_options;
    QStringList m_enum;

    // TODO: do something with it:
//    std::function<void(void)> m_onRequest, m_onReceive, m_onValueChange;

    ONBObject<T> &enumeration()
    {
        min(static_cast<T>(0));
        max(static_cast<T>(m_enum.count() - 1));
        mDesc.extFlags |= Enum;
        return *this;
    }
};

#endif // ONBOBJECT_H
