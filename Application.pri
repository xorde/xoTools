include("Module.pri")
TEMPLATE = app
QT *= core
DESTDIR = $$join(DESTDIR,,,/xoModules/$$NAME)

win32:{
    EXTENSION = .exe
    XO_TOOLS_EXTENSION = .dll
    CONFIG(debug, debug|release): SUFFIX = d

    equals(TEMPLATE, "app") : EXTENSION = .exe
    equals(TEMPLATE, "lib") : EXTENSION = .dll
    QMAKE_POST_LINK += xcopy /Y $$shell_path($${DESTDIR}/../../xoTools$${SUFFIX}$${XO_TOOLS_EXTENSION}) $$shell_path($${DESTDIR}/) &

    DEPLOY_COMMAND = windeployqt

    QMAKE_POST_LINK += $${DEPLOY_COMMAND} $${DESTDIR}/$${TARGET}$${EXTENSION} &
    QMAKE_POST_LINK += $${DEPLOY_COMMAND} $${DESTDIR}/xoTools$${SUFFIX}$${XO_TOOLS_EXTENSION} &

}

macx:{
    equals(TEMPLATE, "app") : EXTENSION = .app
    equals(TEMPLATE, "lib") : EXTENSION = .dylib

    DEPLOY_COMMAND = macdeployqt
}


