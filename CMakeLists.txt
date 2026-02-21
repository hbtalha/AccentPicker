cmake_minimum_required(VERSION 3.16)

project(AccentPicker VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

set(RESOURCES assets/resources.qrc)

qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

find_package(X11 REQUIRED)
if(NOT X11_XTest_FOUND)
    message(FATAL_ERROR "XTest extension not found")
endif()
if(NOT X11_Xext_FOUND)
    message(FATAL_ERROR "Xext extension not found")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${X11_LIBRARIES}
    ${X11_XTest_LIB}
    ${X11_Xext_LIB}
)


if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()


# Set default install prefix if not specified
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "/opt/HBatalha/AccentPicker" CACHE PATH "Install prefix" FORCE)
endif()
# Set the output name to lowercase
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME accentpicker)




# Set the output name to lowercase
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME accentpicker)

# Install binary to root of install prefix
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
