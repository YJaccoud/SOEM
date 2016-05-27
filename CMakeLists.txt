cmake_minimum_required(VERSION 3.1.0)
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake/Modules")
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/CMakeINtime")
#/Platform")

if (COMPILATION_FOR_INTIME)
	set(CMAKE_SYSTEM_NAME INtime)
	set(CMAKE_GENERATOR_PLATFORM INtime)
endif()

project(SOEM C)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(SOEM_INCLUDE_INSTALL_DIR include)

message("CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")

if(COMPILATION_FOR_INTIME)
   set(OS "intime")
else()
   set(OS "win32")
   include_directories(oshw/win32/wpcap/Include)
   link_directories(${CMAKE_SOURCE_DIR}/oshw/win32/wpcap/Lib)
endif()

message("OS is ${OS}")

include_directories(soem)
include_directories(osal)
include_directories(osal/${OS})
include_directories(oshw/${OS})

if (MSVC)
  set(CMAKE_C_FLAGS_RELEASE "/D _CRT_SECURE_NO_WARNINGS /EHsc")
  set(CMAKE_C_FLAGS_DEBUG "/D _CRT_SECURE_NO_WARNINGS /EHsc /ZI /Od")
endif()

file(GLOB SOEM_SOURCES soem/*.c)
file(GLOB OSAL_SOURCES osal/${OS}/*.c)
file(GLOB OSHW_SOURCES oshw/${OS}/*.c)

#if(WIN32)
#  set(SOEM_SOURCES ${SOEM_SOURCES})
#endif()

file(GLOB SOEM_HEADERS soem/*.h)
file(GLOB OSAL_HEADERS osal/osal.h osal/${OS}/*.h)
file(GLOB OSHW_HEADERS oshw/${OS}/*.h)

add_library(soem STATIC ${SOEM_SOURCES} ${OSAL_SOURCES} ${OSHW_SOURCES})
if(NOT COMPILATION_FOR_INTIME)
  target_link_libraries(soem wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)	#${HDF5_LIBRARIES}
endif()

install(TARGETS soem DESTINATION lib)
install(FILES ${SOEM_HEADERS} ${OSAL_HEADERS} ${OSHW_HEADERS} DESTINATION ${SOEM_INCLUDE_INSTALL_DIR})

add_subdirectory(test)