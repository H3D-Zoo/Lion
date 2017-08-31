cmake_minimum_required(VERSION 3.3)

set(THIS_TARGET DX9API)
set(DIR_COMMON ../Common)

include_directories(
	../DXSDK/Include/
	${DIR_COMMON}/
)

set(RENDERAPI_SOURCE_FILES_COMMON
	${DIR_COMMON}/RefCount.hpp
	${DIR_COMMON}/Logger.h
	${DIR_COMMON}/Logger.cpp
	${DIR_COMMON}/BaseAPIGlobal.h
	${DIR_COMMON}/BaseAPIGlobal.cpp
	${DIR_COMMON}/RenderAPIExport.def
)

set(RENDERAPI_INCLUDE_FILES
	 ../../RenderAPI/RenderAPI.h
)

set(RENDERAPI_IMPL_INCLUDE_FILES
	./DX9Include.h
	./AutoPtr.hpp
	./EnumMapping.h
	./APIGlobal.h
	./Device.h
	./Context.h
	./SwapChain.h
	./RenderTarget.h
	./DepthStencil.h
	./VertexBuffer.h
	./IndexBuffer.h
	./VertexDeclaration.h
	./Texture2D.h
	./FXEffect.h
	./StateManager.h
	./OcclusionQuery.h
)

set(RENDERAPI_IMPL_SOURCE_FILES
	./EnumMapping.cpp
	./APIGlobal.cpp
	./Device.cpp
	./Context.cpp
	./SwapChain.cpp
	./RenderTarget.cpp
	./DepthStencil.cpp
	./VertexBuffer.cpp
	./IndexBuffer.cpp
	./VertexDeclaration.cpp
	./Texture2D.cpp
	./FXEffect.cpp
	./StateManager.cpp
	./OcclusionQuery.cpp
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

link_directories(../DXSDK/Lib/x86)
add_library(${THIS_TARGET} SHARED ${${THIS_TARGET}_SOURCE_FILES})
target_link_libraries(${THIS_TARGET} d3dx9.lib dxguid.lib)