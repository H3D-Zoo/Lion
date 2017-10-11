#pragma once

namespace RenderAPI
{
	enum RenderTargetFormat
	{
		RT_XRGB8 = 0,
		RT_ARGB8 = 1,
		RT_RenderTexture = 2,
	};

	enum DepthStencilFormat
	{
		DS_D24S8 = 0,
		DS_D24X8 = 1,
		DS_D16 = 2,
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

	enum ResourceUsage
	{
		RESUSAGE_StaticWO = 0,
		RESUSAGE_Static = 1,
		RESUSAGE_Dynamic = 2,
		RESUSAGE_StaticWOManaged = 3,
		RESUSAGE_StaticManaged = 4,
		RESUSAGE_DynamicManaged = 5,
	};

	enum LockOption
	{
		LOCK_Normal = 0,
		LOCK_Discard = 1,
		LOCK_ReadOnly = 2,
		LOCK_NoOverWrite = 3,
	};

	enum IndexFormat
	{
		INDEX_Int16 = 0,
		INDEX_Int32 = 1,
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
		SEMANTIC_Position = 0,
		SEMANTIC_Color = 1,
		SEMANTIC_Normal = 2,
		SEMANTIC_Texcoord = 3,
		SEMANTIC_Tangent = 4,
		SEMANTIC_Binormal = 5,
		SEMANTIC_BlendWeight = 6,
		SEMANTIC_BlendIndices = 7,
	};

	enum StreamFrequncy
	{
		FREQUENCY_Normal = 0,
		FREQUENCY_Indexed = 1,
		FREQUENCY_Instanced = 2,		
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
		TEX_R32F = 10,
		TEX_RG32F = 11,
		TEX_ARGB32F = 12
	};

	enum CubemapFace
	{
		CUBE_PositiveX = 0,
		CUBE_NegativeX = 1,
		CUBE_PositiveY = 2,
		CUBE_NegativeY = 3,
		CUBE_PositiveZ = 4,
		CUBE_NegativeZ = 5,
	};

	enum ImageFormat
	{
		IMG_BMP = 0,
		IMG_TGA = 1,
		IMG_PNG = 2,
		IMG_DDS = 3,
	};

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

	enum SamplerFilter
	{
		FILTER_None = 0,
		FILTER_Point = 1,
		FILTER_Linear = 2,
		FILTER_Anisotropic  = 3,
	};

	enum TextureAddress
	{
		TEX_ADDRESS_Repeat = 0,
		TEX_ADDRESS_Clamp = 1,
		TEX_ADDRESS_Mirror = 2,
		TEX_ADDRESS_Border = 2,
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

	//!对非2的幂texture的支持
	enum ENONPOW2Support
	{
		//!完全支持
		POW2_Support = 0,
		//!有条件支持：不支持DXT5，只允许CLAMP，不是用MIPMAPS
		POW2_Conditional = 1,
		//!不支持
		POW2_None = 2,
	};

	enum FogMode
	{
		FOG_Linear = 0,
		FOG_Exp = 1,
		FOG_ExpSquare = 2,
	};

	enum TextureTransform
	{
		TT_Disable = 0,
		TT_Dim2 = 1,
		TT_Dim3 = 2,
		TT_Dim4 = 3,
		TT_Dim4Proj = 4,
	};
	
	class Device;
	class Context;
	class VertexBuffer;
	class APIInstance;
	typedef unsigned int HEffectParam;
	const HEffectParam hInvalidParam = 0xFFFFFFFF;

	struct  Float3
	{
		Float3()
			:x(0.0f)
			,y(0.0f)
			,z(0.0f)
		{

		}
		float x;
		float y;
		float z;
	};
	// 如果 backbufferWidth 和 backbufferHeight 同时被设为 0，将会使用 hWindow 默认的大小作为 backbuffer 的大小
	struct SwapChainDesc
	{
		SwapChainDesc()
			: hWindow(0)
			, backbufferWidth(0)
			, backbufferHeight(0)
			, backbufferFormat(RT_XRGB8)
			, zbufferFormat(DS_D24S8)
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
	
