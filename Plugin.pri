TEMPLATE = lib
CONFIG  *= plugin

include("Module.pri")

DESTDIR = $$join(DESTDIR,,,/xoPlugins/$${NAME})
