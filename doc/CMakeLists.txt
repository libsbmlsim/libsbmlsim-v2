cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

# installation: docs
file(GLOB_RECURSE doc_files "${LIBSBMLSIM_DOC_DIR}/*")
install(FILES ${doc_files} DESTINATION share/sbmlsim/doc)

