set(ebusctl_SOURCES ebusctl.cpp)
set(ebuspicloader_SOURCES ebuspicloader.cpp intelhex/intelhexclass.cpp)
set(ebusfeed_SOURCES ebusfeed.cpp)
set(ebusfeed_LIBS ${ebusfeed_LIBS} ebus)

include_directories(../lib/ebus)
include_directories(../lib/utils)
include_directories(intelhex)

add_executable(ebusctl ${ebusctl_SOURCES})
add_executable(ebuspicloader ${ebuspicloader_SOURCES})
add_executable(ebusfeed ${ebusfeed_SOURCES})
target_link_libraries(ebusctl utils ebus ${LIB_ARGP} ${ebusctl_LIBS})
target_link_libraries(ebuspicloader ${LIB_ARGP})
target_link_libraries(ebusfeed ${LIB_ARGP} ${ebusfeed_LIBS})

install(TARGETS ebusctl ebuspicloader ebusfeed EXPORT ebusd DESTINATION usr/bin)