	struct VertexElement
	{
		VertexElement()
			: StreamIndex(0)
			, SemanticName(SEMANTIC_Position)
			, SemanticIndex(0)
			, AlignOffset(0xFFFFFFFF)
			, Format(INPUT_Float4)
		{ }

		unsigned int StreamIndex;
		Semantic SemanticName;
		unsigned int SemanticIndex;
		unsigned int AlignOffset;
		InputFormat Format;
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

	struct FogSetting
	{
		FogSetting()
			:IsEnable(false)
			, TableMode(FOG_Linear)
			, StartDepth(1.0f)
			, EndDepth(1.0f)
			, FogDensity(1.0f)
			, FogColor(0xFFFFFFFF)
		{

		}

		bool IsEnable;
		FogMode TableMode;
		float StartDepth;
		float EndDepth;
		float FogDensity;
		unsigned int FogColor;
	};
	struct PointLight
	{
		PointLight()
			:IsEnable(false)
			, Diffuse()
			, Ambient()
			, Specular()
			, Position()
			, Range(0.0f)
		{

		}
		bool IsEnable;
		Float3 Diffuse;
		Float3 Ambient;
		Float3 Specular;
		Float3 Position;
		float Range;


	};
	struct DirectionalLight
	{
		DirectionalLight()
			:IsEnable(false)
			, Diffuse()
			, Ambient()
			, Specular()
			, Direction()
		{

		}
		bool IsEnable;
		Float3 Diffuse;
		Float3 Ambient;
		Float3 Specular;
		Float3 Direction;
	};
	struct Material
	{
		Material()
			: Diffuse()
			, DiffuseA(0.0f)
			, Ambient()
			, AmbientA(0.0f)
			, Specular()
			, SpecularA(0.0f)
			, Power(0.0f)
		{

		}
		Float3 Diffuse;
		float DiffuseA;
		Float3 Ambient;
		float AmbientA;
		Float3 Specular;
		float SpecularA;
		float Power;

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

	struct TextureSampler
	{
		TextureSampler()
			: MinFilter(FILTER_Point)
			, MagFilter(FILTER_Point)
			, MipFilter(FILTER_None)
			, AddressU(TEX_ADDRESS_Clamp)
			, AddressV(TEX_ADDRESS_Clamp)
			, AddressW(TEX_ADDRESS_Clamp)
			, BorderColor(0x00000000)
			, OptionalAnisotropicFilter(AA_Disable)
		{	}

		SamplerFilter MinFilter;
		SamplerFilter MagFilter;
		SamplerFilter MipFilter;
		TextureAddress AddressU;
		TextureAddress AddressV;
		TextureAddress AddressW;
		unsigned int BorderColor;
		AAMode OptionalAnisotropicFilter;
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
			, FrequencyMethod(FREQUENCY_Normal)
			, OptionalFrequencyInterval(1)
		{ }

		VertexBuffer* BufferPtr;
		unsigned int Offset;
		StreamFrequncy FrequencyMethod;
		unsigned int OptionalFrequencyInterval;
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

	// 系统日志设置函数
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

	class VertexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual unsigned int GetVertexCount() const = 0;

		virtual unsigned int GetVertexStride() const = 0;

		virtual unsigned int GetLength() const = 0;

		virtual void* Lock(unsigned int offset, unsigned int lockLength, LockOption lockOption) = 0;

		virtual void* LockAll(LockOption lockOption) = 0;

		virtual void Unlock() = 0;

		virtual bool NeedRecreateWhenDeviceLost() const = 0;
	};

	class VertexDeclaration : public RObject
	{
	public:
		virtual const VertexElement* GetElements() const = 0;

		virtual unsigned int GetElementCount() const = 0;
	};

