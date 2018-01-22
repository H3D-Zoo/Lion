cmake_minimum_required(VERSION 3.3)
set_property(GLOBAL PROPERTY USE_FOLDERS ON) 

project(RenderAPI)

if(MSVC_VERSION EQUAL 1500)
	set(MSVC_VERSION_STR "vs2008")
elseif(MSVC_VERSION EQUAL 1600)
	set(MSVC_VERSION_STR "vs2010")
elseif(MSVC_VERSION EQUAL 1700)
	set(MSVC_VERSION_STR "vs2012")
elseif(MSVC_VERSION EQUAL 1800)
	set(MSVC_VERSION_STR "vs2013")
elseif(MSVC_VERSION EQUAL 1900)
	set(MSVC_VERSION_STR "vs2015")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/output/${MSVC_VERSION_STR}) 

add_subdirectory(Source/DX9API d3d9renderer)
add_subdirectory(Source/FixedAPI d3d7renderer)
add_subdirectory(Source/NullAPI nullrenderer)

if(MSVC_VERSION GREATER 1800)
add_subdirectory(Win32TestBed Win32TestBed)
endif()
