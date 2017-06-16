#pragma once

namespace RenderAPI
{
	/*
		ȫ�ֳ�ʼ��/��������
	*/
	// RHI ��ʼ���ӿڣ�����ʹ��������һ�����ӿ�֮ǰ���ô˽ӿ�
	bool Initialize();

	// ������������û��ӿڶ��ͷź�����������ӿ�
	void Deinitialize();

	/*
		ϵͳ��־���ú���
	*/
	// �ⲿ�û��̳в�ʵ�ִ˽ӿڣ����ض�����Զ��崦��ϵͳ�ڲ���Log��
	class Logger
	{
	public:
		virtual ~Logger() { }

		virtual void LogError(const char*) = 0;

		virtual void LogWarning(const char*) = 0;

		virtual void LogDebug(const char*) = 0;

		virtual void LogVerbose(const char*) = 0;
	};

	// ���ϵͳ����Ĭ��Log����
	Logger* GetDefaultLogger();

	// ��ȡ��ǰϵͳ����ʹ�õ�Log����
	Logger* GetCurrentLogger();

	// �޸�ϵͳʹ�õ�Log�������Զ���log���λ�á�
	// ������Ϊnullptr��ʱ��ϵͳ��ʹ��Ĭ�ϵ�log���������
	void SetCurrentLogger(Logger*);

	/*
		�û��ӿڴ���
	*/
	enum BackBufferFormat
	{
		BACKBUFFER_XRGB8 = 0,
		BACKBUFFER_ARGB8 = 1,
	};

	enum ZBufferFormat
	{
		ZBUFFER_D24S8 = 0,
		ZBUFFER_D24X8 = 1,
		ZBUFFER_D16 = 2,
	};

	enum AAMode
	{
		AA_Disable = 0,
		AA_X2 = 1 << 0,
		AA_X4 = 1 << 1,
		AA_X6 = 1 << 2,
		AA_X8 = 1 << 3,
		AA_X10 = 1 << 4,
	};

	// ��� backbufferWidth �� backbufferHeight ͬʱ����Ϊ 0������ʹ�� hWindow Ĭ�ϵĴ�С��Ϊ backbuffer �Ĵ�С
	struct SwapChainDesc
	{
		SwapChainDesc()
			: hWindow(0)
			, backbufferWidth(0)
			, backbufferHeight(0)
			, backbufferFormat(BACKBUFFER_XRGB8)
			, zbufferFormat(ZBUFFER_D24S8)
			, aaMode(AA_Disable)
		{ }

		void* hWindow;
		unsigned int backbufferWidth;
		unsigned int backbufferHeight;
		BackBufferFormat backbufferFormat;
		ZBufferFormat zbufferFormat;
		AAMode aaMode;
	};

	class Device;
	class Context;
	struct CreationResult
	{
		CreationResult()
			: Success(false)
			, DevicePtr(0)
			, ContextPtr(0) { }

		bool Success;
		Device* DevicePtr;
		Context* ContextPtr;
	};

	CreationResult CreateDeviceAndContext(SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync);

	/*
		�û���Ⱦ�ӿ�
	*/
	// ���е��û�����ӿڶ��Ӵ˶���̳У�����Ҫ��ʽ��д����ࡣ
	// �û�����ӿ��ڲ�ʹ�õ�����£���Ҫ�ֶ� Release �ͷ���Դ
	// ����ͨ����������ֵ����ӿڶ���
	class RObject
	{
	public:
		virtual void Release() = 0;

	protected:
		RObject() { }
		virtual ~RObject() { }

	private:
		RObject(const RObject&);
		RObject& operator=(const RObject&);
	};

	/*
		��Դ�����ӿ�
	*/
	enum ResourceUsage
	{
		RES_USAGE_Default = 0,
		RES_USAGE_Dynamic = 1,
		RES_USAGE_Immuable = 2,
	};

	enum InputFormat
	{
		INPUT_Float2 = 0,
		INPUT_Float3 = 1,
		INPUT_Float4 = 2,
	};

	enum Semantic
	{
		SEMANTIC_POSITION = 0,
		SEMANTIC_COLOR = 1,
		SEMANTIC_NORMAL = 2,
		SEMANTIC_TEXCOORD = 3,
	};

	struct VertexElement
	{
		VertexElement()
			: SemanticName(SEMANTIC_POSITION)
			, SemanticIndex(0)
			, StreamIndex(0)
			, AlignOffset(0xFFFFFFFF)
			, Format(INPUT_Float4)
		{ }

		Semantic SemanticName;
		unsigned int SemanticIndex;
		unsigned int StreamIndex;
		unsigned int AlignOffset;
		InputFormat Format;
	};

