cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(local_conf.cmake OPTIONAL)
project(QtIotaClient VERSION 0.1 DESCRIPTION "IOTA-QT client library" LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
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
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(qclientMQTT include/client/qclientMQTT.hpp qclientMQTT.cpp src/qwebsocketiodevice.cpp include/client/qwebsocketiodevice.hpp)
include(FetchContent)
FetchContent_Declare(
        qblock
        GIT_REPOSITORY git@github.com:EddyTheCo/Qblock-IOTA.git
        GIT_TAG main
        )
FetchContent_MakeAvailable(qblock)

target_link_libraries(qclientMQTT PUBLIC qblock)
find_package(Qt6 REQUIRED COMPONENTS Core Mqtt WebSockets)
target_link_libraries(qclientMQTT PUBLIC  Qt6::Core Qt6::Mqtt )
target_link_libraries(qclientMQTT PRIVATE  Qt6::WebSockets)

target_include_directories(qclientMQTT PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:include>")

install(TARGETS qclientMQTT EXPORT qclientMQTTTargets DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/)

install(EXPORT qclientMQTTTargets
	FILE qclientMQTTTargets.cmake
	DESTINATION lib/cmake/qclientMQTT
	)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfig.cmake"
	INSTALL_DESTINATION "lib/cmake/qclientMQTT"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfigVersion.cmake"
	VERSION "0.1.1.0"
	COMPATIBILITY AnyNewerVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTConfigVersion.cmake
	DESTINATION lib/cmake/qclientMQTT
	)
export(EXPORT qclientMQTTTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/qclientMQTTTargets.cmake"
	)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
