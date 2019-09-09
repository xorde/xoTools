TOSDIR  = $${DESTDIR}
TOSDIRH = "$$PWD/../xoTools/"
TOSLIB  = "xoTools"

CONFIG(debug, debug|release): TOSLIB = $$join(TOSLIB,,,d)
LIBS *= -L$${TOSDIR} -l$${TOSLIB}

INCLUDEPATH *= $${TOSDIRH}Toolkit/
INCLUDEPATH *= $${TOSDIRH}
INCLUDEPATH *= $$PWD
