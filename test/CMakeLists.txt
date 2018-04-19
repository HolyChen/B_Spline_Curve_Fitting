cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SOURCE)
file(GLOB TEST_HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)

add_executable(BSplineFunction_Tester ${TEST_SOURCE})

target_link_libraries(BSplineFunction_Tester ${GTEST_BOTH_LIBRARIES} pthread)

target_link_libraries(BSplineFunction_Tester Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui Qt5::Xml GL QGLViewer-qt5)

add_test(NAME unit_test
        COMMAND BSplineFunction_Tester)