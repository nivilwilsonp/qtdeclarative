# Generated from qquickgraphicsinfo.pro.

#####################################################################
## tst_qquickgraphicsinfo Test:
#####################################################################

# Collect test data
file(GLOB_RECURSE test_data_glob
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    data/*)
list(APPEND test_data ${test_data_glob})

qt_add_test(tst_qquickgraphicsinfo
    SOURCES
        ../../shared/util.cpp ../../shared/util.h
        tst_qquickgraphicsinfo.cpp
    INCLUDE_DIRECTORIES
        ../../shared
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Quick
    TESTDATA ${test_data}
)

#### Keys ignored in scope 1:.:.:qquickgraphicsinfo.pro:<TRUE>:
# OTHER_FILES = "data/basic.qml"

## Scopes:
#####################################################################

qt_extend_target(tst_qquickgraphicsinfo CONDITION ANDROID OR IOS
    DEFINES
        QT_QMLTEST_DATADIR=\\\":/data\\\"
)

qt_extend_target(tst_qquickgraphicsinfo CONDITION NOT ANDROID AND NOT IOS
    DEFINES
        QT_QMLTEST_DATADIR=\\\"${CMAKE_CURRENT_SOURCE_DIR}/data\\\"
)
