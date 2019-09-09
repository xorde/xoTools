QT += core gui websockets svg widgets
TARGET = xoTools
TEMPLATE = lib

include("Compilation.pri")

DEFINES += XOTOOLS_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Protocol/ONBPacket.cpp \
    ModuleBaseONB.cpp \
    ModuleBaseAppONB.cpp \
    ModuleBaseLibONB.cpp \
    Protocol/objectdescription.cpp \
    Protocol/ObjectInfo.cpp \
    Protocol/onbobject.cpp \
    ComponentBase.cpp \
    Toolkit/AbstractSettings.cpp \
    Toolkit/network/WebSocket/WebSocketClient.cpp \
    Toolkit/network/WebSocket/WebSocketServer.cpp \
    Toolkit/ui/fpswidget.cpp \
    Toolkit/ui/properties/AbstractMetaDescription.cpp \
    Toolkit/ui/properties/AbstractMetaDescriptor.cpp \
    Toolkit/ui/properties/BasePropertyListWidget.cpp \
    Toolkit/ui/properties/BasePropertyView.cpp \
    Toolkit/ui/properties/BoolPropertyView.cpp \
    Toolkit/ui/properties/ColorPropertyView.cpp \
    Toolkit/ui/properties/DatePropertyView.cpp \
    Toolkit/ui/properties/DoublePropertyView.cpp \
    Toolkit/ui/properties/EnumPropertyView.cpp \
    Toolkit/ui/properties/EnumStringListPropertyView.cpp \
    Toolkit/ui/properties/IntPropertyView.cpp \
    Toolkit/ui/properties/metadescription.cpp \
    Toolkit/ui/properties/metadescriptor.cpp \
    Toolkit/ui/properties/MultilinePropertyView.cpp \
    Toolkit/ui/properties/propertylistcontrol.cpp \
    Toolkit/ui/properties/PropertyTabWidget.cpp \
    Toolkit/ui/properties/SliderPropertyView.cpp \
    Toolkit/ui/properties/SpecialPropertyView.cpp \
    Toolkit/ui/properties/StringPropertyView.cpp \
    Toolkit/ui/properties/PropertiesSorter.cpp \
    Toolkit/ui/nlistbox.cpp \
    Toolkit/ui/nscrollviewer.cpp \
    Toolkit/ui/uihelper.cpp \
    Toolkit/ui/clickablewidget.cpp \
    Toolkit/ui/NPushButton.cpp \
    Toolkit/ui/nstackpanel.cpp \
    Toolkit/ui/contextablewidget.cpp \
    Toolkit/ui/GlobalConsoleWidget.cpp \
    Toolkit/ui/nrectangle.cpp \
    Toolkit/helpers/ConnectionHelper.cpp \
    Toolkit/helpers/CursorHelper.cpp \
    Toolkit/fileutilities.cpp \
    Toolkit/nlist.cpp \
    Toolkit/apphelper.cpp \
    Toolkit/thread_safe/thread_safe_queue.cpp \
    Toolkit/thread_safe/thread_safe_stack.cpp \
    Toolkit/observablecollection.cpp \
    Toolkit/GlobalConsole.cpp \
    Toolkit/texthelper.cpp \
    Toolkit/nlogger.cpp \
    xoWindow.cpp


HEADERS += \
    xotools_global.h \
    Protocol/ONBPacket.h \
    Protocol/ONBCommon.h \
    ModuleBaseONB.h \
    ModuleBaseAppONB.h \
    ModuleBaseLibONB.h \
    ComponentBase.h \
    Protocol/objectdescription.h \
    Protocol/ObjectInfo.h \
    Protocol/onbobject.h \
    Toolkit/AbstractSettings.h \
    Toolkit/network/WebSocket/WebSocketClient.h \
    Toolkit/network/WebSocket/WebSocketServer.h \
    Toolkit/frameratecounter.h \
    Toolkit/ui/fpswidget.h \
    Toolkit/ui/properties/AbstractMetaDescription.h \
    Toolkit/ui/properties/AbstractMetaDescriptor.h \
    Toolkit/ui/properties/BasePropertyListWidget.h \
    Toolkit/ui/properties/BasePropertyView.h \
    Toolkit/ui/properties/BoolPropertyView.h \
    Toolkit/ui/properties/ColorPropertyView.h \
    Toolkit/ui/properties/DatePropertyView.h \
    Toolkit/ui/properties/DoublePropertyView.h \
    Toolkit/ui/properties/EnumPropertyView.h \
    Toolkit/ui/properties/EnumStringListPropertyView.h \
    Toolkit/ui/properties/IntPropertyView.h \
    Toolkit/ui/properties/metadescription.h \
    Toolkit/ui/properties/metadescriptor.h \
    Toolkit/ui/properties/MultilinePropertyView.h \
    Toolkit/ui/properties/propertylistcontrol.h \
    Toolkit/ui/properties/PropertyTabWidget.h \
    Toolkit/ui/properties/SliderPropertyView.h \
    Toolkit/ui/properties/SpecialPropertyView.h \
    Toolkit/ui/properties/StringPropertyView.h \
    Toolkit/ui/properties/PropertiesSorter.h \
    Toolkit/ui/nlistbox.h \
    Toolkit/ui/nscrollviewer.h \
    Toolkit/ui/uihelper.h \
    Toolkit/ui/clickablewidget.h \
    Toolkit/ui/NPushButton.h \
    Toolkit/ui/nstackpanel.h \
    Toolkit/ui/contextablewidget.h \
    Toolkit/ui/GlobalConsoleWidget.h \
    Toolkit/ui/nrectangle.h \
    Toolkit/commontypes.h \
    Toolkit/helpers/ConnectionHelper.h \
    Toolkit/helpers/CursorHelper.h \
    Toolkit/fileutilities.h \
    Toolkit/nlist.h \
    Toolkit/apphelper.h \
    Toolkit/thread_safe/thread_safe_queue.h \
    Toolkit/thread_safe/thread_safe_stack.h \
    Toolkit/observablecollection.h \
    Toolkit/GlobalConsole.h \
    Toolkit/texthelper.h \
    Toolkit/nlogger.h \
    xoWindow.h

INCLUDEPATH += $$PWD/Toolkit

DISTFILES += \
    Application.pri \
    Plugin.pri \
    xoTools.pri \
    Module.pri
