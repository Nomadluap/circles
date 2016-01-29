include(../defaults.pri)
TEMPLATE = app

# QT += testlib

SOURCES += \
    testGraph.cpp \
    main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../src/release/ -lcircles
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/debug/ -lcircles
else:unix: LIBS += -L$$OUT_PWD/../src/ -lcircles

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../googletest/googletest/ -lgtest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../googletest/googletest/ -lgtestd
else:unix: LIBS += -L$$PWD/../../googletest/googletest/ -lgtest



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lgtest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lgtestd
else:unix: LIBS += -L$$PWD/lib/ -lgtest

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libgtest.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libgtestd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/gtest.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/gtestd.lib
else:unix: PRE_TARGETDEPS += $$PWD/lib/libgtest.a
