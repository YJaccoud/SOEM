cmake_minimum_required(VERSION 2.8.4)
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake/Modules")
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_CURRENT_SOURCE_DIR}/CMakeINtime")

if (COMPILATION_FOR_INTIME)
   set(CMAKE_SYSTEM_NAME INtime)
   set(CMAKE_GENERATOR_PLATFORM INtime)
endif()

project(SOEM C)

if(DEFINED HOST_INSTALL)
   set(SOEM_INCLUDE_INSTALL_DIR include/soem)
else()
   set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
   set(SOEM_INCLUDE_INSTALL_DIR include)
endif()

message("CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")

if(WIN32)   
   set(OS "win32")
   include_directories(${CMAKE_SOURCE_DIR}/oshw/win32/wpcap/Include)
   link_directories(${CMAKE_SOURCE_DIR}/oshw/win32/wpcap/Lib)
elseif(COMPILATION_FOR_INTIME)
   set(OS "intime")
elseif(UNIX)
   set(OS "linux")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "rt-kernel")
   set(OS "rtk")
   message("ARCH is ${ARCH}")
   message("BSP is ${BSP}")
   include_directories(oshw/${OS}/${ARCH})
   add_definitions("-Wno-unused-but-set-variable")
   add_definitions("-Wno-unused-function")
   add_definitions("-Wno-format")
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

file(GLOB SOEM_HEADERS soem/*.h)
file(GLOB OSAL_HEADERS osal/osal.h osal/${OS}/*.h)
file(GLOB OSHW_HEADERS oshw/${OS}/*.h)

if(${CMAKE_SYSTEM_NAME} MATCHES "rt-kernel")
  file(GLOB OSHW_ARCHSOURCES oshw/${OS}/${ARCH}/*.c)
endif()

add_library(soem STATIC ${SOEM_SOURCES} ${OSAL_SOURCES} ${OSHW_SOURCES} ${OSHW_ARCHSOURCES})
if(WIN32)
   target_link_libraries(soem wpcap.lib Packet.lib Ws2_32.lib Winmm.lib ${HDF5_LIBRARIES})
elseif(UNIX)
  target_link_libraries(soem pthread rt)
endif()

install(TARGETS soem DESTINATION lib)
install(FILES ${SOEM_HEADERS} ${OSAL_HEADERS} ${OSHW_HEADERS} DESTINATION ${SOEM_INCLUDE_INSTALL_DIR})

add_subdirectory(test)
add_subdirectory(test/linux/antonpaar_validation)