	class IndexBuffer : public RObject
	{
	public:
		virtual ResourceUsage GetUsage() const = 0;

		virtual IndexFormat GetFormat() const = 0;

		virtual unsigned int GetIndexCount() const = 0;

		virtual unsigned int GetLength() const = 0;

		virtual void* Lock(unsigned int offset, unsigned int lockLength, LockOption lockOption) = 0;

		virtual void* LockAll(LockOption lockOption) = 0;

		virtual void Unlock() = 0;

		virtual bool NeedRecreateWhenDeviceLost() const = 0;
	};

	class Texture : public RObject
	{
	public:
		virtual TextureFormat GetFormat() const = 0;

		virtual void GenerateMipmaps() = 0;

		virtual bool AutoGenMipmaps() const = 0;

		virtual bool IsCubemap() const = 0;

		virtual bool NeedRecreateWhenDeviceLost() const = 0;
	};

	class TextureSurface : public RObject
	{
	public:
		virtual void* GetDC() = 0;

		virtual void ReleaseDC() = 0;

		virtual bool SaveToFile(const char* fileName, ImageFormat format) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual MappedResource LockRect(unsigned int layer, LockOption lockOption) = 0;

		virtual void UnlockRect(unsigned int layer) = 0;

		virtual unsigned int GetLayerCount() const = 0;

		virtual TextureSurface* GetSurface(unsigned int layer) = 0;

		virtual bool IsRenderTexture() const = 0;
		
		virtual bool SaveToFile(const char* fileName, ImageFormat format) = 0;
	};

	class TextureCube : public Texture
	{
	public:
		virtual unsigned int GetEdgeLength() const = 0;

		virtual MappedResource LockRect(CubemapFace face, unsigned int layer, LockOption lockOption) = 0;

		virtual void UnlockRect(CubemapFace face, unsigned int layer) = 0;
	};

	//如果这是一个通过Device创建出来的RT，那设备丢失的时候一定要释放掉
	class RenderTarget : public RObject
	{
	public:
		virtual RenderTargetFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual Texture2D* GetTexturePtr() = 0;
	};

	class DepthStencil : public RObject
	{
	public:
		virtual DepthStencilFormat GetFormat() const = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;
	};

	class FXEffect : public RObject
	{
	public:
		virtual unsigned int Begin(bool saveState) = 0;

		virtual void End() = 0;

		virtual bool BeginPass(unsigned int passIndex) = 0;

		virtual void EndPass() = 0;

		virtual void SetValidateTechnique() = 0;

		virtual const char* GetTechniqueName(HEffectParam) = 0;

		virtual HEffectParam GetTechniqueByName(const char* name) = 0;

		virtual HEffectParam GetTechniqueByID(unsigned int index) = 0;

		virtual HEffectParam GetParameterByName(const char* name) = 0;

		virtual HEffectParam GetParameterByName(HEffectParam parent, const char* name) = 0;

		virtual HEffectParam GetParameterElement(HEffectParam parent, unsigned int elementIndex) = 0;

		virtual bool SetTechniqueByName(const char* name) = 0;

		virtual bool SetMatrix(const char* paramName, const float* matrix) = 0;

		virtual bool SetMatrixTranspose(const char* paramName, const float* matrix) = 0;

		virtual bool SetMatrixInArray(const char* paramName, const float* matrix, unsigned int index) = 0;

		virtual bool SetMatrixTransposeInArray(const char* paramName, const float* matrix, unsigned int index) = 0;

		virtual bool SetMatrixArray(const char* paramName, const float* matrices, unsigned int count) = 0;

		virtual bool SetMatrixTransposeArray(const char* paramName, const float* matrices, unsigned int count) = 0;

		virtual bool SetValue(const char* paramName, const void* pValue, unsigned int sizeInByte) = 0;

		virtual bool SetValueInArray(const char* paramName, const void* pValue, unsigned int sizeInByte, unsigned int index) = 0;

