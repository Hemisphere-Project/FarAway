set(CORE_SRCS
    main.cpp
)

# Timer
FILE(GLOB CPPsources lib/Timer/*.cpp)
set(TIMER_SRCS
    ${CPPsources}
)
message(STATUS "TIMER_SRCS include: ${TIMER_SRCS}")

# RemoteDebug
FILE(GLOB_RECURSE CPPsources lib/RemoteDebug/src/*.cpp)
set(REMOTE_SRCS
    ${CPPsources}
)
message(STATUS "REMOTE_SRCS include: ${REMOTE_SRCS}")

# K32
FILE(GLOB_RECURSE CPPsources lib/K32-lite/*.cpp)
set(K32_SRCS
    ${CPPsources}
)
message(STATUS "K32_SRCS include: ${K32_SRCS}")


set(COMPONENT_SRCS ${CORE_SRCS} ${TIMER_SRCS} ${REMOTE_SRCS} ${K32_SRCS})
set(COMPONENT_ADD_INCLUDEDIRS "")

register_component()