include("Module.pri")
TEMPLATE = lib
CONFIG  *= plugin
DESTDIR = $$join(DESTDIR,,,/xoPlugins/$${NAME})

win32: {
    EXTENSION = .dll
}

QMAKE_POST_LINK += windeployqt $${DESTDIR}/$${TARGET}$${EXTENSION}
