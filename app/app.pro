include(../defaults.pri)

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lcircles
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lcircles
else:unix: LIBS += -L$$OUT_PWD/../src/ -lcircles

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src
