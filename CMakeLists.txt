cmake_minimum_required(VERSION 2.8)

set(PROJ_NAME ALMRun)
project(${PROJ_NAME})

# platform
math(EXPR BIT "${CMAKE_SIZEOF_VOID_P} * 8")
if(WIN32)
	set(PLATFORM win${BIT})
elseif(APPLE)
	set(PLATFORM macos)
elseif(UNIX)
	set(PLATFORM linux${BIT})
else()
	message(FATAL_ERROR "The platform isn't supported")
endif()
message(STATUS "Platform - ${PLATFORM}")

# build type
set(ALL_BUILD_TYPE Debug Release)

if(NOT CMAKE_BUILD_TYPE)
	list(GET ALL_BUILD_TYPE 0 BUILD_TYPE_0)
	set(CMAKE_BUILD_TYPE ${BUILD_TYPE_0})
endif()

foreach(BUILD_TYPE ${ALL_BUILD_TYPE})
	string(TOLOWER ${BUILD_TYPE} BUILD_TYPE_LOWER)
	string(TOLOWER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_LOWER)
	if(${BUILD_TYPE_LOWER} STREQUAL ${CMAKE_BUILD_TYPE_LOWER})
		set(IS_VALID_BUILD_TYPE 1)
	endif()
endforeach()
if(NOT IS_VALID_BUILD_TYPE)
	message(FATAL_ERROR "Build type ${CMAKE_BUILD_TYPE} isn't supported")
endif()

message(STATUS "Build type (Makefile) - ${CMAKE_BUILD_TYPE}")

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# add executable
aux_source_directory(src SRCS)
aux_source_directory(src/Dialog SRCS)
include_directories(src/Dialog)
include_directories(src)
if(NOT APPLE)
	file(GLOB OBJC_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.mm)
	list(REMOVE_ITEM SRCS ${OBJC_SRCS})
endif()

if(WIN32)
	add_executable(${PROJ_NAME} WIN32 ${SRCS} resource/merry.rc)
else(APPLE)
	add_executable(${PROJ_NAME} MACOSX_BUNDLE ${SRCS})
else()
	add_executable(${PROJ_NAME} ${SRCS})
endif()

# add third party libraries
file(GLOB ALL_THIRD_PARTY_PATH third_party/*)
foreach(THIRD_PARTY_PATH ${ALL_THIRD_PARTY_PATH})
	if(NOT (${THIRD_PARTY_PATH} MATCHES "\\.svn$"))
		include_directories(${THIRD_PARTY_PATH}/${PLATFORM}/include)

		foreach(BUILD_TYPE ${ALL_BUILD_TYPE})
			string(TOLOWER ${BUILD_TYPE} BUILD_TYPE_LOWER)
			set(THIRD_PARTY_LIB_PATH ${THIRD_PARTY_PATH}/${PLATFORM}/${BUILD_TYPE_LOWER})
			file(GLOB ALL_LIB_PATH ${THIRD_PARTY_LIB_PATH}/*.a ${THIRD_PARTY_LIB_PATH}/*.lib)
			foreach(LIB_PATH ${ALL_LIB_PATH})
				if(${BUILD_TYPE_LOWER} STREQUAL debug)
					target_link_libraries(${PROJ_NAME} debug ${LIB_PATH})
				else()
					target_link_libraries(${PROJ_NAME} optimized ${LIB_PATH})
				endif()
			endforeach()
		endforeach()
	endif()
endforeach()

if(UNIX)
	find_package(wxWidgets REQUIRED)
	include(${wxWidgets_USE_FILE})
	target_link_libraries(${PROJ_NAME} ${wxWidgets_LIBRARIES})
endif()

# set flags
if(WIN32)
	foreach(FLAG_VAR
			CMAKE_CXX_FLAGS
			CMAKE_CXX_FLAGS_DEBUG
			CMAKE_CXX_FLAGS_RELEASE
			CMAKE_CXX_FLAGS_MINSIZEREL
			CMAKE_CXX_FLAGS_RELWITHDEBINFO)
		if(${FLAG_VAR} MATCHES "/MD")
			string(REGEX REPLACE "/MD" "/MT" ${FLAG_VAR} "${${FLAG_VAR}}")
		endif()
	endforeach()

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_UNICODE -DUNICODE /wd4996")
	target_link_libraries(${PROJ_NAME} comctl32 rpcrt4)
endif()

if(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

# configuration types
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES ${ALL_BUILD_TYPE})
	set(CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES} CACHE STRING
		"Configuration types" FORCE)
	message(STATUS "Configuration types - ${CMAKE_CONFIGURATION_TYPES}")
endif()
