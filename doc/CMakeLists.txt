
# NOTE: The order of this list determines the order of items in the Guides
#       (i.e. Pages) list in the generated documentation
set(source_files
 ../examples/b64.cxx
 ../examples/buildidx.cxx
 ../examples/catpart.cxx
 ../examples/exbin.cxx
 ../examples/mbox.cxx
 ../examples/qp.cxx
 ../examples/search.cxx
 ../examples/smime2xml.cxx
 ../examples/smimestruct.cxx
 ../examples/structure.cxx
 )

set(extra_files  header.html footer.html)

#set(header_paths
#    "${mimetic_SOURCE_DIR}/mimetic/mimetic.h"
#    )
file(GLOB_RECURSE header_paths CONFIGURE_DEPENDS "${mimetic_SOURCE_DIR}/mimetic/*.h")
# Format the source list into a Doxyfile INPUT value that Doxygen can parse
foreach(path IN LISTS header_paths)
    string(APPEND MIMETIC_DOXYGEN_INPUT " \\\n\"${path}\"")
endforeach()
foreach(file IN LISTS source_files)
    string(APPEND MIMETIC_DOXYGEN_INPUT " \\\n\"${CMAKE_CURRENT_SOURCE_DIR}/${file}\"")
endforeach()

configure_file(doxygen.config.cmake.in Doxyfile @ONLY)

add_custom_command(OUTPUT "html/index.html"
                   COMMAND "${DOXYGEN_EXECUTABLE}"
                   WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
                   MAIN_DEPENDENCY Doxyfile
                   DEPENDS ${header_paths} ${source_files} ${extra_files}
                   COMMENT "Generating HTML documentation"
                   VERBATIM)

add_custom_target(docs ALL SOURCES "html/index.html")
set_target_properties(docs PROPERTIES FOLDER "mimetic")

