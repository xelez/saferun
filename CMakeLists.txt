#
# This is root build script for libsaferun and srun
#

cmake_minimum_required (VERSION 2.6)
project (SAFERUN)

SET(SAFERUN_VERSION "0.1-GIT")

#set(CMAKE_BUILD_TYPE Release)
set(BUILD_SHARED_LIBS true)

add_subdirectory(libsaferun)
add_subdirectory(srun)

