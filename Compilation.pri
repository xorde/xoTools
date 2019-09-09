CONFIG *= c++14
IS_DEBUG = false
COMPILER = ''
PLATFORM = "x86"
win32-msvc*:contains(QMAKE_TARGET.arch, x86_64): {
    CONFIG *= IS_X64
    DEFINES *= IS_X64
    PLATFORM = "x64"
}
win32-g++: {
    COMPILER = 'mingw'
    equals(QMAKE_TARGET.arch, x86): {
        DEFINES *= __MINGW32__
    } else {
        DEFINES *= __MINGW64__
        CONFIG *= IS_X64
        DEFINES *= IS_X64
        PLATFORM = "x64"
    }
}

unix { PLATFORM = $$QMAKE_HOST.arch }

PROJECTNAME = $${TARGET}
CONFIG *= skip_target_version_ext

#Define DEBUG constant
CONFIG(debug,debug|release) {
    TARGET = $$join(TARGET,,,d)
    IS_DEBUG = true
}



win32: {
    win32-msvc* {
        MSVC_VER = $$(VisualStudioVersion)
        equals(MSVC_VER, 10.0){ COMPILER = 'msvc2008' }
        equals(MSVC_VER, 11.0){ COMPILER = 'msvc2010' }
        equals(MSVC_VER, 12.0){ COMPILER = 'msvc2012' }
        equals(MSVC_VER, 13.0){ COMPILER = 'msvc2013' }
        equals(MSVC_VER, 14.0){ COMPILER = 'msvc2015' }
        equals(MSVC_VER, 15.0){ COMPILER = 'msvc2017' }
    }
}
else { COMPILER = $$QMAKE_CXX }

DESTDIR = "$$PWD/../../build-qt$$QT_VERSION-$${COMPILER}-$${PLATFORM}"

DIRS = $${DESTDIR} \
       $${DESTDIR}/debug \
       $${DESTDIR}/release

CONFIG(debug, debug|release): DESTDIR = $$join(DESTDIR,,,/debug)
else:                         DESTDIR = $$join(DESTDIR,,,/release)

ROOTDESTDIR = $${DESTDIR}
DEPENDPATH *= $${DESTDIR}

for(DIR, DIRS) {
     mkcommands += $$OUT_PWD/$$DIR
}

createDirs.commands = $(MKDIR) $$mkcommands
first.depends += createDirs
QMAKE_EXTRA_TARGETS += createDirs
