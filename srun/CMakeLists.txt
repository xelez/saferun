#Build sample app

configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${SAFERUN_SOURCE_DIR}/libsaferun)
link_directories (${SAFERUN_BINARY_DIR}/libsaferun)

file(GLOB srun_SOURCES *.c)
add_executable(srun ${srun_SOURCES})
target_link_libraries (srun saferun)

install(TARGETS srun DESTINATION sbin)
