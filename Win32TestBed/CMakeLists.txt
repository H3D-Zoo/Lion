cmake_minimum_required(VERSION 3.3)

set(THIS_TARGET Win32TestBed)

include_directories(
	../RenderAPI/
)

set(TESTBED_INCLUDE_FILES
	./pch.h
	./testbed.h
)

set(ZLIB_SOURCE_FILES
	zlib/adler32.c
	zlib/compress.c
	zlib/crc32.c
	zlib/crc32.h
	zlib/deflate.c
	zlib/deflate.h
	zlib/gzguts.h
	zlib/infback.c
	zlib/inffast.c
	zlib/inffast.h
	zlib/inffixed.h
	zlib/inflate.c
	zlib/inflate.h
	zlib/inftrees.c
	zlib/inftrees.h
	zlib/trees.c
	zlib/trees.h
	zlib/uncompr.c
	zlib/zconf.h
	zlib/zlib.h
	zlib/zutil.c
	zlib/zutil.h
)

set(TESTBEDD_IMAGE_FILES
	./image/file_data.h
	./image/img_data.h
	./image/impl/bmp.cpp
	./image/impl/dds.cpp
	./image/impl/file_data.cpp
	./image/impl/img_data.cpp
	./image/impl/inner_include.cpp
	./image/impl/inner_include.h
	./image/impl/png.cpp
	./image/impl/tga.cpp
)

set(TESTBEDD_MATH_FILES
	./math/gmlaabb.h
	./math/gmlangle.h
	./math/gmlcolor.h
	./math/gmlconversion.h
	./math/gmldualquat.h
	./math/gmlmatrix.h
	./math/gmlray.h
	./math/gmlrect.h
	./math/gmlrotation.h
	./math/gmlutility.h
	./math/gmlvector.h
)

set(TESTBED_SOURCE_FILES
	./main.cpp
	./testbed.cpp
	./box.hpp
	./particle.h
	./particle.cpp
	./TintColor.fx
	./Particle.fx
)

source_group(include FILES ${TESTBED_INCLUDE_FILES})
source_group(source FILES ${TESTBED_SOURCE_FILES})
source_group(img FILES ${TESTBEDD_IMAGE_FILES})
source_group(math FILES ${TESTBEDD_MATH_FILES})
source_group(zlib FILES ${ZLIB_SOURCE_FILES})

list(APPEND ${THIS_TARGET}_SOURCE_FILES 
	${TESTBED_INCLUDE_FILES}
	${TESTBED_SOURCE_FILES}
	${TESTBEDD_IMAGE_FILES}
	${TESTBEDD_MATH_FILES}
	${ZLIB_SOURCE_FILES}
)

add_executable(${THIS_TARGET} WIN32 ${${THIS_TARGET}_SOURCE_FILES})
set_target_properties(${THIS_TARGET} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")
target_link_libraries(${THIS_TARGET} winmm.lib)
