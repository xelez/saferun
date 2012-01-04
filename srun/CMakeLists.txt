#Build sample app

configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${SAFERUN_SOURCE_DIR}/libsaferun)
link_directories (${SAFERUN_BINARY_DIR}/libsaferun)

find_package(PkgConfig)
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.6)
link_directories(${GLIB_LIBRARY_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})


file(GLOB srun_SOURCES *.c)
add_executable(srun ${srun_SOURCES})
target_link_libraries (srun saferun)
target_link_libraries (srun ${GLIB_LIBRARIES})


install(TARGETS srun DESTINATION sbin)
