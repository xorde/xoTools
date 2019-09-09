#ifndef ONBCOMMON_H
#define ONBCOMMON_H

#include <QString>

#define STRING_ID

#pragma pack(push,1)

//! Standard ActionID enumeration.
typedef enum
{
    aidPropagationUp    = 0x40, //!< пересылка сообщения через мастера на уровень выше
    aidPropagationDown  = 0x80, //!< пересылка сообщения через узлы на уровень ниже
    aidPollNodes        = 0x00, //!< опрос узлов, можно добавить пересылку с помощью ИЛИ
    aidConnReset        = 0x01, //!< сброс состояния узлов до disconnected, установка соединения заново
//    aidEnumerate        = 0x02, //!< построение карты сети
    aidPollClasses      = 0x03, //!< опрос классов устройств, все отвечают своим классом

    aidUpgradeStart     = 0x30, //!< запуск обновления прошивки, в данных класс устройства
    aidUpgradeConfirm   = 0x31, //!< подтверждение начала прошивки, чтобы не было случайностей
    aidUpgradeEnd       = 0x32, //!< окончание обновления прошивки
    aidUpgradeSetPage   = 0x33, //!< установка текущей страницы
    aidUpgradeData      = 0x34, //!< собственно, сама прошивка (см. протокол)
    aidUpgradeAddress   = 0x35, //!< адрес начала прошивки
    aidUpgradeProbe     = 0x36, //!< мастер после отправки страницы спрашивает, всё ли ок
    aidUpgradeRepeat    = 0x38, //!< запрос повтора страницы
    aidUpgradePageDone  = 0x39, //!< прошиваемые устройства говорят, что всё хорошо
    aidUpgradeAccepted  = 0x3A, //!< прошиваемые устройства обозначают себя
    aidUpgradeReady     = 0x3B, //!< прошиваемые устройства готовы принимать прошивку
} StdAID;

//! Service ObjectID enumeration.
typedef enum
{
    svcClass            = 0x00, //!< узел говорит, из каких он слоёв общества
    svcName             = 0x01, //!< узел сообщает, как его величать
    svcFullName         = 0x02, //!< полное имя узла или описание
    svcSerial           = 0x03, //!< серийный номер
    svcVersion          = 0x04, //!< версия прошивки
    svcBuildDate        = 0x05, //!< дата и время сборки
    svcCpuInfo          = 0x06, //!< информация о процессоре
    svcBurnCount        = 0x07, //!< без комментариев..
    svcObjectCount      = 0x08, //!< число объектов
    svcBusType          = 0x09, //!< тип шины физического уровня
    svcBusAddress       = 0x0A, //!< физический адрес (можно поменять)
    svcClassName        = 0x0B, //!< имя класса, в теории должно быть уникальным, как и svcClass
    svcIcon             = 0x0C, //!< иконка

    svcObjectInfo       = 0x80, //!< описание объекта
    svcObjectMinimum    = 0x81, //!< минимальное значение объекта
    svcObjectMaximum    = 0x82, //!< максимальное значение объекта
    svcObjectDefault    = 0x83, //!< значение по умолчанию объекта
    svcObjectStep       = 0x84, //!< шаг изменения значения объекта
    svcObjectMimeType   = 0x85, //!< mime-type объекта
    svcObjectHint       = 0x86, //!< текстовое описание объекта
    svcObjectUnit       = 0x87, //!< единица измерения объекта
    svcObjectOptions    = 0x88, //!< пользовательские опции, связанные с объектом (текст)
    svcObjectExtInfo    = 0x89, //!< дополнительные параметры подписки для объекта
    svcObjectEnum       = 0x8A, //!< список значений, если объект является перечислением

    svcTimedObject      = 0x90, //!< ответ узла синхронизованным объектом
    svcGroupedObject    = 0xA0, //!< ответ узла группированным объектом

    svcCreate           = 0xC0, //!< мастер просит создать компонент данного класса (cid в данных)

    svcMessage          = 0xDB, //!< отладочное сообщение узла

    svcUnsubscribe      = 0xEA, //!< отмена подписки, оформленной через svcAutoRequest
    svcSubscribe        = 0xFA, //!< синоним для svcAutoRequest

    svcEcho             = 0xF0, //!< узел напоминает мастеру, что он здесь
    svcHello            = 0xF1, //!< сообщение узла, что он пришёл с приветом, либо мастер играет в юща и спрашивает узла: "А вы, собственно, кто?"
    svcWelcome          = 0xF2, //!< мастер приглашает в гости
    svcWelcomeAgain     = 0xF3, //!< мастер уже приглашал в гости
    svcConnected        = 0xF4, //!< мастер сообщает вверх, что пришёл девайс
    svcDisconnected     = 0xF5, //!< мастер понял, что девайс отключился, и передаёт по цепочке главному
    svcKill             = 0xF6, //!< мастеру надоело, что девайс в отключке, он его убивает и рассказывает об этом всем
    svcRequestAllInfo   = 0xF8, //!< мастер хочет знать всё о девайсе и даже больше
    svcRequestObjInfo   = 0xF9, //!< мастер хочет знать дескрипторы всех зареганных объектов девайса
    svcAutoRequest      = 0xFA, //!< мастер говорит узлу, что хочет от него порой получать объект без лишних слов
    svcTimedRequest     = 0xFB, //!< мастер говорит узлу, что хочет получать объект с timestamp'ом
    svcGroupedRequest   = 0xFC, //!< мастер говорит узлу, что хочет получать группированный объект
    svcUpgradeRequest   = 0xFE, //!< мастер просит включить бутлоадер
    svcFail             = 0xFF, //!< узел сообщает мастеру, что это фиаско братан (типа запрос не поддерживается)
} SvcOID;

