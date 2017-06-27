#pragma once

#include "Device.h"
#include "Context.h"
#include "RenderAPIImpl.h"
#include "DX9Include.h"

class APIContext
{
public:
	void AddRef();
	void Release();

	::Device* pDevice;
	::Context* pContext;
	D3D9DLL* pD3D;
	D3DPRESENT_PARAMETERS CreationParam;

private:
	RefCount m_refCount;
};