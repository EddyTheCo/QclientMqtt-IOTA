cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
    ccommon
    GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
    GIT_TAG main
)
FetchContent_MakeAvailable(ccommon)
version_from_git(
    LOG       OFF
    TIMESTAMP "%Y%m%d%H%M%S"
)

project(QtIotaClient VERSION ${VERSION} DESCRIPTION "IOTA-QT client library for MQTT comunication" LANGUAGES CXX)

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
        STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

find_package(Qt6 COMPONENTS Core OPTIONAL_COMPONENTS WebSockets Mqtt)


if (Qt6_FOUND)
    qt_standard_project_setup()
    if(NOT TARGET Qt6::WebSockets)
        FetchContent_Declare(
            qtwebsockets
            GIT_REPOSITORY git://code.qt.io/qt/qtwebsockets.git
            GIT_TAG 6.5.0
        )
    FetchContent_MakeAvailable(qtwebsockets)
endif()
if(NOT TARGET Qt6::Mqtt)
    FetchContent_Declare(
        qtmqtt
        GIT_REPOSITORY git://code.qt.io/qt/qtmqtt.git
        GIT_TAG dev
    )
FetchContent_MakeAvailable(qtmqtt)
endif()

endif(Qt6_FOUND)

add_library(qclientMQTT include/client/qclientMQTT.hpp qclientMQTT.cpp src/qwebsocketiodevice.cpp include/client/qwebsocketiodevice.hpp)

target_compile_definitions(qclientMQTT PRIVATE WINDOWS_QCMQTT)
FetchContent_Declare(
    qblock
    GIT_REPOSITORY https://github.com/EddyTheCo/Qblock-IOTA.git
    GIT_TAG v0.2.2
    FIND_PACKAGE_ARGS 0 CONFIG
)
FetchContent_MakeAvailable(qblock)

target_link_libraries(qclientMQTT PUBLIC qblock)
if (Qt6_FOUND)
    target_link_libraries(qclientMQTT PUBLIC  Qt6::Core Qt6::Mqtt )
    target_link_libraries(qclientMQTT PRIVATE  Qt6::WebSockets)
endif(Qt6_FOUND)

target_include_directories(qclientMQTT PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    "$<INSTALL_INTERFACE:include>")

install(TARGETS qclientMQTT EXPORT qclientMQTTTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT qclientMQTTTargets
    FILE qclientMQTTTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qclientMQTT
)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/qclientMQTT"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfigVersion.cmake"
    VERSION ${VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qclientMQTT
)
export(EXPORT qclientMQTTTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTTargets.cmake"
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
    if(NOT BUILD_SHARED_LIBS)
        set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-static")
    endif(NOT BUILD_SHARED_LIBS)
    include(CPack)
endif()
#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
#	add_subdirectory(tests)
#endif()
if(BUILD_DOCS)
    get_target_property(build_docs cmake_build_docs SOURCES)
    include(${build_docs})
endif()
