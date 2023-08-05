cmake_minimum_required(VERSION 3.13.4)


#EXTRA_DISTtest.msg
#INCLUDES-I$(top_srcdir)
#LDADD../mimetic/libmimetic.la
#EXAMPLESb64 qp exbin structure catpart buildidx search mbox mm \
#    smime2xml smimestruct

#clean-local:
#	rm -f b64 qp exbin structure catpart buildidx search mbox mm \
#        smime2xml smimestruct

#examples: $(EXAMPLES)

#EXTRA_PROGRAMS$(EXAMPLES)

add_executable(b64 b64.cxx)
target_compile_options(b64 PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
#target_include_directories(runtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# 6 define directories of include files - top level directory for mimetic .h files
#target_include_directories(runtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

target_link_libraries(b64   $<TARGET_FILE:mimetic>)
target_include_directories(b64 PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(qp qp.cxx)
target_compile_options(qp PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(qp   $<TARGET_FILE:mimetic>)
target_include_directories(qp PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(exbin exbin.cxx)
target_compile_options(exbin PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(exbin   $<TARGET_FILE:mimetic>)
target_include_directories(exbin PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(structure structure.cxx)
target_compile_options(structure PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)

target_link_libraries(structure   $<TARGET_FILE:mimetic>)
target_include_directories(structure PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(catpart catpart.cxx)
target_compile_options(catpart PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(catpart   $<TARGET_FILE:mimetic>)
target_include_directories(catpart PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(buildidx buildidx.cxx)
target_compile_options(buildidx PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(buildidx   $<TARGET_FILE:mimetic>)
target_include_directories(buildidx PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(search search.cxx)
target_compile_options(search PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(search   $<TARGET_FILE:mimetic>)
target_include_directories(search PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(mbox mbox.cxx)
target_compile_options(mbox PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(mbox   $<TARGET_FILE:mimetic>)
target_include_directories(mbox PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

add_executable(mm mm.cxx mm.h engine.h engine.cxx cmdline.cxx cmdline.h)
target_compile_options(mm PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(mm   $<TARGET_FILE:mimetic>)
target_include_directories(mm PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)

#find_library(MIMETIC_EXAMPLES_CRYPTO_LIB crypto)

find_package(OpenSSL)
message("Crypto ${MIMETIC_EXAMPLES_CRYPTO_LIB} and openssl ${OPENSSL_FOUND} found" )
#if(MIMETIC_EXAMPLES_CRYPTO_LIB)
if(${OPENSSL_FOUND})
message("Crypto and openssl found")
# this definition removed from the newest versions of OpenSSL
add_compile_definitions(CRYPTO_MEM_CHECK_ON=0x1)
add_compile_definitions(CRYPTO_mem_ctrl=)
add_executable(smimestruct smimestruct.cxx)
target_compile_options(smimestruct PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(smimestruct   $<TARGET_FILE:mimetic> OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(smimestruct PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/)
#smimestruct_LDADD-lcrypto -lssl ../mimetic/libmimetic.la

add_executable(smime2xml smime2xml.cxx)
target_compile_options(smime2xml PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 >
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic >
)
target_link_libraries(smime2xml   $<TARGET_FILE:mimetic>  OpenSSL::Crypto OpenSSL::SSL)
target_include_directories(smime2xml PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/../ ${mimetic_BINARY_DIR}/include/ )
#smime2xml_LDADD-lcrypto -lssl ../mimetic/libmimetic.la

endif()
#endif() 

# 7 copy binary files needed to run tests
add_custom_command(
    TARGET b64 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${CMAKE_CURRENT_SOURCE_DIR}/test.msg 
    $<TARGET_FILE_DIR:b64>
)
