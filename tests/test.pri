CONFIG += C++11
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../src/release/ -lcpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../src/debug/ -lcpp
else:unix: LIBS += -L$$OUT_PWD/../src/ -lcpp

INCLUDEPATH += $$PWD/../src
DEPENDPATH += $$PWD/../src

DEFINES += TEST_MATERIAL_PATH=$$quote(\\\"$$PWD/../additionMaterials\\\")
