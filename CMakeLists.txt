cmake_minimum_required(VERSION 3.13.4)

project(mimetic)

set(MIMETIC_MAJOR_VERSION 0)
set(MIMETIC_MINOR_VERSION 9)
set(MIMETIC_PATCH_VERSION 8)
set(MIMETIC_VERSION
    ${MIMETIC_MAJOR_VERSION}.${MIMETIC_MINOR_VERSION}.${MIMETIC_PATCH_VERSION})
set(MIMETIC_VERSION_STRING "${MIMETIC_VERSION}")

set (CMAKE_CXX_STANDARD 11)

string(COMPARE EQUAL "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_SOURCE_DIR}" MIMETIC_STANDALONE)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(MIMETIC_BUILD_EXAMPLES "Build the MIMETIC example programs" ${MIMETIC_STANDALONE})
option(MIMETIC_BUILD_TESTS "Build the MIMETIC test programs" ${MIMETIC_STANDALONE})
option(MIMETIC_BUILD_DOCS "Build the MIMETIC documentation" ${MIMETIC_STANDALONE})
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

#create config file
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h.cmake.in ${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h)
#create config file in build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mimetic/config.h.cmake.in ${mimetic_BINARY_DIR}/include/config.h)
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

target_sources(mimetic PRIVATE ${libraryFiles} )
target_link_directories(mimetic PRIVATE mimetic)
target_include_directories(mimetic PUBLIC .)

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
endif()


if (MIMETIC_INSTALL)
    include(GNUInstallDirs)
    install(TARGETS mimetic DESTINATION lib)
    install(FILES ${mimetic_BINARY_DIR}/include/config.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/mimetic )

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


    #install(DIRECTORY include/MIMETIC DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    #        FILES_MATCHING PATTERN MIMETIC3.h PATTERN MIMETIC3native.h)

    #install(FILES "${MIMETIC_BINARY_DIR}/src/MIMETIC3Config.cmake"
    #              "${MIMETIC_BINARY_DIR}/src/MIMETIC3ConfigVersion.cmake"
    #        DESTINATION "${MIMETIC_CONFIG_PATH}")

    #install(EXPORT MIMETICTargets FILE MIMETIC3Targets.cmake
    #        EXPORT_LINK_INTERFACE_LIBRARIES
    #        DESTINATION "${MIMETIC_CONFIG_PATH}")
    #install(FILES "${MIMETIC_BINARY_DIR}/src/MIMETIC3.pc"
    #        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    if (DOXYGEN_FOUND AND MIMETIC_BUILD_DOCS)
        install(DIRECTORY "${mimetic_BINARY_DIR}/docs/html"
                DESTINATION "${CMAKE_INSTALL_DOCDIR}")
    endif()

    # Only generate this target if no higher-level project already has
    if (NOT TARGET uninstall)
        configure_file(CMake/cmake_uninstall.cmake.in
                       cmake_uninstall.cmake IMMEDIATE @ONLY)
        add_custom_target(uninstall
                          "${CMAKE_COMMAND}" -P
                          "${mimetic_BINARY_DIR}/cmake_uninstall.cmake")
        set_target_properties(uninstall PROPERTIES FOLDER "mimetic")
    endif()
endif()
