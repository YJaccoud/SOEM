
if (COMPILATION_FOR_INTIME)
	set(CMAKE_SYSTEM_NAME INtime)
	set(CMAKE_GENERATOR_PLATFORM INtime)
endif()

project(SLAVEINFO C)

if (COMPILATION_FOR_INTIME)
	file(GLOB SOURCES_SLAVEINFO intime/ec_master/*.c)	
else()
	set(SOURCES_EEPROMROOL ${OS}/eepromtool/eepromtool.c)
	set(SOURCES_SIMPLETEST ${OS}/simple_test/simple_test.c)
	set(SOURCES_SLAVEINFO  ${OS}/slaveinfo/slaveinfo.c)
endif ()


add_executable(slaveinfo ${SOURCES_SLAVEINFO})
target_link_libraries(slaveinfo soem)

if (NOT COMPILATION_FOR_INTIME)
	add_executable(eepromtool ${SOURCES_EEPROMROOL})
	target_link_libraries(eepromtool soem)

	add_executable(simple_test ${SOURCES_SIMPLETEST})
	target_link_libraries(simple_test soem)
endif ()


if(NOT COMPILATION_FOR_INTIME)
   target_link_libraries(eepromtool  wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
   target_link_libraries(simple_test wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
   target_link_libraries(slaveinfo   wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
endif()


install(TARGETS slaveinfo DESTINATION bin)

if (NOT COMPILATION_FOR_INTIME)
	install(TARGETS eepromtool  DESTINATION bin)
	install(TARGETS simple_test DESTINATION bin)
endif ()