		virtual bool SetFloat(const char* paramName, float fValue) = 0;

		virtual bool SetInt(const char* paramName, int iValue) = 0;

		virtual bool SetTexture(const char* paramName, Texture* texture) = 0;

		virtual bool SetTechnique(HEffectParam hParam) = 0;

		virtual bool SetMatrix(HEffectParam hParam, const float* matrix) = 0;

		virtual bool SetMatrixTranspose(HEffectParam hParam, const float* matrix) = 0;

		virtual bool SetMatrixInArray(HEffectParam hParam, const float* matrix, unsigned int index) = 0;

		virtual bool SetMatrixTransposeInArray(HEffectParam hParam, const float* matrix, unsigned int index) = 0;

		virtual bool SetMatrixArray(HEffectParam hParam, const float* matrices, unsigned int count) = 0;

		virtual bool SetMatrixTransposeArray(HEffectParam hParam, const float* matrices, unsigned int count) = 0;

		virtual bool SetValue(HEffectParam hParam, const void* pValue, unsigned int sizeInByte) = 0;

		virtual bool SetValueInArray(HEffectParam hParam, const void* pValue, unsigned int sizeInByte, unsigned int index) = 0;

		virtual bool SetFloat(HEffectParam hParam, float fValue) = 0;

		virtual bool SetInt(HEffectParam hParam, int iValue) = 0;

		virtual bool SetTexture(HEffectParam hParam, Texture* texture) = 0;

		virtual void SetTextureSampler(HEffectParam hParam, unsigned int index, TextureSampler sampler) = 0;

		virtual void CommitChange() = 0;

		virtual void OnLostDevice() = 0;

		virtual void OnResetDevice() = 0;
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
	class TextBox :public RObject
	{
	public:
		virtual void Release() = 0;

		virtual void SetPosSize(int x, int y, int width, int height) = 0;

		virtual void SetText(const char* str, bool bWordWrap) = 0;

		virtual void Clear() = 0;

		virtual void SetTextColor(float r, float g, float b, float a) = 0;

		virtual void Show(bool bShow) = 0;

		virtual void Render() = 0;

		virtual void OnLostDevice() = 0;

		virtual void OnResetDevice() = 0;
	};
	class OcclusionQuery : public RObject
	{
	public:
		virtual bool Begin() = 0;

		virtual void End() = 0;

		virtual bool Get(void* dataPtr, unsigned int length) = 0;
	};

	// 用户渲染接口
	class Device : public RObject
	{
	public:
		// 这个 RenderTarget 不再使用的时候要调用 Release
		virtual SwapChain* GetDefaultSwapChain() = 0;

		virtual SwapChain* CreateAdditionalSwapChain(const SwapChainDesc&) = 0;

		virtual VertexBuffer* CreateVertexBuffer(ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize) = 0;

		virtual IndexBuffer* CreateIndexBuffer(ResourceUsage usage, IndexFormat format, unsigned int indexCount) = 0;

		virtual VertexDeclaration* CreateVertexDeclaration(const VertexElement* elements, unsigned int elementCount) = 0;

		virtual Texture2D* CreateTexture2D(ResourceUsage usage, TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps) = 0;

		virtual TextureCube* CreateTextureCube(ResourceUsage usage, TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps) = 0;

		virtual FXEffect* CreateFXEffectFromFile(const char* effectFilePath, const char * includeDir) = 0;

		virtual RenderTarget* CreateRenderTarget(TextureFormat format, unsigned int width, unsigned int height) = 0;

		virtual DepthStencil* CreateDepthStencil(DepthStencilFormat format, unsigned int width, unsigned int height) = 0;

		virtual OcclusionQuery* CreateOcclusionQuery() = 0;

		virtual TextBox* CreateTextBox(int screenX, int screenY, int width, int height) = 0;
	};
	
