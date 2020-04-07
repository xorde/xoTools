include("Module.pri")
TEMPLATE = lib
CONFIG  *= plugin
DESTDIR = $$join(DESTDIR,,,/xoPlugins/$${NAME})

win32: EXTENSION = .dll

win32: CONFIG(debug, debug|release): SUFFIX = d

QMAKE_POST_LINK += xcopy /Y $$shell_path($${DESTDIR}/../../xoTools$${SUFFIX}$${EXTENSION}) $$shell_path($${DESTDIR}/) &
QMAKE_POST_LINK += windeployqt $${DESTDIR}/$${TARGET}$${EXTENSION} &
