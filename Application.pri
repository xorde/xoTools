include("Module.pri")
TEMPLATE = app
QT *= core
DESTDIR = $$join(DESTDIR,,,/xoModules/$$NAME)

win32: EXTENSION = .exe
win32: XO_TOOLS_EXTENSION = .dll

win32: CONFIG(debug, debug|release): SUFFIX = d

QMAKE_POST_LINK += xcopy /Y $$shell_path($${DESTDIR}/../../xoTools$${SUFFIX}$${XO_TOOLS_EXTENSION}) $$shell_path($${DESTDIR}/) &
QMAKE_POST_LINK += windeployqt $${DESTDIR}/$${TARGET}$${EXTENSION} &
