include(../defaults.pri)
TEMPLATE = app

# QT += testlib

SOURCES += \
    main.cpp \
    graph/testEdge.cpp \
    graph/testGraph.cpp \
    graph/testTriangle.cpp

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

HEADERS += \
    include/gtest/internal/custom/gtest-port.h \
    include/gtest/internal/custom/gtest-printers.h \
    include/gtest/internal/custom/gtest.h \
    include/gtest/internal/gtest-death-test-internal.h \
    include/gtest/internal/gtest-filepath.h \
    include/gtest/internal/gtest-internal.h \
    include/gtest/internal/gtest-linked_ptr.h \
    include/gtest/internal/gtest-param-util-generated.h \
    include/gtest/internal/gtest-param-util.h \
    include/gtest/internal/gtest-port-arch.h \
    include/gtest/internal/gtest-port.h \
    include/gtest/internal/gtest-string.h \
    include/gtest/internal/gtest-tuple.h \
    include/gtest/internal/gtest-type-util.h \
    include/gtest/gtest-death-test.h \
    include/gtest/gtest-message.h \
    include/gtest/gtest-param-test.h \
    include/gtest/gtest-printers.h \
    include/gtest/gtest-spi.h \
    include/gtest/gtest-test-part.h \
    include/gtest/gtest-typed-test.h \
    include/gtest/gtest.h \
    include/gtest/gtest_pred_impl.h \
    include/gtest/gtest_prod.h
