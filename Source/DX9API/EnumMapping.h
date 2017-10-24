#pragma once
#include <string>
#include "DX9Include.h"
#include "../../RenderAPI/RenderAPI.h"

const int kRTFormatCount = 2;
const int kDSFormatCount = 3;
const int kMSFormatCount = 6;
const int kTexFormatCount = 13;
const int kIndexLengthCount = 6;
const int kIndexFormatCount = 6;
const int kLockOptionCount = 4;
const int kImageFormatCount = 5;
const int kSamplerFilterCount = 6;

extern D3DFORMAT s_RTFormats[kRTFormatCount];
extern D3DFORMAT s_DSFormats[kDSFormatCount];
extern D3DFORMAT s_TextureFormats[kTexFormatCount];
extern D3DFORMAT s_IndexFormats[kIndexFormatCount];
extern unsigned int s_IndexLengths[kIndexLengthCount];
extern unsigned int s_lockOptions[kLockOptionCount];
extern D3DMULTISAMPLE_TYPE s_sampleTypes[kMSFormatCount];
extern D3DXIMAGE_FILEFORMAT s_d3dxFileFormat[kImageFormatCount]; 
extern D3DTEXTUREFILTERTYPE s_d3dSamplerFilter[kSamplerFilterCount];

bool IsLocalFileExist(const std::string & fileName);
RenderAPI::DeviceState DeviceStateMapping(unsigned int rst);
RenderAPI::BlendFactor BlendFactorMapping(unsigned int factor);
RenderAPI::BlendOperator BlendOperatorMapping(unsigned int op);
RenderAPI::CompareMethod CompareMethodMapping(unsigned int method);
RenderAPI::StencilOp StencilOpMapping(unsigned int sop);