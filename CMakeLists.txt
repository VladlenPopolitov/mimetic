cmake_minimum_required(VERSION 3.13.4)

project(mimetic)

set(MIMETIC_MAJOR_VERSION 0)
set(MIMETIC_MINOR_VERSION 10)
set(MIMETIC_PATCH_VERSION 0)
set(MIMETIC_VERSION
    ${MIMETIC_MAJOR_VERSION}.${MIMETIC_MINOR_VERSION}.${MIMETIC_PATCH_VERSION})
set(MIMETIC_VERSION_STRING "${MIMETIC_VERSION}")

set (CMAKE_CXX_STANDARD 11)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" MIMETIC_STANDALONE)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(MIMETIC_BUILD_EXAMPLES "Build the MIMETIC example programs" ${MIMETIC_STANDALONE})
option(MIMETIC_BUILD_TESTS "Build the MIMETIC test programs" ${MIMETIC_STANDALONE})
option(MIMETIC_BUILD_DOCS "Build the MIMETIC documentation" ON)
option(MIMETIC_INSTALL "Generate installation target" ON)



include(CheckFunctionExists)
include(CheckIncludeFiles)
CHECK_INCLUDE_FILES("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE_CXX(dirent.h HAVE_DIRENT_H)
CHECK_INCLUDE_FILE_CXX(dlfcn.h HAVE_DLFCN_H)
check_function_exists(getpagesize HAVE_GETPAGESIZE)
CHECK_INCLUDE_FILE_CXX(inttypes.h HAVE_INTTYPES_H)
check_function_exists(madvise HAVE_MADVISE)
CHECK_INCLUDE_FILE_CXX(memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILE_CXX(ndir.h HAVE_NDIR_H)
CHECK_INCLUDE_FILE_CXX(stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILE_CXX(stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILE_CXX(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILE_CXX(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE_CXX(sys/dir.h HAVE_SYS_DIR_H)
CHECK_INCLUDE_FILE_CXX(sys/ndir.h HAVE_SYS_NDIR_H)
CHECK_INCLUDE_FILE_CXX(sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILE_CXX(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE_CXX(unistd.h HAVE_UNISTD_H)

CHECK_INCLUDE_FILE_CXX(sys/mman.h HAVE_MMAN_H)
if(${HAVE_MMAN_H})
   set(HAVE_MMAP 1)
endif()

# add variable HAVE_MIMETIC_CONFIG - it is used by libconfig.h
add_compile_definitions(HAVE_MIMETIC_CONFIG=1)
# create library mimetic
add_library(mimetic)

#generate maximal level of compiler warnings
target_compile_options(mimetic PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
#create config file
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h)
#create config file in build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h.cmake.in ${mimetic_BINARY_DIR}/include/mimetic/config.h)
target_include_directories(mimetic PRIVATE ${mimetic_BINARY_DIR}/include/)

# include sources from subdirectories
include(mimetic/codec/CMakeLists.txt)
foreach(FILE ${SOURCESmimeticcodec})
  set(libraryFiles ${libraryFiles} mimetic/codec/${FILE})
endforeach()

include(mimetic/os/CMakeLists.txt)
foreach(FILE ${SOURCESmimeticos})
 if( (${HAVE_MMAN_H}) OR (NOT (${FILE} STREQUAL "mmfile.cxx")))
  set(libraryFiles ${libraryFiles} mimetic/os/${FILE})
 else()
  message("mmfile.cxx is skipped in this build") 
 endif() 
endforeach()

include(mimetic/CMakeLists.txt)
foreach(FILE ${SOURCESmimetic})
  set(libraryFiles ${libraryFiles} mimetic/${FILE})
endforeach()

include(mimetic/parser/CMakeLists.txt)
foreach(FILE ${SOURCESmimeticparser})
  set(libraryFiles ${libraryFiles} mimetic/parser/${FILE})
endforeach()

include(mimetic/rfc822/CMakeLists.txt)
foreach(FILE ${SOURCESmimeticrfc822})
  set(libraryFiles ${libraryFiles} mimetic/rfc822/${FILE})
endforeach()

# Dirs are used by target_include_directories() and install()
include(GNUInstallDirs)

target_sources(mimetic PRIVATE ${libraryFiles} )
target_link_directories(mimetic PRIVATE mimetic)
target_include_directories(mimetic PUBLIC 
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"    # for build
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"  # for install
)

if(MIMETIC_BUILD_TESTS)
add_subdirectory(test)
endif()

if(MIMETIC_BUILD_EXAMPLES)
add_subdirectory(examples)
endif()
if (MIMETIC_BUILD_DOCS)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
endif()
if(DOXYGEN_FOUND AND MIMETIC_BUILD_DOCS)
 add_subdirectory(doc)
else()
 message("Doxygen not used") 
endif()


if (MIMETIC_INSTALL)
    
####
# add include directories
#target_include_directories(MathFunctions
#                           PUBLIC
#                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
#                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
#)

# install the target and create export-set
###install(TARGETS MathFunctions
   #     EXPORT MathFunctionsTargets
    #    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    #    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    #    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#)

# install header file
#install(FILES MathFunctions.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# generate and install export file
#install(EXPORT MathFunctionsTargets
 #      FILE MathFunctionsTargets.cmake
 #       NAMESPACE MathFunctions::
#        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MathFunctions
#)


####
    install(TARGETS mimetic 
        EXPORT mimeticTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        #DESTINATION lib
        )
    install(FILES ${mimetic_BINARY_DIR}/include/mimetic/config.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic )

    file(GLOB all_include_files_list CONFIGURE_DEPENDS "mimetic/*.h")
    install(FILES ${all_include_files_list} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic )
    file(GLOB all_include_files_list CONFIGURE_DEPENDS "mimetic/codec/*.h")
    install(FILES ${all_include_files_list} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic/codec )
    file(GLOB all_include_files_list CONFIGURE_DEPENDS "mimetic/os/*.h")
    install(FILES ${all_include_files_list} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic/os )
    file(GLOB all_include_files_list CONFIGURE_DEPENDS "mimetic/parser/*.h")
    install(FILES ${all_include_files_list} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic/parser )
    file(GLOB all_include_files_list CONFIGURE_DEPENDS "mimetic/rfc822/*.h")
    install(FILES ${all_include_files_list} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic/rfc822 )


    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR "${MIMETIC_MAJOR_VERSION}")
    set(CPACK_PACKAGE_VERSION_MINOR "${MIMETIC_MINOR_VERSION}")
    set(CPACK_PACKAGE_VERSION_PATCH "${MIMETIC_PATCH_VERSION}")
    include(CPack)

    if (DOXYGEN_FOUND AND MIMETIC_BUILD_DOCS)
        install(DIRECTORY "${mimetic_BINARY_DIR}/docs/html"
                DESTINATION "${CMAKE_INSTALL_DOCDIR}")
    endif()

    install(EXPORT mimeticTargets
      FILE mimeticTargets.cmake
      NAMESPACE mimetic::
      DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
    )

        #########
    # Section to create package to use find_package(mimetic) command in Cmake files.
    include(CMakePackageConfigHelpers)

    set_property(TARGET mimetic PROPERTY VERSION ${MIMETIC_VERSION})
    set_property(TARGET mimetic PROPERTY SOVERSION ${MIMETIC_MAJOR_VERSION})
    set_property(TARGET mimetic PROPERTY
      INTERFACE_mimetic_MAJOR_VERSION ${MIMETIC_MAJOR_VERSION} )
    set_property(TARGET mimetic APPEND PROPERTY
      COMPATIBLE_INTERFACE_STRING mimetic_MAJOR_VERSION
    )

    # generate the version file for the config file
    write_basic_package_version_file(
      "${CMAKE_CURRENT_BINARY_DIR}/mimeticConfigVersion.cmake"
      VERSION "${MIMETIC_VERSION}"
      COMPATIBILITY AnyNewerVersion
    )

    # create config file
    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/CMAKE/Config.cmake.in
      "${CMAKE_CURRENT_BINARY_DIR}/mimeticConfig.cmake"
      INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
    )

    # install config files
    install(FILES
              "${CMAKE_CURRENT_BINARY_DIR}/mimeticConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/mimeticConfigVersion.cmake"
              DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/mimetic
        # or: DESTINATION "${MIMETIC_CONFIG_PATH}")
            )

    # generate the export targets for the build tree
    export(EXPORT mimeticTargets
          FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/mimeticTargets.cmake"
          NAMESPACE mimetic::
    )
    #########

    # Only generate this target if no higher-level project already has
    if (NOT TARGET uninstall)
        configure_file(CMAKE/cmake_uninstall.cmake.in
                       cmake_uninstall.cmake IMMEDIATE @ONLY)
        add_custom_target(uninstall
                          "${CMAKE_COMMAND}" -P
                          "${mimetic_BINARY_DIR}/cmake_uninstall.cmake")
        set_target_properties(uninstall PROPERTIES FOLDER "mimetic")
    endif()


endif()
