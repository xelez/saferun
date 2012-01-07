#
# This is root build script for libsaferun and srun
#

cmake_minimum_required (VERSION 2.6)
project (SAFERUN)

#set(SAFERUN_VERSION "0.1-GIT")
execute_process(
    COMMAND "${SAFERUN_SOURCE_DIR}/GIT-VERSION-GEN"
    WORKING_DIRECTORY "${SAFERUN_SOURCE_DIR}"
    OUTPUT_VARIABLE SAFERUN_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

set(BUILD_SHARED_LIBS true)
option(USE_PROFILING "Compile in profiling output" Off)

add_subdirectory(libsaferun)
add_subdirectory(srun)

# Packaging
# All in one for now
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/")

set(CPACK_PACKAGE_VENDOR "Alex Ankudinov")
set(CPACK_PACKAGE_CONTACT "Alex Ankudinov <xelez0@gmail.com>")

set(CPACK_PACKAGE_NAME "saferun")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "library for safely execution of programs with resource limiting")
set(CPACK_PACKAGE_VERSION "${SAFERUN_VERSION}")

#set(CPACK_BINARY_DEB "on")
#set(CPACK_SOURCE_DEB "on")
set(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_SECTION "libs")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.8), libcap2 (>= 2.10)")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Library for safely execution and resource limiting
 Saferun is a library for safely exection of programs
 with limiting of execution time and memory usage.
 .
 SRun is a sample app that uses this library.
 .
 Homepage: http://github.com/xelez/saferun/")

set(CPACK_PACKAGE_DESCRIPTION_FILE "${SAFERUN_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${SAFERUN_SOURCE_DIR}/COPYING")

set (CPACK_SOURCE_IGNORE_FILES "\\/_;/build/;/CVS/;/\\.svn/;/\\.bzr/;/\\.hg/;/\\.git/;\\.swp$;\\.#;/#" CACHE STRING "CPACK will ignore this files" )
include(CPack)
