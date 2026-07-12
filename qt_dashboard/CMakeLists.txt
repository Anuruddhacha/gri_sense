cmake_minimum_required(VERSION 3.16)
project(agri_sense_dash LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

include(FetchContent)

set(PAHO_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(PAHO_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PAHO_WITH_SSL OFF CACHE BOOL "" FORCE)
set(PAHO_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(PAHO_BUILD_DOCUMENTATION OFF CACHE BOOL "" FORCE)
set(PAHO_ENABLE_CPACK OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    paho_mqtt_c
    GIT_REPOSITORY https://github.com/eclipse/paho.mqtt.c.git
    GIT_TAG        v1.3.13
)
FetchContent_MakeAvailable(paho_mqtt_c)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(APP_SOURCES
    src/main.cpp
    src/core/TelemetryParser.cpp
    src/mqtt/MqttSubscriber.cpp
    src/model/TelemetryModel.cpp
    src/ui/SensorTile.cpp
    src/ui/MainWindow.cpp
)

add_executable(agri_sense_dash ${APP_SOURCES})

target_include_directories(agri_sense_dash PRIVATE
    src
    ${paho_mqtt_c_SOURCE_DIR}/src
)

target_compile_definitions(agri_sense_dash PRIVATE MQTTCLIENT_STATIC)
target_link_libraries(agri_sense_dash PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    paho-mqtt3c-static
)

if(WIN32)
    target_link_libraries(agri_sense_dash PRIVATE ws2_32)
endif()
