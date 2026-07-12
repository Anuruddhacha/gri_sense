cmake_minimum_required(VERSION 3.16)
project(agri_sense_dash LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

set(PAHO_BUILD_STATIC ON CACHE BOOL "" FORCE)
set(PAHO_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PAHO_WITH_SSL OFF CACHE BOOL "" FORCE)
set(PAHO_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(PAHO_BUILD_DOCUMENTATION OFF CACHE BOOL "" FORCE)
set(PAHO_ENABLE_CPACK OFF CACHE BOOL "" FORCE)

set(PAHO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/paho.mqtt.c")
if(NOT EXISTS "${PAHO_SRC_DIR}/CMakeLists.txt")
    message(FATAL_ERROR
        "Missing ${PAHO_SRC_DIR}. Run build.bat once to clone Paho, "
        "or: git clone --depth 1 --branch v1.3.13 https://github.com/eclipse/paho.mqtt.c.git third_party/paho.mqtt.c")
endif()

add_subdirectory("${PAHO_SRC_DIR}" "${CMAKE_BINARY_DIR}/paho_mqtt_c" EXCLUDE_FROM_ALL)

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
    "${PAHO_SRC_DIR}/src"
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
