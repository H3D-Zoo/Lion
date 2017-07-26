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

RenderAPI::BlendFactor BlendFactorMapping(unsigned int factor)
{
	switch (factor)
	{
	default:
	case D3DBLEND_ZERO:return RenderAPI::BLEND_Zero;
	case D3DBLEND_ONE: return RenderAPI::BLEND_One;
	case D3DBLEND_SRCCOLOR: return RenderAPI::BLEND_SrcColor;
	case D3DBLEND_SRCALPHA: return RenderAPI::BLEND_SrcAlpha;
	case D3DBLEND_DESTCOLOR: return RenderAPI::BLEND_DstColor;
	case D3DBLEND_DESTALPHA: return RenderAPI::BLEND_DstAlpha;
	case D3DBLEND_INVSRCCOLOR: return RenderAPI::BLEND_InvSrcColor;
	}
}

RenderAPI::BlendOperator BlendOperatorMapping(unsigned int factor)
{
	return RenderAPI::BlendOperator();
}

RenderAPI::CompareMethod CompareMethodMapping(unsigned int method)
{
	switch (method)
	{
	case D3DCMP_NEVER: return RenderAPI::COMPARE_Never;
	default:
	case D3DCMP_ALWAYS: return RenderAPI::COMPARE_Always;
	case D3DCMP_EQUAL: return RenderAPI::COMPARE_Equal;
	case D3DCMP_NOTEQUAL: return RenderAPI::COMPARE_NotEqual;
	case D3DCMP_LESS: return RenderAPI::COMPARE_Less;
	case D3DCMP_LESSEQUAL: return RenderAPI::COMPARE_LessEqual;
	case D3DCMP_GREATER: return RenderAPI::COMPARE_Greater;
	case D3DCMP_GREATEREQUAL: return RenderAPI::COMPARE_GreaterEqual;
	}
}

RenderAPI::StencilOp StencilOpMapping(unsigned int sop)
{
	switch (sop)
	{
	default:
	case D3DSTENCILOP_KEEP: return RenderAPI::STENCIL_Keep;
	case D3DSTENCILOP_ZERO: return RenderAPI::STENCIL_Zero;
	case D3DSTENCILOP_REPLACE: return RenderAPI::STENCIL_Replace;
	case D3DSTENCILOP_INCRSAT: return RenderAPI::STENCIL_IncreaseSaturate;
	case D3DSTENCILOP_DECRSAT: return RenderAPI::STENCIL_DecreaseSaturate;
	case D3DSTENCILOP_INVERT: return RenderAPI::STENCIL_Invert;
	case D3DSTENCILOP_INCR: return RenderAPI::STENCIL_IncreaseRevert;
	case D3DSTENCILOP_DECR: return RenderAPI::STENCIL_DecreaseRevert;
	}
}