	enum TextureFormat
	{
		TEX_Unknown = 0,
		TEX_ARGB = 1,
		TEX_ABGR = 2,
		TEX_XRGB = 3,
		TEX_XBGR = 4,
		TEX_DXT1 = 5,
		TEX_DXT3 = 6,
		TEX_DXT5 = 7,
		TEX_D24S8 = 8,
		TEX_D24X8 = 9,
		TEX_D32 = 10,
		TEX_D16 = 11,
	};

	enum IndexFormat
	{
		INDEX_Int16 = 0,
		INDEX_Int32 = 1,
	};

	enum TextureBinding
	{
		BINDING_ShaderResource = 1 << 0,
		BINDING_RenderTraget = 1 << 1,
		BINDING_DepthStencil = 1 << 2,
	};

	class SwapChain;
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	class FXEffect;
	class RenderTarget;
	class DepthStencil;

	class Device : public RObject
	{
	public:
		virtual SwapChain* GetDefaultSwapChain() const = 0;

		virtual RenderAPI::DepthStencil* GetDefaultDepthStencil() const = 0;

		virtual SwapChain* CreateAdditionalSwapChain(const SwapChainDesc&) = 0;

		virtual VertexBuffer* CreateVertexBuffer(ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, Semantic* semantics, unsigned int semanticCount, void* initialData) = 0;

		virtual IndexBuffer* CreateIndexBuffer(ResourceUsage usage, IndexFormat format, unsigned int indexCount, void* initialData) = 0;

		virtual Texture2D* CreateTexture2D(ResourceUsage usage, TextureFormat format, TextureBinding binding, unsigned int width, unsigned int height, void* initialData) = 0;

		virtual FXEffect* CreateFXEffectFromFile(const char* effectFilePath) = 0;

		virtual RenderTarget* CreateRenderTarget(BackBufferFormat format, unsigned int width, unsigned int height) = 0;

		virtual DepthStencil* CreateDepthStencil(ZBufferFormat format, unsigned int width, unsigned int height) = 0;
	};

	/*
		��Ⱦ�����ύ�ӿ�
	*/
	enum BlendFactor
	{
		BLEND_Zero = 0,
		BLEND_One = 1,
		BLEND_SourceColor = 2,
		BLEND_SourceAlpha = 3,
		BLEND_DestColor = 4,
		BLEND_DestAlpha = 5,
		BLEND_InverseSourceColor = 6,
		BLEND_InverseSourceAlpha = 7,
		BLEND_InverseDestColor = 8,
		BLEND_InverseDestAlpha = 9,
	};

	enum BlendOperator
	{
		BLEND_OP_Add = 0,
		BLEND_OP_Sub = 1,
	};

	struct BlendState
	{
		BlendState()
			: IsEnable(false)
			, IsAlphaSeperate(false)
			, ColorSrcAlpha(BLEND_One)
			, ColorDstAlpha(BLEND_Zero)
			, AlphaSrcAlpha(BLEND_One)
			, AlphaDstAlpha(BLEND_Zero)
			, ColorOp(BLEND_OP_Add)
			, AlphaOp(BLEND_OP_Add)
		{	}


		bool IsEnable;
		bool IsAlphaSeperate;
		BlendFactor ColorSrcAlpha;
		BlendFactor ColorDstAlpha;
		BlendFactor AlphaSrcAlpha;
		BlendFactor AlphaDstAlpha;
		BlendOperator ColorOp;
		BlendOperator AlphaOp;
	};

	enum CompareMethod
	{
		COMPARE_Never = 0,
		COMPARE_Always = 1,
		COMPARE_Equal = 2,
		COMPARE_NotEqual = 3,
		COMPARE_Less = 4,
		COMPARE_LessEqual = 5,
		COMPARE_Greater = 6,
		COMPARE_GreaterEqual = 7,
	};

	struct AlphaTestingState
	{
		AlphaTestingState()
			: IsEnable(false)
			, Reference(0)
			, Function(COMPARE_LessEqual)
		{	}

		bool IsEnable;
		unsigned char Reference;
		CompareMethod Function;
	};

	struct DepthTestingState
	{
		DepthTestingState()
			: IsEnable(false)
			, Reference(1.0f)
			, Function(COMPARE_LessEqual)
		{	}

		bool IsEnable;
		float Reference;
		CompareMethod Function;
	};

	enum StencilOp
	{
		STENCIL_Keep = 0,
		STENCIL_Zero = 1,
		STENCIL_Replace = 2,
		STENCIL_IncreaseSaturate = 3,
		STENCIL_DecreaseSaturate = 4,
		STENCIL_Invert = 5,
		STENCIL_IncreaseRevert = 6,
		STENCIL_DecreaseRevert = 7,
	};

