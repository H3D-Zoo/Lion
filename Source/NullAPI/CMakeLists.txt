cmake_minimum_required(VERSION 3.3)

set(THIS_TARGET nullrenderer)
set(DIR_COMMON ../Common)

include_directories(
	${DIR_COMMON}/
)

set(RENDERAPI_SOURCE_FILES_COMMON
	${DIR_COMMON}/RefCount.hpp
	${DIR_COMMON}/Logger.h
	${DIR_COMMON}/Logger.cpp
	${DIR_COMMON}/BaseAPIInstance.h
	${DIR_COMMON}/BaseAPIInstance.cpp
	${DIR_COMMON}/RenderAPIExport.def
	${DIR_COMMON}/RenderAPI.rc
	${DIR_COMMON}/resource.h
)

set(RENDERAPI_INCLUDE_FILES
	 ../../RenderAPI/RenderAPI.h
)

set(RENDERAPI_IMPL_INCLUDE_FILES
	./AutoPtr.hpp
	./APIInstance.h
	./Device.h
	./Context.h
	./SwapChain.h
	./RenderTarget.h
	./DepthStencil.h
	./VertexBuffer.h
	./IndexBuffer.h
	./VertexDeclaration.h
	./Texture2D.h
	./TextureCube.h
	./FXEffect.h
	./StateManager.h
	./OcclusionQuery.h
	./TextBox.h
	./RenderStatistic.h
)

set(RENDERAPI_IMPL_SOURCE_FILES
	./APIInstance.cpp
	./Device.cpp
	./Context.cpp
	./ContextLegacy.cpp
	./SwapChain.cpp
	./RenderTarget.cpp
	./DepthStencil.cpp
	./VertexBuffer.cpp
	./IndexBuffer.cpp
	./VertexDeclaration.cpp
	./Texture2D.cpp
	./TextureCube.cpp
	./FXEffect.cpp
	./OcclusionQuery.cpp
	./TextBox.cpp
	./RenderStatistic.cpp
)

source_group(include FILES ${DIR_INTERFACE})
source_group(source\\include FILES ${RENDERAPI_IMPL_INCLUDE_FILES})
source_group(source\\source FILES ${RENDERAPI_IMPL_SOURCE_FILES})
source_group(source\\common FILES ${RENDERAPI_SOURCE_FILES_COMMON})

list(APPEND ${THIS_TARGET}_SOURCE_FILES 
	${RENDERAPI_INCLUDE_FILES}
	${RENDERAPI_IMPL_INCLUDE_FILES}
	${RENDERAPI_IMPL_SOURCE_FILES}
	${RENDERAPI_SOURCE_FILES_COMMON}
)

add_library(${THIS_TARGET} SHARED ${${THIS_TARGET}_SOURCE_FILES})

set_target_properties( ${THIS_TARGET} PROPERTIES OUTPUT_NAME_DEBUG "${THIS_TARGET}_d")
set_target_properties( ${THIS_TARGET} PROPERTIES OUTPUT_NAME_RELEASE "${THIS_TARGET}")

set (CMAKE_SHARED_LINKER_FLAGS_RELEASE "/DEBUG")
set_target_properties( ${THIS_TARGET} PROPERTIES LINK_FLAGS_DEBUG "/NXCOMPAT")
set_target_properties( ${THIS_TARGET} PROPERTIES LINK_FLAGS_RELEASE "/OPT:REF /OPT:ICF")

install (TARGETS ${THIS_TARGET} RUNTIME DESTINATION .)
install (FILES $<TARGET_PDB_FILE:${THIS_TARGET}> DESTINATION .)