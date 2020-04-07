include("Module.pri")
TEMPLATE = app
QT *= core
DESTDIR = $$join(DESTDIR,,,/xoModules/$$NAME)

win32: {
    EXTENSION = .exe
}

QMAKE_POST_LINK += windeployqt $${DESTDIR}/$${TARGET}$${EXTENSION}
