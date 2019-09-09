QT      *= core
NAME = $$TARGET

include("Compilation.pri")
include("xoTools.pri")

unix {
    target.path = /usr/lib
    INSTALLS += target
}
