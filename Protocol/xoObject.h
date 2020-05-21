#ifndef XOOBJECT_H
#define XOOBJECT_H

#include "xoObjectBase.h"

using namespace std::chrono_literals;

template <typename T>
class xoObject : public xoObjectBase<T>
{
public:
    xoObject() : xoObjectBase<T>() {}
    xoObject(const xoObject<T> &copy)
    {
        *this->m_ptr = *copy.m_ptr;
    }
    xoObject(const T &value) : xoObjectBase<T>(value) {}

    void operator =(const T &v) {xoObjectBase<T>::operator=(v);}
//    {
//        *m_ptr = v;
//        setEvent();
//    }

    //! @brief Set the minimum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The minimum value to be set.
    //! @return reference to the object.
    xoObject<T> &min(const T &value)
    {
        xoObjectBase<T>::m_min = value;
        setExtFlag(ObjectBase::MV_Min);
        return *this;
    }

    //! @brief Set the maximum value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The maximum value to be set.
    //! @return reference to the object.
    xoObject<T> &max(const T &value)
    {
        xoObjectBase<T>::m_max = value;
        setExtFlag(ObjectBase::MV_Max);
        return *this;
    }

    //! @brief Set the default value of the object.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The default value to be set.
    //! @return reference to the object.
    xoObject<T> &def(const T &value)
    {
        xoObjectBase<T>::m_def = value;
        xoObjectBase<T>::m_value = value;
        setExtFlag(ObjectBase::MV_Def);
        return *this;
    }

    //! @brief Set the step value of the object.
    //! @details Step is used commonly for changing the value in discrete steps.
    //! @attention This meta-value is only for information and does not apply to the variable.
    //! @param value The step value to be set.
    //! @return reference to the object.
    xoObject<T> &step(const T &value)
    {
        xoObjectBase<T>::m_step = value;
        setExtFlag(ObjectBase::MV_Step);
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
    xoObject<T> &MMD(const T &minValue, const T &maxValue, const T &defValue)
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
    xoObject<T> &mime(const QString &value)
    {
        xoObjectBase<T>::m_mimeType = value;
        setExtFlag(ObjectBase::MV_Mime);
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
    xoObject<T> &hint(const QString &value)
    {
        xoObjectBase<T>::m_hint = value;
        setExtFlag(ObjectBase::MV_Hint);
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
    xoObject<T> &unit(const QString &value)
    {
        xoObjectBase<T>::m_unit = value;
        setExtFlag(ObjectBase::MV_Unit);
        return *this;
    }

    //! @brief Set the optional text associated to the object.
    //! @details Can contain arbitrary text, but some special words are used for UI enhancement.
    //! @param value The string of optional text to be set.
    //! @return reference to the object.
    xoObject<T> &opt(const QString &value)
    {
        xoObjectBase<T>::m_options = value;
        setExtFlag(ObjectBase::MV_Options);
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
    xoObject<T> &sampling(std::chrono::milliseconds value)
    {
        ObjectBase::m_RMIP = value.count();
        ObjectBase::m_needTimestamp = ObjectBase::m_RMIP > 0; // it's evident
        setExtFlag(ObjectBase::MV_RMIP);
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
    xoObject<T> &enumeration(QString s, Args... a)
    {
        this->m_enum << s;
        return enumeration(a...);
    }

    //! @brief Specify the names of the enumeration object with given string list.
    //! @details This is overloaded function.
    //! @param list The string list to be set.
    //! @return reference to the object.
    //! @see ONBObject::enumeration(QString s, Args... a)
    template<typename... Args>
    xoObject<T> &enumeration(QStringList list)
    {
        for (auto item: list)
            this->m_enum << item;
        return enumeration();
    }

private:
    xoObject<T> &enumeration()
    {
        min(static_cast<T>(0));
        max(static_cast<T>(this->m_enum.count() - 1));
        setExtFlag(ObjectBase::MV_Enum);
        return *this;
    }
};

#endif // XOOBJECT_H