	enum LegacyFVF
	{
		FVF_XYZ = 1 << 0,
		FVF_NORMAL = 1 << 1,
		FVF_DIFFUSE = 1 << 2,
		FVF_SPECULAR = 1 << 3,
		FVF_TEXCOORD0 = 1 << 4,
		FVF_TEXCOORD1 = 1 << 5,
		FVF_TEXCOORD2 = 1 << 6,
		FVF_TEXCOORD3 = 1 << 7,
		FVF_TEXCOORD4 = 1 << 8,
		FVF_TEXCOORD5 = 1 << 9,
		FVF_TEXCOORD6 = 1 << 10,
		FVF_TEXCOORD7 = 1 << 11,
	};

	class ContextLegacy
	{
	public:
		//为了引擎临时加上去的，等一下一步在去掉吧
		virtual void* GetImplementPtr() = 0;

		virtual void SetTextureColorBlendingState(unsigned int slot, const TextureBlendingState& state) = 0;

		virtual void SetTextureAlphaBlendingState(unsigned int slot, const TextureBlendingState& state) = 0;

		virtual void SetTexcoordTransform(unsigned int slot, TextureTransform transform) = 0;

		virtual void ResetTexcoordIndex(unsigned int slot) = 0;

		virtual void DisableCustomShaderUsage() = 0;

		virtual void SetCustomFVF(unsigned int fvf) = 0;

		virtual void SetWorldMatrix(const float* matrix) = 0;

		virtual void SetViewMatrix(const float* matrix) = 0;

		virtual void SetProjectionMatrix(const float* matrix) = 0;

		virtual void SetTextureMatrix(unsigned int slot, const float* matrix) = 0;
		
		virtual void SetFog(const FogSetting& fog) = 0;

		virtual void SetLight(const PointLight& pLight) = 0;

		virtual void SetLight(const DirectionalLight& pLight) = 0;

		virtual void DisableLight() = 0;

		virtual void SetMaterial(const Material& mat) = 0;

		virtual void GenPerspectiveMatrixRH(float outMatrix[16], float fovRadian, float aspect, float nearZ, float farZ) = 0;

		virtual void GenOrthoCenterMatrixRH(float outMatrix[16], float width, float height, float nearZ, float farZ) = 0;

		virtual void GenOrthoOffCenterMatrixRH(float outMatrix[16], float left, float right, float bottom, float top, float nearZ, float farZ) = 0;

		virtual void GenViewMatrix(float outMatrix[16], const Float3& eye, const Float3& lookAt, const Float3& upward) = 0;

		virtual void ProjectVertexPos(Float3& inoutPos, const float matMV[16], const float matP[16], Viewport viewport) = 0;

		virtual void SaveNXDebugRenderState() = 0;

		virtual void RestoreNXDebugRenderState(bool lightsOn) = 0;
	};

	// 渲染命令接口
	class Context : public RObject
	{
	public:
		virtual DeviceCaps GetDeviceCaps() = 0;

		virtual unsigned int GetAvailableTextureMemory() = 0;

		virtual void ClearRenderTarget(unsigned int color) = 0;

		virtual void ClearDepthBuffer(float z) = 0;

		virtual void ClearStencilBuffer(unsigned int stencil) = 0;

		virtual void SetViewport(const Viewport&) = 0;

		virtual Viewport GetViewport() = 0;

		virtual void SetRenderTarget(unsigned int index, RenderTarget* renderTarget) = 0;

		virtual void SetDepthStencil(DepthStencil* depthStencil) = 0;

		virtual void SetVertexBuffers(VertexBufferInfo* buffers, unsigned int bufferCount) = 0;

		virtual void SetIndexBuffer(IndexBuffer* buffer, unsigned int offset) = 0;

		virtual void SetVertexDeclaration(VertexDeclaration* decl) = 0;

		virtual void SetTexture(unsigned int slot, Texture* textures) = 0;

