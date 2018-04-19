cmake_minimum_required(VERSION 3.5)

file(GLOB SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*/*.c

)

file(GLOB HEADER
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/*/*/*/*.hpp
)

file(GLOB QT_UI *.ui)
file(GLOB QT_QRC *.qrc)

add_executable(B_Spline
        ${SOURCE}
        ${HEADER}
        ${QT_UI}
        ${QT_QRC})

target_link_libraries(B_Spline Qt5::Widgets Qt5::OpenGL Qt5::Core Qt5::Gui Qt5::Xml)

target_link_libraries(B_Spline GL)

# QGLViewer-Qt5
target_link_libraries(B_Spline QGLViewer-qt5)
