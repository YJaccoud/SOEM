
file(GLOB SOURCES_AP_VALIDATION *.c)

include_directories(/)

add_executable(antonpaar_validation ${SOURCES_AP_VALIDATION})
target_link_libraries(antonpaar_validation soem)

if(WIN32)
   target_link_libraries(antonpaar_validation wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
elseif(UNIX)
   target_link_libraries(antonpaar_validation pthread rt)
endif()

install(TARGETS antonpaar_validation DESTINATION bin)
