file(GLOB saferun_SOURCES *.cpp)

add_library(saferun ${saferun_SOURCES})

#link libcap
target_link_libraries(saferun cap)

install(TARGETS saferun DESTINATION lib)
