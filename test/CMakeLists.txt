cmake_minimum_required(VERSION 3.13.4)

CHECK_INCLUDE_FILE_CXX(getopt.h HAVE_GETOPT_H)
if(${HAVE_GETOPT_H})
   add_compile_definitions(HAVE_GETOPT_H=1)
endif()

# 1 executable to generate tests sources
add_executable(cutee cutee.cxx cutee.h)
target_compile_options(cutee PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
# 2 function to generate tests sources - input and output directory passed as ARGV1 and ARGV2
# ##############################################################################
# function cutee_create_test_cxx_files(target source_path output_path)
# ##############################################################################
function(cutee_create_test_cxx_files arg)
  if(ARGC LESS 3)
    message(STATUS "arguments error when calling cutee_create_test_cxx_files")
    return()
  endif()
  file(MAKE_DIRECTORY ${ARGV2})
  file(GLOB_RECURSE TEST_HEADERS_FILES_LIST ${ARGV1}/t.*.h)
  foreach(testSourceHFile ${TEST_HEADERS_FILES_LIST})
    file(RELATIVE_PATH
         inFile
         ${CMAKE_CURRENT_SOURCE_DIR}
         ${testSourceHFile})
    string(REPLACE "/" "_"  f1 ${inFile})
    string(REPLACE "\\"  "_" f2 ${f1})
    string(REPLACE ".h" ".cutee.cxx" outputFile ${f2})
    if(( NOT (${HAVE_MMAN_H}) ) AND ( ${inFile} STREQUAL "_t.file.h" ))
     message("t.file.h excluded from build (mmap() function is not implemented in this OS)")   
    else()
     if(( NOT ((${HAVE_UNISTD_H}) AND (HAVE_SYS_STAT_H)) ) AND ( ${inFile} STREQUAL "t.directory.h" ))
      message("t.directory.h excluded from build ( function is not implemented in this OS)")   
     else()
      add_custom_command(OUTPUT ${ARGV2}/${outputFile}
                         COMMAND $<TARGET_FILE:cutee>
                                 ARGS
                                 -o
                                 ${ARGV2}/${outputFile}
                                 ${testSourceHFile}
                         DEPENDS ${testSourceHFile} 
                         DEPENDS cutee
                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                         VERBATIM)
      set(TESTSRC ${TESTSRC} ${ARGV2}/${outputFile})
     endif()
    endif() 
    
  endforeach()
  # text cxx files from test directory
  file(GLOB_RECURSE TEST_CXX_FILES_LIST ${ARGV1}/t.*.cxx)
  set(TESTSRC ${TESTSRC} ${TEST_CXX_FILES_LIST})
  # main program to run tests - runtest.cxx
  set(TESTSRC ${TESTSRC} ${ARGV1}/runtest.cxx)
  target_sources(${ARGV0} PRIVATE ${TESTSRC})
endfunction(cutee_create_test_cxx_files)

# 3 - testing executable "runtest" (runtest.exe in Windows)
add_executable(runtest)
target_compile_options(runtest PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
# 4 - create test sources using function from step 2
cutee_create_test_cxx_files(runtest ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_BINARY_DIR}/autogenerated )

# 5 copy binary files needed to run tests
add_custom_command(
    TARGET runtest 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${CMAKE_CURRENT_SOURCE_DIR}/file.bin 
    ${CMAKE_CURRENT_SOURCE_DIR}/small.bin 
    ${CMAKE_CURRENT_SOURCE_DIR}/test.msg 
    ${CMAKE_CURRENT_SOURCE_DIR}/test3.msg 
    $<TARGET_FILE_DIR:runtest>
)

# 5 create 'check' target to run: make check
add_custom_target(check
                  COMMAND $<TARGET_FILE:runtest>
                  COMMENT "Running $<TARGET_FILE:runtest>...")
                  
target_link_libraries(runtest mimetic)
target_include_directories(runtest 
 PRIVATE ${mimetic_BINARY_DIR}/include/  # config.h
 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../  # mimetic headers
 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}      # test headers
 )

