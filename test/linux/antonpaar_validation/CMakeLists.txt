
file(GLOB SOURCES_AP_VALIDATION *.c)

include_directories(/)

add_executable(ap_validation ${SOURCES_AP_VALIDATION})
target_link_libraries(ap_validation soem)

if(WIN32)
   target_link_libraries(ap_validation wpcap.lib Packet.lib Ws2_32.lib Winmm.lib)
elseif(UNIX)
   target_link_libraries(ap_validation pthread rt)
endif()

install(TARGETS ap_validation DESTINATION bin)