typedef enum
{
    BusUnknown  = 0,
    BusCan      = 1,
    BusUsbHid   = 2,
    BusWifi     = 3,
    BusSwonb    = 4,
    BusVirtual  = 5,
    BusRadio    = 6,
    BusWebSocket= 7,
} BusType;

//typedef enum
//{
//    netAddrUniversal    = 0x7F,
//    netAddrInvalid      = 0xFF
//} NetAddress;

////! Local Message ID type.
//struct LocalMsgId
//{
//    unsigned char oid;          //!< object ID
//    unsigned char sender: 7;    //!< sender network address (logical)
//    unsigned char frag: 1;      //!< message is fragmented
//    unsigned char addr: 7;      //!< receiver network address (logical)
//    unsigned char svc: 1;       //!< message is service
//    unsigned char mac: 4;       //!< receiver bus address (physical)
//    const unsigned char local: 1; //!< local area message (inside the bus) = 1
//    unsigned char _not_used: 3; //!< reserved bits in CAN 2.0B spec
//    /*! Конструктор обнуляет. */
//    LocalMsgId() :
//      oid(0), sender(0), frag(0), addr(0), svc(0), mac(0), local(1), _not_used(0)
//    {
//    }
//    /*! Неявный конструктор. */
//    LocalMsgId(const uint32_t &data) :
//      local(1), _not_used(0)
//    {
//        *reinterpret_cast<uint32_t*>(this) = data;
//    }
//    /*! Приведение типа к uint32_t */
//    operator uint32_t&() {return *reinterpret_cast<uint32_t*>(this);}
//    /*! Приведение типа из uint32_t */
//    void operator =(const uint32_t &data) {*reinterpret_cast<uint32_t*>(this) = data;}
//};

////! Global Message ID type.
//struct GlobalMsgId
//{
//    unsigned char aid;      //!< action ID
//    unsigned char res;      //!< reserved
//    unsigned char addr: 7;  //!< own network address (logical)
//    unsigned char svc: 1;   //!< message is service
//    unsigned char mac: 4;   //!< own bus address (physical)
//    unsigned char local: 1; //!< local area message (inside the bus) = 0
//    unsigned char _not_used: 3; //!< reserved bits in CAN 2.0B spec
//    /*! Конструктор обнуляет. */
//    GlobalMsgId() :
//      aid(0), res(0), addr(0), svc(0), mac(0), local(0), _not_used(0)
//    {
//    }
//    /*! Неявный конструктор. */
//    GlobalMsgId(const uint32_t &data) :
//      local(0), _not_used(0)
//    {
//        *reinterpret_cast<uint32_t*>(this) = data;
//    }
//    /*! Приведение типа к uint32_t */
//    operator uint32_t&() {return *reinterpret_cast<uint32_t*>(this);}
//    /*! Приведение типа из uint32_t */
//    void operator =(const uint32_t &data) {*reinterpret_cast<uint32_t*>(this) = data;}
//};

//! ONBPacket header
struct ONBHeader
{
    unsigned char objectID;     //!< unique ID of the object (0...255)
    unsigned short componentID; //!< unique ID of the component [aka address] (1...65535), 0 is reserved for current module
    unsigned char _not_used: 4; //!< reserved for the future
    unsigned char local: 1;     //!< set to 0 for message broadcasting
    unsigned char classInfo: 1; //!< message contains classID and info for it
    unsigned char svc: 1;       //!< flag of service message
    unsigned char headerFlag: 1;//!< message is ONBPacket

    /*! Конструктор обнуляет. */
    ONBHeader()
    {
        *reinterpret_cast<uint32_t*>(this) = 0x80000000; // headerFlag = 1
    }
    /*! Неявный конструктор. */
    ONBHeader(const uint32_t &data)
    {
        *reinterpret_cast<uint32_t*>(this) = data;
        _not_used = 0;
        headerFlag = 1;
    }
    /*! Приведение типа к uint32_t */
    operator uint32_t&() {return *reinterpret_cast<uint32_t*>(this);}
    /*! Приведение типа из uint32_t */
    void operator =(const uint32_t &data) {*reinterpret_cast<uint32_t*>(this) = data;}

};

#pragma pack(pop)

#endif // ONBCOMMON_H
