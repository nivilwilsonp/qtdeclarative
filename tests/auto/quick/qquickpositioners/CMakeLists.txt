# Generated from qquickpositioners.pro.

#####################################################################
## tst_qquickpositioners Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    data/*)
list(APPEND test_data ${test_data_glob})

qt_add_test(tst_qquickpositioners
    SOURCES
        ../../shared/util.cpp ../../shared/util.h
        ../shared/geometrytestutil.cpp ../shared/geometrytestutil.h
        ../shared/viewtestutil.cpp ../shared/viewtestutil.h
        ../shared/visualtestutil.cpp ../shared/visualtestutil.h
        tst_qquickpositioners.cpp
    DEFINES
        QT_QMLTEST_DATADIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/data\\\"
    INCLUDE_DIRECTORIES
        ../../shared
    PUBLIC_LIBRARIES
        Qt::CorePrivate
        Qt::Gui
        Qt::GuiPrivate
        Qt::QmlPrivate
        Qt::QuickPrivate
        Qt::QuickTest
    TESTDATA ${test_data}
)

## Scopes:
#####################################################################

qt_extend_target(tst_qquickpositioners CONDITION ANDROID OR IOS
    DEFINES
        QT_QMLTEST_DATADIR=\\\":/data\\\"
)

qt_extend_target(tst_qquickpositioners CONDITION NOT ANDROID AND NOT IOS
    DEFINES
        QT_QMLTEST_DATADIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/data\\\"
)
