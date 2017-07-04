#pragma once
#include <string>
#include "DX9Include.h"

const int RTFormatCount = 2;
const int DSFormatCount = 3;
const int MSFormatCount = 6;
const int TexFormatCount = 12;
const int IndexLengthCount = 6;
const int IndexFormatCount = 6;
const int LockOptionCount = 3;

extern D3DFORMAT s_RTFormats[RTFormatCount];
extern D3DFORMAT s_DSFormats[DSFormatCount];
extern D3DFORMAT s_TextureFormats[TexFormatCount];
extern D3DFORMAT s_IndexFormats[IndexFormatCount];
extern unsigned int s_IndexLengths[IndexLengthCount];
extern unsigned int s_lockOptions[LockOptionCount];
extern D3DMULTISAMPLE_TYPE s_sampleTypes[MSFormatCount];

bool IsLocalFileExist(const std::string & fileName);
