cmake_minimum_required(VERSION 3.24)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG use_IFW
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
	QtIotaBlock	
	GIT_REPOSITORY https://github.com/EddyTheCo/Qblock-IOTA.git
	GIT_TAG use_IFW
	FIND_PACKAGE_ARGS 0.3 CONFIG
	)
FetchContent_MakeAvailable(QtIotaBlock)

find_package(Qt6 COMPONENTS Core OPTIONAL_COMPONENTS WebSockets Mqtt)


if (Qt6_FOUND)
	qt_standard_project_setup()
	if(NOT TARGET Qt6::WebSockets)
		FetchContent_Declare(
			qtwebsockets
			GIT_REPOSITORY https://github.com/qt/qtwebsockets.git
			GIT_TAG 6.6.0
			)
		FetchContent_MakeAvailable(qtwebsockets)
	endif()
	if(NOT TARGET Qt6::Mqtt)
		FetchContent_Declare(
			qtmqtt
			GIT_REPOSITORY https://github.com/qt/qtmqtt.git
			GIT_TAG 6.6.0
			)
		FetchContent_MakeAvailable(qtmqtt)
	endif()


	add_library(qclientMQTT include/client/qclientMQTT.hpp qclientMQTT.cpp src/qwebsocketiodevice.cpp include/client/qwebsocketiodevice.hpp)
	add_library(${PROJECT_NAME}::qclientMQTT ALIAS qclientMQTT)
	set_target_properties(qclientMQTT PROPERTIES VERSION ${VERSION} SOVERSION ${VERSION_MAJOR})

	target_compile_definitions(qclientMQTT PRIVATE WINDOWS_QCMQTT)


	target_link_libraries(qclientMQTT PUBLIC QtIotaBlock::qblock)
	target_link_libraries(qclientMQTT PUBLIC  Qt6::Core Qt6::Mqtt )
	target_link_libraries(qclientMQTT PRIVATE  Qt6::WebSockets)

	target_include_directories(qclientMQTT PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")

	install(TARGETS qclientMQTT
		EXPORT ${PROJECT_NAME}-config
		DESTINATION ${CMAKE_INSTALL_LIBDIR}
		COMPONENT clientMQTT
		)
	install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv
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
		COMPONENT clientMQTT
		)
	export(EXPORT ${PROJECT_NAME}-config
		FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
		)

	get_cmake_property(CPACK_COMPONENTS_ALL_VAR COMPONENTS)
	list(REMOVE_ITEM CPACK_COMPONENTS_ALL_VAR "_install_html_docs_Mqtt" "_install_qch_docs_Mqtt")
	set(CPACK_COMPONENTS_ALL ${CPACK_COMPONENTS_ALL_VAR} CACHE INTERNAL "" )

	if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
		set(cpack_file_name "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
		if(NOT BUILD_SHARED_LIBS)
			set(cpack_file_name "${cpack_file_name}-static")
		endif(NOT BUILD_SHARED_LIBS)
		set(CPACK_PACKAGE_FILE_NAME ${cpack_file_name})
		include(CPack)
		include(CPackIFW)
		cpack_add_component(clientMQTT)
		cpack_ifw_configure_component(clientMQTT LICENSES "License" ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
		cpack_ifw_add_repository("Repo" URL "${REPO_URL}/packages/${CMAKE_SYSTEM_NAME}/" )
	endif()
	#if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	#	include(CTest)
	#    add_subdirectory(tests)
	#endif()
endif(Qt6_FOUND)
if(BUILD_DOCS)
	build_docs()
endif()
