#pragma once

#include "DX9Include.h"
#include "Device.h"
#include "Context.h"
#include "RefCount.hpp"

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