cmake_minimum_required(VERSION 3.3)

set(THIS_TARGET Win32TestBed)

include_directories(
	../RenderAPI/
)

set(TESTBED_INCLUDE_FILES
	./pch.h
	./testbed.h
)

set(TESTBED_SOURCE_FILES
	./main.cpp
	./testbed.cpp
)

source_group(include FILES ${TESTBED_INCLUDE_FILES})
source_group(source FILES ${TESTBED_SOURCE_FILES})

list(APPEND ${THIS_TARGET}_SOURCE_FILES 
	${TESTBED_INCLUDE_FILES}
	${TESTBED_SOURCE_FILES}
)

add_executable(${THIS_TARGET} WIN32 ${${THIS_TARGET}_SOURCE_FILES})