	struct StencilOps
	{
		StencilOps()
			: Function(COMPARE_Always)
			, SFail(STENCIL_Keep)
			, SPassZFail(STENCIL_Keep)
			, AllPass(STENCIL_Keep)
		{	}

		CompareMethod Function;
		StencilOp SFail;
		StencilOp SPassZFail;
		StencilOp AllPass;
	};

	struct StencilTestingState
	{
		StencilTestingState()
			: IsEnable(false)
			, TwoSide(false)
			, Reference(0)
			, TestMask(0xFFFFFFFF)
			, WriteMask(0xFFFFFFFF)
		{	}

		bool IsEnable;
		bool TwoSide; // �������ѡ���Ҫ�� CullMode ��Ϊ None
		unsigned char Reference;
		unsigned int TestMask;
		unsigned int WriteMask;
		StencilOps FrontSide;
		StencilOps BackSide;
	};

	enum TextureOp
	{
		TEXOP_Disable = 0,
		TEXOP_SelectArg1 = 1,
		TEXOP_SelectArg2 = 2,
		TEXOP_Modulate = 3,
		TEXOP_Modulate2x = 4,
		TEXOP_Modulate4x = 5,
		TEXOP_Add = 6,
		TEXOP_AddSigned = 7,
		TEXOP_AddSigned2x = 8,
		TEXOP_AddSigned4x = 9,
		TEXOP_Sub = 10,
		TEXOP_AddSmooth = 11, // Screen
		TEXOP_Lerp = 12
	};

	enum TextureArg
	{
		TEXARG_Texture,
		TEXARG_Constant,
		TEXARG_Current,
		TEXARG_TextureFactor,
	};

	struct TextureBlendingState
	{
		TextureBlendingState()
			: ColorOp(TEXOP_Disable)
			, AlphaOp(TEXOP_Disable)
			, ColorArg0(TEXARG_Texture)
			, ColorArg1(TEXARG_Current)
			, AlphaArg0(TEXARG_Texture)
			, AlphaArg1(TEXARG_Current)
			, TextureFactor(0xFFFFFFFF)
		{	}

		TextureOp  ColorOp;
		TextureOp  AlphaOp;
		TextureArg ColorArg0;
		TextureArg ColorArg1;
		TextureArg AlphaArg0;
		TextureArg AlphaArg1;
		unsigned int TextureFactor;
	};

	enum SamplerFilter
	{
		FILTER_MinP_MagP_MipP = 0x0000,		// P = Point
		FILTER_MinP_MagP_MipL = 0x0001,		// L = Linear
		FILTER_MinP_MagL_MipP = 0x0010,		// X = None
		FILTER_MinP_MagL_MipL = 0x0011,
		FILTER_MinL_MagP_MipP = 0x0100,
		FILTER_MinL_MagP_MipL = 0x0101,
		FILTER_MinL_MagL_MipP = 0x0110,
		FILTER_MinL_MagL_MipL = 0x0111,
		FILTER_MinP_MagP_MipX = 0x0002,
		FILTER_MinP_MagL_MipX = 0x0012,
		FILTER_MinL_MagP_MipX = 0x0102,
		FILTER_MinL_MagL_MipX = 0x0112,
		FILTER_Anisotropic = 0x1000,
	};

	enum TextureAddress
	{
		TEX_ADDRESS_Repeat = 0,
		TEX_ADDRESS_Clamp = 1,
		TEX_ADDRESS_Mirror = 2,
	};

	struct TextureSampler
	{
		TextureSampler()
			: Filter(FILTER_MinP_MagP_MipX)
			, AddressU(TEX_ADDRESS_Clamp)
			, AddressV(TEX_ADDRESS_Clamp)
			, BorderColor(0x00000000)
		{	}

		SamplerFilter Filter;
		TextureAddress AddressU;
		TextureAddress AddressV;
		unsigned int BorderColor;
	};

	enum FillMode
	{
		FILL_Solid = 0,
		FILL_Wireframe = 1,
		FILL_Point = 2,
	};

	enum CullMode
	{
		CULL_None = 0,
		CULL_ClockWise = 1,
		CULL_CounterClockWise = 2,
	};

	enum Primitive
	{
		PRIMITIVE_TriangleList = 0,
		PRIMITIVE_TriangleStrip = 1,
		PRIMITIVE_TriangleFan = 2,
		PRIMITIVE_LineList = 3,
		PRIMITIVE_LineStrip = 4,
	};

	struct ScissorState
	{
		ScissorState()
			: IsEnable(false)
			, Left(0)
			, Right(0)
			, Top(0)
			, Bottom(0)
		{	}