		virtual void SetBlendState(const BlendState& state) = 0;

		virtual BlendState GetBlendState() const = 0;

		virtual void SetAlphaSeparateBlendState(const BlendState& state) = 0;

		virtual BlendState GetAlphaSeparateBlendState() const = 0;

		virtual void SetAlphaTestingState(const AlphaTestingState& state) = 0;

		virtual AlphaTestingState GetAlphaTestingState() const = 0;

		virtual void SetDepthTestingState(const DepthTestingState& state) = 0;

		virtual DepthTestingState GetDepthTestingState() const = 0;

		virtual void SetClipPlaneState(bool isEnable) = 0;

		virtual bool GetClipPlaneState() const = 0;

		virtual void SetStencilTestingState(const StencilTestingState& state) = 0;

		virtual StencilTestingState GetStencilTestingState() const = 0;

		virtual void SetDepthWriting(bool enable) = 0;

		virtual void SetTextureSampler(unsigned int slot, const TextureSampler& sampler) = 0;

		virtual void SetScissorState(const ScissorState& state) = 0;

		virtual ScissorState GetScissorState() const = 0;

		virtual void SetColorWriteMask(bool r, bool g, bool b, bool a) = 0;

		virtual void SetFillMode(FillMode mode) = 0;

		virtual FillMode GetFillMode() const = 0;

		virtual void SetCullMode(CullMode mode) = 0;

		virtual CullMode GetCullMode() const = 0;

		virtual void SetDepthBias(float bias) = 0;

		virtual void SetSlopScaleDepthBias(float bias) = 0;

		virtual void SetTextureFactor(unsigned int factor) = 0;

		virtual bool BeginScene() = 0;

		virtual void EndScene() = 0;

		virtual void Draw(Primitive primitive, unsigned int startVertex, unsigned int primitiveCount) = 0;

		virtual void DrawWithDynamicVertex(Primitive primitive, unsigned int primitiveCount, const void* pVertexData, unsigned int vertexStride) = 0;

		virtual void DrawIndexed(Primitive primitive, unsigned int baseVertex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount) = 0;

		virtual void DrawIndexedWithDynamicVertex(Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned int* pIndexData, const void* pVertexData, unsigned int vertexStride) = 0;

		virtual void DrawIndexedWithDynamicVertex(Primitive primitive, unsigned int vertexCount, unsigned int primitiveCount, const unsigned short* pIndexData, const void* pVertexData, unsigned int vertexStride) = 0;

		virtual bool UpdateTexture(Texture2D* src, Texture2D* dst) = 0;
		
		virtual bool StretchTexture(Texture2D* src, Texture2D* dst, SamplerFilter filter) = 0;

		virtual DeviceState Present() = 0;

		virtual DeviceState GetState() = 0;

		virtual DeviceState ResetDevice(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync) = 0;

		virtual void EvictManagedResources() = 0;

		virtual ContextLegacy* GetContextLegacy() = 0;
	};

	// RenderAPI 总入口
	class APIInstance
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
		virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile, const char* includeDir) = 0;

		virtual bool CheckMultiSampleSupport(RenderTargetFormat, DepthStencilFormat, AAMode, bool fullscreen) const = 0;

		virtual RenderTargetFormat GetDefaultRenderTargetFormat() = 0;

		virtual const char* GetDeviceDriver() const = 0;

		virtual const char* GetDeviceName() const = 0;

		virtual const char* GetDeviceDesc() const = 0;

		virtual unsigned int GetVendorID() const = 0;

		virtual DriverVersion GetDriverVersion() const = 0;

		virtual bool IsSupportEx() const = 0;

		virtual void PerfBegin(unsigned int color, const char* name) = 0;

		virtual void PerfMark(unsigned int color, const char* name) = 0;

		virtual void PerfEnd() = 0;
	};

	// APIInstance 初始化接口
	APIInstance* CreateAPIInstance();
}
