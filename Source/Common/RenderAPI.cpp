#include "../../RenderAPI/RenderAPI.h"
#include "Logger.h"


namespace RenderAPI
{
	static Logger* s_currentLogger = NULL;

	Logger* GetDefaultLogger()
	{
		return &(SilentLogger::GetInstance());
	}

	Logger* GetCurrentLogger()
	{
		return s_currentLogger;
	}

	void SetCurrentLogger(Logger* logger)
	{
		if (logger == NULL)
		{
			s_currentLogger = &(SilentLogger::GetInstance());
		}
		else
		{
			s_currentLogger = logger;
		}
	}



	Logger::~Logger() { }

	RObject::RObject() { }
	RObject::~RObject() { }
	RObject::RObject(const RObject&) { }
	RObject& RObject::operator=(const RObject&) { return *this; }

	CreationResult::CreationResult()
		: Success(false)
		, DevicePtr(0)
		, ContextPtr(0) { }

	SwapChainDesc::SwapChainDesc()
		: hWindow(0)
		, backbufferWidth(0)
		, backbufferHeight(0)
		, backbufferFormat(BACKBUFFER_XRGB8)
		, zbufferFormat(ZBUFFER_Auto)
		, aaMode(AA_Disable)
	{ }

	VertexElement::VertexElement()
		: SemanticName(SEMANTIC_POSITION)
		, SemanticIndex(0)
		, StreamIndex(0)
		, AlignOffset(0xFFFFFFFF)
		, Format(INPUT_Float4)
	{ }

	BlendState::BlendState()
		: IsEnable(false)
		, IsAlphaSeperate(false)
		, ColorSrcAlpha(BLEND_One)
		, ColorDstAlpha(BLEND_Zero)
		, AlphaSrcAlpha(BLEND_One)
		, AlphaDstAlpha(BLEND_Zero)
		, ColorOp(BLEND_OP_Add)
		, AlphaOp(BLEND_OP_Add)
	{	}

	AlphaTestingState::AlphaTestingState()
		: IsEnable(false)
		, Reference(0)
		, Function(COMPARE_LessEqual)
	{	}

	DepthTestingState::DepthTestingState()
		: IsEnable(false)
		, Reference(1.0f)
		, Function(COMPARE_LessEqual)
	{	}

	StencilOps::StencilOps()
		: Function(COMPARE_Always)
		, SFail(STENCIL_Keep)
		, SPassZFail(STENCIL_Keep)
		, AllPass(STENCIL_Keep)
	{	}

	StencilTestingState::StencilTestingState()
		: IsEnable(false)
		, TwoSide(false)
		, Reference(0)
		, TestMask(0xFFFFFFFF)
		, WriteMask(0xFFFFFFFF)
	{	}

	TextureBlendingState::TextureBlendingState()
		: ColorOp(TEXOP_Disable)
		, AlphaOp(TEXOP_Disable)
		, ColorArg0(TEXARG_Texture)
		, ColorArg1(TEXARG_Current)
		, AlphaArg0(TEXARG_Texture)
		, AlphaArg1(TEXARG_Current)
		, TextureFactor(0xFFFFFFFF)
	{	}

	TextureSampler::TextureSampler()
		: Filter(FILTER_MinP_MagP_MipX)
		, AddressU(TEX_ADDRESS_Clamp)
		, AddressV(TEX_ADDRESS_Clamp)
		, BorderColor(0x00000000)
	{	}

	ScissorState::ScissorState()
		: IsEnable(false)
		, Left(0)
		, Right(0)
		, Top(0)
		, Bottom(0)
	{	}

	Viewport::Viewport()
		: Left(0)
		, Top(0)
		, Width(0)
		, Height(0)
		, MinZ(0.0f)
		, MaxZ(1.0f)
	{ }

	VertexBufferInfo::VertexBufferInfo()
		: BufferPtr(0)
		, Stride(0)
		, Offset(0)
	{ }
	MappedResource::MappedResource()
		: Success(false)
		, DataPtr(0)
		, LinePitch(0)
	{ }
}