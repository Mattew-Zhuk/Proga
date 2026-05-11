cmake_minimum_required(VERSION 3.16)

project(lab2 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

set(COMMON_SOURCES
    meterreading.cpp meterreading.h
    meterreadingmodel.cpp meterreadingmodel.h
    logger.cpp logger.h
    conditionparser.cpp conditionparser.h
    commandprocessor.cpp commandprocessor.h
)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h
    firstwindow.cpp firstwindow.h
    helpwindow.cpp helpwindow.h
    ${COMMON_SOURCES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(lab2 MANUAL_FINALIZATION ${PROJECT_SOURCES})
else()
    add_executable(lab2 ${PROJECT_SOURCES})
endif()

target_link_libraries(lab2 PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set(TEST_SOURCES
    test_meterreadingmodel.cpp
    ${COMMON_SOURCES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_model_runner ${TEST_SOURCES})
else()
    add_executable(test_model_runner ${TEST_SOURCES})
endif()
target_link_libraries(test_model_runner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

set(UI_TEST_SOURCES
    test_ui.cpp
    mainwindow.cpp mainwindow.h
    firstwindow.cpp firstwindow.h
    helpwindow.cpp helpwindow.h
    ${COMMON_SOURCES}
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(test_ui_runner ${UI_TEST_SOURCES})
else()
    add_executable(test_ui_runner ${UI_TEST_SOURCES})
endif()
target_link_libraries(test_ui_runner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

add_executable(test_conditionparser test_conditionparser.cpp ${COMMON_SOURCES})
target_link_libraries(test_conditionparser PRIVATE Qt${QT_VERSION_MAJOR}::Test)
add_executable(test_commandprocessor test_commandprocessor.cpp ${COMMON_SOURCES})
target_link_libraries(test_commandprocessor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Test)

if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.lab2)
endif()
set_target_properties(lab2 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS lab2
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lab2)
endif()

enable_testing()
add_test(NAME MeterReadingModelTests COMMAND test_model_runner)
add_test(NAME UITests COMMAND test_ui_runner)
add_test(NAME ConditionParserTests COMMAND test_conditionparser)
add_test(NAME CommandProcessorTests COMMAND test_commandprocessor)
