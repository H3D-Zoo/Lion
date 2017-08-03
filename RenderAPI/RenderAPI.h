#pragma once

namespace RenderAPI
{
	class Device;
	class Context;
	class Logger;

	enum RenderTargetFormat
	{
		BACKBUFFER_XRGB8 = 0,
		BACKBUFFER_ARGB8 = 1,
	};

	enum DepthStencilFormat
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

	// 如果 backbufferWidth 和 backbufferHeight 同时被设为 0，将会使用 hWindow 默认的大小作为 backbuffer 的大小
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
		RenderTargetFormat backbufferFormat;
		DepthStencilFormat zbufferFormat;
		AAMode aaMode;
	};

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

	struct DriverVersion
	{
		unsigned int Product;
		unsigned int Version;
		unsigned int Subversion;
		unsigned int BuildNumber;
		unsigned int WHQLLevel;
	};

	/*
	全局初始化/析构函数
	*/
	class APIGlobal
	{
	public:
		// 请把所有其他用户接口都释放后，最后调用这个接口
		virtual void Release() = 0;

		// 获得系统内置默认Log对象
		virtual Logger* GetDefaultLogger() = 0;

		// 获取当前系统正在使用的Log对象
		virtual Logger* GetCurrentLogger() = 0;

		// 修改系统使用的Log对象，以自定义log输出位置。
		// 当参数为nullptr的时候，系统会使用默认的log对象处理输出
		virtual void SetCurrentLogger(Logger*) = 0;

		// 用户接口创建
		virtual CreationResult CreateDeviceAndContext(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync) = 0;

		// 从文件中读取fx代码并编译，结果输出到compiledFXFile里
		virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile) = 0;

		virtual bool CheckMultiSampleSupport(RenderTargetFormat, DepthStencilFormat, AAMode, bool fullscreen) const = 0;

		virtual const char* GetDeviceDriver() const = 0;

		virtual const char* GetDeviceName() const = 0;

		virtual const char* GetDeviceDesc() const = 0;

		virtual unsigned int GetVendorID() const = 0;

		virtual DriverVersion GetDriverVersion() const = 0;
	};

	// RHI 初始化接口
	APIGlobal* CreateAPIGlobal();

	/*
		系统日志设置函数
	*/
	// 外部用户继承并实现此接口，以重定向或自定义处理系统内部的Log。
	class Logger
	{
	public:
		virtual ~Logger() { }

		virtual void LogE(const char*) = 0;

		virtual void LogW(const char*) = 0;

		virtual void LogD(const char*) = 0;

		virtual void LogV(const char*) = 0;
	};

	/*
		用户渲染接口
	*/
	// 所有的用户对象接口都从此对象继承，不需要显式书写虚基类。
	// 用户对象接口在不使用的情况下，需要手动 Release 释放资源
	// 不能通过拷贝、赋值构造接口对象
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
		资源创建接口
	*/
	enum ResourceUsage
	{
		RESUSAGE_Default = 0,
		RESUSAGE_Dynamic = 1,
		RESUSAGE_Immuable = 2,
	};

	enum LockOption
	{
		LOCK_Normal = 0,
		LOCK_Discard = 1,
		LOCK_ReadOnly = 2,
	};

	enum InputFormat
	{
		INPUT_Float1 = 0,
		INPUT_Float2 = 1,
		INPUT_Float3 = 2,
		INPUT_Float4 = 3,
		INPUT_Color4 = 4,
		INPUT_UByte4 = 5,
		INPUT_Short2 = 6,
		INPUT_Short4 = 7,
		INPUT_UShort2 = 8,
		INPUT_UShort4 = 9,
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
			, AlignOffset(0xFFFFFFFF)
			, Format(INPUT_Float4)
		{ }

		Semantic SemanticName;
		unsigned int SemanticIndex;
		unsigned int AlignOffset;
		InputFormat Format;
	};

	enum TextureFormat
	{
		TEX_Unknown = 0,
		TEX_ARGB = 1,
		TEX_XRGB = 2,
		TEX_DXT1 = 3,
		TEX_DXT3 = 4,
		TEX_DXT5 = 5,
		TEX_D24S8 = 6,
		TEX_D24X8 = 7,
		TEX_D32 = 8,
		TEX_D16 = 9,
	};

	enum IndexFormat
	{
		INDEX_Int16 = 0,
		INDEX_Int32 = 1,
	};

	class SwapChain;
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;
	class FXEffect;
	class RenderTarget;
	class DepthStencil;
	class OcclusionQuery;

	class Device : public RObject
	{
	public:
		// 这个 RenderTarget 不再使用的时候要调用 Release
		virtual SwapChain* GetDefaultSwapChain() = 0;

		virtual SwapChain* CreateAdditionalSwapChain(const SwapChainDesc&) = 0;

		virtual VertexBuffer* CreateVertexBuffer(ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, VertexElement* elements, unsigned int elementCount, void* initialData) = 0;

		virtual IndexBuffer* CreateIndexBuffer(ResourceUsage usage, IndexFormat format, unsigned int indexCount, void* initialData) = 0;

		virtual Texture2D* CreateTexture2D(ResourceUsage usage, TextureFormat format, unsigned int width, unsigned int height, void* initialData, int dataLinePitch, int dataHeight) = 0;

		virtual FXEffect* CreateFXEffectFromFile(const char* effectFilePath) = 0;

		virtual RenderTarget* CreateRenderTarget(RenderTargetFormat format, unsigned int width, unsigned int height) = 0;

		virtual DepthStencil* CreateDepthStencil(DepthStencilFormat format, unsigned int width, unsigned int height) = 0;

		virtual OcclusionQuery* CreateOcclusionQuery() = 0;

		//为了引擎临时加上去的，等一下一步在去掉吧
		virtual void* GetImplementPtr() = 0;
	};

	/*
		渲染命令提交接口
	*/
	enum BlendFactor
	{
		BLEND_Zero = 0,
		BLEND_One = 1,
		BLEND_SrcColor = 2,
		BLEND_SrcAlpha = 3,
		BLEND_DstColor = 4,
		BLEND_DstAlpha = 5,
		BLEND_InvSrcColor = 6,
		BLEND_InvSrcAlpha = 7,
		BLEND_InvDstColor = 8,
		BLEND_InvDstAlpha = 9,
	};

	enum BlendOperator
	{
		BLENDOP_Add = 0,
		BLENDOP_Sub = 1,
		BLENDOP_Min = 2,
		BLENDOP_Max = 2,
	};

	struct BlendState
	{
		BlendState()
			: IsEnable(false)
			, SrcBlend(BLEND_One)
			, DstBlend(BLEND_Zero)
			, BlendOp(BLENDOP_Add)
		{	}

		bool IsEnable;
		BlendFactor SrcBlend;
		BlendFactor DstBlend;
		BlendOperator BlendOp;
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
			, Function(COMPARE_LessEqual)
		{	}

		bool IsEnable;
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
		bool TwoSide; // 启用这个选项，需要将 CullMode 设为 None
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
		TEXOP_Sub = 9,
		TEXOP_AddSmooth = 10, // Screen
		TEXOP_DotProduct3 = 11,
		TEXOP_MultiplyAdd = 12,
		TEXOP_Lerp = 13
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
			: BlendOp(TEXOP_Disable)
			, Argument0(TEXARG_Texture)
			, Argument1(TEXARG_Current)
		{	}

		TextureOp  BlendOp;
		TextureArg Argument0;
		TextureArg Argument1;
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
		TEX_ADDRESS_Border = 2,
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
		CULL_CW = 1,
		CULL_CCW = 2,
	};

	enum Primitive
	{
		PRIMITIVE_TriangleList = 0,
		PRIMITIVE_TriangleStrip = 1,
		PRIMITIVE_TriangleFan = 2,
		PRIMITIVE_LineList = 3,
		PRIMITIVE_LineStrip = 4,
		PRIMITIVE_Point = 5,
	};

	enum DeviceState
	{
		DEVICE_OK = 0,
		DEVICE_Lost = 1,
		DEVICE_NeedReset = 2,
		DEVICE_NeedRecreate = 3,
		DEVICE_Busy = 4,
		DEVICE_OutOfMemory = 5,
		DEVICE_Error = 6,
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
			, Offset(0)
		{ }

		VertexBuffer* BufferPtr;
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

	//!对非2的幂texture的支持
	enum ENONPOW2Support
	{
		//!完全支持
		POW2_Support = 0,
		//!有条件支持
		POW2_Conditional = 1,
		//!不支持
		POW2_None = 2,
	};

	struct DeviceCaps
	{
		//!最大texture宽度
		unsigned int MaxTextureWidth;
		//!最大texture高度
		unsigned int MaxTextureHeight;
		//!最大各项异性程度
		unsigned int MaxAnisotropy;
		//!最大texture层数
		unsigned int MaxTextureStage;
		//!最大同时绑定的texture数
		unsigned int MaxSimultaneousTextures;
		//!用户裁减面最大数
		unsigned int MaxUserClipPlanes;
		//!一次绘制的最大图原数
		unsigned int MaxPrimitiveCount;
		//!最大顶点索引数
		unsigned int MaxVertexIndex;
		//!最大顶点数据流数
		unsigned int MaxStreams;
		//!最大顶点宽度
		unsigned int MaxStreamStride;
		//!最大VS寄存器常量
		unsigned int MaxVertexShaderConsts;
		//!最大同时render target数
		unsigned int MaxMRTs;
		//!VS最大指令数
		unsigned int MaxVertexShaderInstruction;
		//!PS最大指令数
		unsigned int MaxPixelShaderInstruction;
		//!最大vertex blend矩阵数
		unsigned int MaxVertexBlendMatrix;
		//!最大vertex blend索引数
		unsigned int MaxVertexBlendMatrixIndex;
		//!VS版本，是一个两位数数字
		unsigned int VertexShaderVersion;
		//!PS版本，是一个两位数数字
		unsigned int PixelShaderVersion;
		//!是否支持32位索引
		bool SupportIndex32;
		//!是否支持dynamic texture
		bool SupportsDynamicTexture;
		//支持贴图透明通道
		bool SupportTextureAlphaChannel;
		//!仅支持正方形贴图
		bool SupportOnlySquareTexture;
		//!非pow2贴图支持
		ENONPOW2Support NonePOW2Support;
		//!初始显存数
		unsigned int InitVideoMemory;
	};

	class Context : public RObject
	{
	public:
		virtual DeviceCaps GetDeviceCaps() = 0;

		virtual unsigned int GetAvailableTextureMemory() = 0;

		virtual void ClearRenderTarget(unsigned int color) = 0;

		virtual void ClearDepthBuffer(float z) = 0;

		virtual void ClearStencilBuffer(unsigned int stencil) = 0;

		virtual void SetViewport(const Viewport&) = 0;

		virtual RenderAPI::Viewport GetViewport() = 0;

		virtual void SetRenderTarget(unsigned int index, RenderTarget* renderTarget) = 0;

		virtual void SetDepthStencil(DepthStencil* depthStencil) = 0;

		virtual void SetVertexBuffers(VertexBufferInfo* buffers, unsigned int bufferCount) = 0;

		virtual void SetIndexBuffer(IndexBuffer* buffer, unsigned int offset) = 0;

		virtual void SetTexture(unsigned int slot, RenderAPI::Texture2D* textures) = 0;

		virtual void SetBlendState(const BlendState& state) = 0;

		virtual BlendState GetBlendState() const = 0;

		virtual void SetAlphaSeparateBlendState(const BlendState& state) = 0;

		virtual BlendState GetAlphaSeparateBlendState() const = 0;

		virtual void SetAlphaTestingState(const AlphaTestingState& state) = 0;

		virtual AlphaTestingState GetAlphaTestingState() const = 0;

		virtual void SetDepthTestingState(const DepthTestingState& state) = 0;

		virtual DepthTestingState GetDepthTestingState() const = 0;

		virtual void SetStencilTestingState(const StencilTestingState& state) = 0;

		virtual StencilTestingState GetStencilTestingState() const = 0;

		virtual void SetDepthWriting(bool enable) = 0;

		virtual void SetTextureColorBlendingState(unsigned int slot, const TextureBlendingState& state) = 0;

		virtual void SetTextureAlphaBlendingState(unsigned int slot, const TextureBlendingState& state) = 0;

		virtual void SetTextureSampler(unsigned int slot, const TextureSampler& sampler) = 0;

		virtual void SetScissorState(const ScissorState& state) = 0;

		virtual ScissorState GetScissorState() const = 0;

		virtual void SetColorWriteMask(bool r, bool g, bool b, bool a) = 0;

		virtual void SetFillMode(FillMode mode) = 0;

		virtual void SetCullMode(CullMode mode) = 0;

		virtual void SetDepthBias(float bias) = 0;

		virtual void SetSlopScaleDepthBias(float bias) = 0;

		virtual void SetTextureFactor(unsigned int factor) = 0;

		virtual bool BeginScene() = 0;

		virtual void EndScene() = 0;

		virtual void Draw(Primitive primitive, unsigned int startVertex, unsigned int primitiveCount) = 0;

		virtual void DrawIndexed(RenderAPI::Primitive primitive, unsigned int baseVertex, unsigned int startIndex, unsigned int primitiveCount) = 0;

		virtual bool UpdateTexture(Texture2D* src, Texture2D* dst) = 0;

		virtual DeviceState Present() = 0;

		virtual DeviceState GetState() = 0;

		virtual DeviceState ResetDevice(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync) = 0;
	};

	class VertexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual unsigned int GetVertexCount() const = 0;

		virtual unsigned int GetVertexStride() const = 0;

		virtual unsigned int GetLength() const = 0;

		virtual const VertexElement* GetElementPtr() const = 0;

		virtual unsigned int GetElementCount() const = 0;

		virtual void* Lock(unsigned int offset, unsigned int lockLength, LockOption lockOption) = 0;

		virtual void* DiscardLock() = 0;

		virtual void Unlock() = 0;
	};

	class IndexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual IndexFormat GetFormat() const = 0;

		virtual unsigned int GetIndexCount() const = 0;

		virtual unsigned int GetLength() const = 0;

		virtual void* Lock(unsigned int offset, unsigned int lockLength, LockOption lockOption) = 0;

		virtual void* DiscardLock() = 0;

		virtual void Unlock() = 0;
	};

	class Texture2D : public RObject
	{
	public:
		virtual TextureFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual MappedResource LockRect(unsigned int layer, LockOption lockOption) = 0;

		virtual void UnlockRect(unsigned int layer) = 0;

		virtual void GenerateMipmaps() = 0;
	};

	class RenderTarget : public RObject
	{
	public:
		virtual RenderTargetFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool IsTexture2D() const = 0;

		virtual Texture2D* GetTexturePtr() = 0;
	};

	class DepthStencil : public RObject
	{
	public:
		virtual DepthStencilFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool IsTexture2D() const = 0;

		virtual Texture2D* GetTexturePtr() = 0;
	};

	class FXEffect : public RObject
	{
	public:
		virtual unsigned int Begin() = 0;

		virtual void End() = 0;

		virtual bool BeginPass(unsigned int passIndex) = 0;

		virtual void EndPass() = 0;

		virtual void SetValidateTechnique() = 0;

		virtual void SetTechniqueByName(const char* name) = 0;

		virtual bool SetMatrix(const char* paramName, const float* matrix) = 0;

		virtual bool SetMatrixInArray(const char* paramName, const float* data, unsigned int index) = 0;

		virtual bool SetMatrixArray(const char* paramName, const float* data, unsigned int count) = 0;

		virtual bool SetValue(const char* paramName, const void* pData, unsigned int sizeinByte) = 0;

		virtual bool SetValueInArray(const char* paramName, const void* pData, unsigned int sizeinByte, unsigned int index) = 0;

		virtual bool SetFloat(const char* paramName, float val) = 0;

		virtual bool SetInt(const char* paramName, int data) = 0;

		virtual bool SetTexture(const char*  paramName, RenderAPI::Texture2D* texture) = 0;

		virtual void CommitChange() = 0;
	};

	class SwapChain : public RObject
	{
	public:
		// 这个 RenderTarget 不再使用的时候要调用 Release
		virtual RenderTarget* GetRenderTarget() = 0;

		// 这个 RenderTarget 不再使用的时候要调用 Release
		virtual DepthStencil* GetDepthStencil() = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool OnResize(unsigned int width, unsigned int height) = 0;

		// Present失败的时候，有两个可能
		// 如果Present在BeginScene/EndScene之间调用，则表示调用失败
		// 否则表示设备丢失，使用Context::CheckDeviceLost来查询设备状态，以确认是否需要重建
		virtual DeviceState Present() = 0;
	};

	class OcclusionQuery : public RObject
	{
	public:
		virtual bool Begin() = 0;

		virtual void End() = 0;

		virtual bool Get(void* dataPtr, unsigned int length) = 0;
	};
}