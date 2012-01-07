file(GLOB saferun_SOURCES *.cpp)

add_library(saferun ${saferun_SOURCES})

#link libcap
target_link_libraries(saferun cap)

if(USE_PROFILING)
    add_definitions(-DUSE_PROFILING)
endif(USE_PROFILING)

install(TARGETS saferun DESTINATION lib)
