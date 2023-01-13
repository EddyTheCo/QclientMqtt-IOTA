cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(testing VERSION 0.1 DESCRIPTION "diferent tests for the library" LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
qt_standard_project_setup()
include(FetchContent)



qt_add_executable(newoutput newoutput.cpp)


add_test(NAME newoutput COMMAND newoutput wss://api.testnet.shimmer.network:443/api/mqtt/v1 rms1qrzgmpr0lzvqxzu73qakkvg7v2qd2lngkaf64w256c76vepag4sqs27e25s)
target_link_libraries(newoutput PRIVATE qclientMQTT)


