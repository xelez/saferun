file(GLOB saferun_SOURCES *.cpp)

add_library(saferun ${saferun_SOURCES})
target_link_libraries(saferun cap)
#linking libcap
