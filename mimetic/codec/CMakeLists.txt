#INCLUDES=-I$(top_srcdir) 
#includedir=@INCLUDEDIR@/codec
#noinst_LTLIBRARIES = libmimeticcodec.la
#include_HEADERS=base64.h codec_base.h \
# codec_chain.h code.h other_codecs.h qp.h codec.h
#libmimeticcodec_la_SOURCES=base64.cxx qp.cxx 
cmake_minimum_required(VERSION 3.13.4)

set(HEADERSmimeticcodec 
base64.h 
codec_base.h 
codec_chain.h 
code.h 
other_codecs.h 
qp.h 
codec.h
)

set(SOURCESmimeticcodec 
base64.cxx 
qp.cxx 
base64.h 
codec_base.h 
codec_chain.h 
code.h 
other_codecs.h 
qp.h 
codec.h
)
