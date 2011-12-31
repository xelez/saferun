#Build sample app

include_directories (${SAFERUN_SOURCE_DIR}/libsaferun)
link_directories (${SAFERUN_BINARY_DIR}/libsaferun)

file(GLOB srun_SOURCES *.c)
add_executable(srun ${srun_SOURCES})
target_link_libraries (srun saferun) 
