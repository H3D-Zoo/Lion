#include <stdlib.h>
#include <io.h>
#include "../../RenderAPI/RenderAPI.h"
#include "Device.h"
#include "Context.h"
#include "EnumMapping.h"
#include "DX9Include.h"
#include "APIGlobal.h"
#include "AutoPtr.hpp"

D3DFORMAT s_RTFormats[RTFormatCount] =
{
	D3DFMT_X8R8G8B8,
	D3DFMT_A8R8G8B8,
};

D3DFORMAT s_DSFormats[DSFormatCount] =
{
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D16,
};

D3DFORMAT s_TextureFormats[TexFormatCount] =
{
	D3DFMT_UNKNOWN,
	D3DFMT_A8R8G8B8,
	D3DFMT_X8R8G8B8,
	D3DFMT_DXT1,
	D3DFMT_DXT3,
	D3DFMT_DXT5,
	D3DFMT_D24S8,
	D3DFMT_D24X8,
	D3DFMT_D32,
	D3DFMT_D16,
};

D3DMULTISAMPLE_TYPE s_sampleTypes[MSFormatCount] =
{
	D3DMULTISAMPLE_NONE,
	D3DMULTISAMPLE_2_SAMPLES,
	D3DMULTISAMPLE_4_SAMPLES,
	D3DMULTISAMPLE_6_SAMPLES,
	D3DMULTISAMPLE_8_SAMPLES,
	D3DMULTISAMPLE_10_SAMPLES,
};

unsigned int s_IndexLengths[IndexLengthCount] =
{
	sizeof(unsigned short),
	sizeof(unsigned int),
};

D3DFORMAT s_IndexFormats[IndexFormatCount] =
{
	D3DFMT_INDEX16,
	D3DFMT_INDEX32,
};

unsigned int s_lockOptions[LockOptionCount] =
{
	0,
	D3DLOCK_DISCARD,
	D3DLOCK_READONLY,
};

bool IsLocalFileExist(const std::string & fileName)
{
	return (_access(fileName.c_str(), 0) != -1);
}

RenderAPI::DeviceState DeviceStateMapping(unsigned int rst)
{
	if (rst == S_OK)
	{
		return RenderAPI::DEVICE_OK;
	}
	else if (rst == D3DERR_DEVICELOST)
	{
		return RenderAPI::DEVICE_Lost;
	}
	else if (rst == D3DERR_DEVICENOTRESET)
	{
		return RenderAPI::DEVICE_NeedReset;
	}
	else if (rst == D3DERR_DRIVERINTERNALERROR)
	{
		// 这个返回值是需要重新创建d3d9对象
		return RenderAPI::DEVICE_NeedRecreate;
	}
	else
	{
		return RenderAPI::DEVICE_Error;
	}

}
