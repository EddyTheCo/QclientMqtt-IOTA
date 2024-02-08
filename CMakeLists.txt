cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
    ccommon
    GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
    GIT_TAG v0.1.1
)
FetchContent_MakeAvailable(ccommon)
version_from_git(
    LOG       OFF
    TIMESTAMP "%Y%m%d%H%M%S"
)

project(IotaMqttClient VERSION ${VERSION} DESCRIPTION "Methods to comunicate with the Event API of Iota nodes" LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

FetchContent_Declare(
    qblock
    GIT_REPOSITORY https://github.com/EddyTheCo/Qblock-IOTA.git
    GIT_TAG v0.3.2
    FIND_PACKAGE_ARGS 0.3 CONFIG
)
FetchContent_MakeAvailable(qblock)

find_package(Qt6 COMPONENTS Core OPTIONAL_COMPONENTS WebSockets Mqtt)


if (Qt6_FOUND)
    qt_standard_project_setup()
    if(NOT TARGET Qt6::WebSockets)
        FetchContent_Declare(
            qtwebsockets
            GIT_REPOSITORY git://code.qt.io/qt/qtwebsockets.git
            GIT_TAG 6.6.0
        )
    FetchContent_MakeAvailable(qtwebsockets)
endif()
if(NOT TARGET Qt6::Mqtt)
    FetchContent_Declare(
        qtmqtt
        GIT_REPOSITORY git://code.qt.io/qt/qtmqtt.git
        GIT_TAG 6.6.0
    )
FetchContent_MakeAvailable(qtmqtt)
endif()

endif(Qt6_FOUND)

add_library(clientMQTT include/client/qclientMQTT.hpp qclientMQTT.cpp src/qwebsocketiodevice.cpp include/client/qwebsocketiodevice.hpp)
add_library(${PROJECT_NAME}::clientMQTT ALIAS clientMQTT)

target_compile_definitions(clientMQTT PRIVATE WINDOWS_QCMQTT)


target_link_libraries(clientMQTT PUBLIC qblock)
if (Qt6_FOUND)
    target_link_libraries(clientMQTT PUBLIC  Qt6::Core Qt6::Mqtt )
    target_link_libraries(clientMQTT PRIVATE  Qt6::WebSockets)
endif(Qt6_FOUND)

target_include_directories(clientMQTT PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/${PROJECT_NAME}>")

install(TARGETS clientMQTT
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT clientMQTT
)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv/${PROJECT_NAME}
    COMPONENT clientMQTT
)

install(EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    COMPONENT ${PROJECT_NAME}
)
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION ${VERSION}
        COMPATIBILITY SameMajorVersion
        )
install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        )
export(EXPORT ${PROJECT_NAME}-config
        FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        )

get_cmake_property(CPACK_COMPONENTS_ALL_VAR COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL_VAR "_install_html_docs_Mqtt" "_install_qch_docs_Mqtt")
set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL_VAR} CACHE INTERNAL "" )

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CPACK_PACKAGE_CONTACT "estervtech")
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