		bool IsEnable;
		long Left;
		long Right;
		long Top;
		long Bottom;
	};

	struct Viewport
	{
		Viewport()
			: Left(0)
			, Top(0)
			, Width(0)
			, Height(0)
			, MinZ(0.0f)
			, MaxZ(1.0f)
		{ }

		unsigned int Left;
		unsigned int Top;
		unsigned int Width;
		unsigned int Height;
		float MinZ;
		float MaxZ;
	};
	
	struct VertexBufferInfo
	{
		VertexBufferInfo()
			: BufferPtr(0)
			, Stride(0)
			, Offset(0)
		{ }

		VertexBuffer* BufferPtr;
		unsigned int Stride;
		unsigned int Offset;
	};

	struct MappedResource
	{
		MappedResource()
			: Success(false)
			, DataPtr(0)
			, LinePitch(0)
		{ }

		bool Success;
		void* DataPtr;
		unsigned int LinePitch;
	};

	class Context :public RObject
	{
	public:
		virtual void ClearRenderTarget(RenderTarget* rt, unsigned int color) = 0;

		virtual void ClearDepthStencil(DepthStencil* ds, float z, unsigned int stencil) = 0;

		virtual void SetViewport(const Viewport&) = 0;

		virtual void SetRenderTarget(unsigned int index, RenderTarget* renderTarget) = 0;

		virtual void SetRenderTarget(DepthStencil* depthStencil) = 0;

		virtual void SetVertexBuffers(unsigned int startSlot, VertexBufferInfo* buffers, unsigned int bufferCount) = 0;

		virtual void SetIndexBuffer(IndexBuffer* buffer, unsigned int offset) = 0;

		virtual void SetEffect(FXEffect*) = 0;

		virtual void SetTextures(unsigned int startSlot, Texture2D** textures, unsigned int resCount) = 0;

		virtual void SetBlendState(const BlendState& state) = 0;

		virtual void SetAlphaTestingState(const AlphaTestingState& state) = 0;

		virtual void SetDepthTestingState(const DepthTestingState& state) = 0;

		virtual void SetStencilTestingState(const StencilTestingState& state) = 0;

		virtual void SetDepthWriting(bool enable) = 0;

		virtual void SetTextureBlendingState(unsigned int startSlot, const TextureBlendingState** samplers, unsigned int count) = 0;

		virtual void SetTextureSampler(unsigned int startSlot, const TextureSampler** samplers, unsigned int count) = 0;

		virtual void SetScissorState(const ScissorState& state) = 0;

		virtual void SetFillMode(FillMode mode) = 0;

		virtual void SetCullMode(CullMode mode) = 0;

		virtual void SetDepthBias(float bias) = 0;

		virtual void Draw(Primitive primitive, unsigned int startIndex, unsigned int primitiveCount) = 0;

		virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount) = 0;

		virtual void GenerateMipmaps(Texture2D* texture) = 0;

		virtual void CheckDeviceLost() = 0;

		virtual void ResetDevice() = 0;
	};

	class VertexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual unsigned int GetVertexCount() const = 0;

		virtual unsigned int GetVertexStride() const = 0;

		virtual unsigned int GetLength() const = 0;

		virtual const Semantic* GetSemanticPtr() const = 0;

		virtual unsigned int GetSemanticCount() const = 0;
	};

	class IndexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual IndexFormat GetFormat() const = 0;

		virtual unsigned int GetIndexCount() const = 0;

		virtual unsigned int GetLength() const = 0;
	};

	class Texture2D : public RObject
	{
	public:
		virtual TextureFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool IsRenderTarget() const = 0;

		//�벻Ҫ������������Release
		virtual RenderTarget* GetRenderTarget() const = 0;

		virtual bool IsDepthStencil() const = 0;

		//�벻Ҫ������������Release
		virtual DepthStencil* GetDepthStencil() const = 0;
	};

	class RenderTarget : public RObject
	{
	public:
		virtual BackBufferFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;
	};

	class DepthStencil : public RObject
	{
	public:
		virtual ZBufferFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;
	};

	class FXEffect : public RObject
	{

	};

	class SwapChain : public RObject
	{
	public:
		// ��� RenderTarget ����ʹ�õ�ʱ��Ҫ���� Release
		virtual RenderTarget* GetRenderTarget() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool OnResize(unsigned int width, unsigned int height) = 0;

		// ֻ�� Ĭ��Swapchain��������ȫ��ģʽ
		virtual void SetFullscreen(bool fullscreen) = 0;

		virtual bool IsFullscreen() const = 0;

		virtual void Present() = 0;
	};
}