include (UseCython)

include_directories (${SAFERUN_SOURCE_DIR}/libsaferun)
link_directories (${SAFERUN_BINARY_DIR}/libsaferun)

cython_add_module (pysaferun pysaferun.pyx)
target_link_libraries (pysaferun saferun)

#TODO: maybe write something more useful and less stupid
install(TARGETS pysaferun DESTINATION lib/python2.6/dist-packages)
install(TARGETS pysaferun DESTINATION lib/python2.7/dist-packages)
