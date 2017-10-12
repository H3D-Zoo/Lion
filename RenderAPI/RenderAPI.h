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
		RESUSAGE_Default = 0,
		RESUSAGE_Dynamic = 1,
		RESUSAGE_Immuable = 2,
		RESUSAGE_DefaultRW = 3,
		RESUSAGE_DynamicRW = 4,
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

	//!�Է�2����texture��֧��
	enum ENONPOW2Support
	{
		//!��ȫ֧��
		POW2_Support = 0,
		//!������֧��
		POW2_Conditional = 1,
		//!��֧��
		POW2_None = 2,
	};
	
	class Device;
	class Context;
	class VertexBuffer;
	typedef unsigned int HEffectParam;
	const HEffectParam hInvalidParam = 0xFFFFFFFF;

	// ��� backbufferWidth �� backbufferHeight ͬʱ����Ϊ 0������ʹ�� hWindow Ĭ�ϵĴ�С��Ϊ backbuffer �Ĵ�С
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
		//!���texture���
		unsigned int MaxTextureWidth;
		//!���texture�߶�
		unsigned int MaxTextureHeight;
		//!���������Գ̶�
		unsigned int MaxAnisotropy;
		//!���texture����
		unsigned int MaxTextureStage;
		//!���ͬʱ�󶨵�texture��
		unsigned int MaxSimultaneousTextures;
		//!�û��ü��������
		unsigned int MaxUserClipPlanes;
		//!һ�λ��Ƶ����ͼԭ��
		unsigned int MaxPrimitiveCount;
		//!��󶥵�������
		unsigned int MaxVertexIndex;
		//!��󶥵���������
		unsigned int MaxStreams;
		//!��󶥵���
		unsigned int MaxStreamStride;
		//!���VS�Ĵ�������
		unsigned int MaxVertexShaderConsts;
		//!���ͬʱrender target��
		unsigned int MaxMRTs;
		//!VS���ָ����
		unsigned int MaxVertexShaderInstruction;
		//!PS���ָ����
		unsigned int MaxPixelShaderInstruction;
		//!���vertex blend������
		unsigned int MaxVertexBlendMatrix;
		//!���vertex blend������
		unsigned int MaxVertexBlendMatrixIndex;
		//!VS�汾����һ����λ������
		unsigned int VertexShaderVersion;
		//!PS�汾����һ����λ������
		unsigned int PixelShaderVersion;
		//!�Ƿ�֧��32λ����
		bool SupportIndex32;
		//!�Ƿ�֧��dynamic texture
		bool SupportsDynamicTexture;
		//֧����ͼ͸��ͨ��
		bool SupportTextureAlphaChannel;
		//!��֧����������ͼ
		bool SupportOnlySquareTexture;
		//!��pow2��ͼ֧��
		ENONPOW2Support NonePOW2Support;
		//!��ʼ�Դ���
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

	// ϵͳ��־���ú���
	// �ⲿ�û��̳в�ʵ�ִ˽ӿڣ����ض�����Զ��崦��ϵͳ�ڲ���Log��
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

		virtual void* DiscardLock() = 0;

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

		virtual void* DiscardLock() = 0;

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

	//�������һ��ͨ��Device����������RT�����豸��ʧ��ʱ��һ��Ҫ�ͷŵ�
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
		// ��� RenderTarget ����ʹ�õ�ʱ��Ҫ���� Release
		virtual RenderTarget* GetRenderTarget() = 0;

		// ��� RenderTarget ����ʹ�õ�ʱ��Ҫ���� Release
		virtual DepthStencil* GetDepthStencil() = 0;

		virtual unsigned int GetWidth() const = 0;

		virtual unsigned int GetHeight() const = 0;

		virtual bool OnResize(unsigned int width, unsigned int height) = 0;

		// Presentʧ�ܵ�ʱ������������
		// ���Present��BeginScene/EndScene֮����ã����ʾ����ʧ��
		// �����ʾ�豸��ʧ��ʹ��Context::CheckDeviceLost����ѯ�豸״̬����ȷ���Ƿ���Ҫ�ؽ�
		virtual DeviceState Present() = 0;
	};

	class OcclusionQuery : public RObject
	{
	public:
		virtual bool Begin() = 0;

		virtual void End() = 0;

		virtual bool Get(void* dataPtr, unsigned int length) = 0;
	};

	// �û���Ⱦ�ӿ�
	class Device : public RObject
	{
	public:
		// ��� RenderTarget ����ʹ�õ�ʱ��Ҫ���� Release
		virtual SwapChain* GetDefaultSwapChain() = 0;

		virtual SwapChain* CreateAdditionalSwapChain(const SwapChainDesc&) = 0;

		virtual VertexBuffer* CreateVertexBuffer(ResourceUsage usage, unsigned int vertexCount, unsigned int vertexSize, void* initialData) = 0;

		virtual IndexBuffer* CreateIndexBuffer(ResourceUsage usage, IndexFormat format, unsigned int indexCount, void* initialData) = 0;

		virtual VertexDeclaration* CreateVertexDeclaration(const VertexElement* elements, unsigned int elementCount) = 0;

		virtual Texture2D* CreateTexture2D(ResourceUsage usage, TextureFormat format, unsigned int width, unsigned int height, unsigned int layer, bool autoGenMipmaps, void* initialData, int dataLinePitch, int dataHeight) = 0;

		virtual TextureCube* CreateTextureCube(ResourceUsage usage, TextureFormat format, unsigned int edgeLength, unsigned int layer, bool autoGenMipmaps, void** initialData, int dataLinePitch, int dataHeight) = 0;

		virtual FXEffect* CreateFXEffectFromFile(const char* effectFilePath, const char * includeDir) = 0;

		virtual RenderTarget* CreateRenderTarget(TextureFormat format, unsigned int width, unsigned int height) = 0;

		virtual DepthStencil* CreateDepthStencil(DepthStencilFormat format, unsigned int width, unsigned int height) = 0;

		virtual OcclusionQuery* CreateOcclusionQuery() = 0;

		//Ϊ��������ʱ����ȥ�ģ���һ��һ����ȥ����
		virtual void* GetImplementPtr() = 0;
	};
	
	// ��Ⱦ����ӿ�
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

		virtual bool GetRenderTargetData(RenderTarget* rt, TextureSurface* surface) = 0;

		virtual bool GetDepthStencilData(DepthStencil* ds, TextureSurface* surface) = 0;

		virtual DeviceState Present() = 0;

		virtual DeviceState GetState() = 0;

		virtual DeviceState ResetDevice(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync) = 0;

		virtual void EvictManagedResources() = 0;
	};

	// RenderAPI �����
	class APIInstance
	{
	public:
		// ������������û��ӿڶ��ͷź�����������ӿ�
		virtual void Release() = 0;

		// ���ϵͳ����Ĭ��Log����
		virtual Logger* GetDefaultLogger() = 0;

		// ��ȡ��ǰϵͳ����ʹ�õ�Log����
		virtual Logger* GetCurrentLogger() = 0;

		// �޸�ϵͳʹ�õ�Log�������Զ���log���λ�á�
		// ������Ϊnullptr��ʱ��ϵͳ��ʹ��Ĭ�ϵ�log���������
		virtual void SetCurrentLogger(Logger*) = 0;

		// �û��ӿڴ���
		virtual CreationResult CreateDeviceAndContext(const SwapChainDesc& desc, bool isFullscreen, bool useVerticalSync) = 0;

		// ���ļ��ж�ȡfx���벢���룬��������compiledFXFile��
		virtual bool CompileFXEffectFromFile(const char* sourceFXFile, const char* compiledFXFile, const char* includeDir) = 0;

		virtual bool CheckMultiSampleSupport(RenderTargetFormat, DepthStencilFormat, AAMode, bool fullscreen) const = 0;

		virtual RenderTargetFormat GetDefaultRenderTargetFormat() = 0;

		virtual const char* GetDeviceDriver() const = 0;

		virtual const char* GetDeviceName() const = 0;

		virtual const char* GetDeviceDesc() const = 0;

		virtual unsigned int GetVendorID() const = 0;

		virtual DriverVersion GetDriverVersion() const = 0;

		virtual void PerfBegin(unsigned int color, const char* name) = 0;

		virtual void PerfMark(unsigned int color, const char* name) = 0;

		virtual void PerfEnd() = 0;
	};

	// APIInstance ��ʼ���ӿ�
	APIInstance* CreateAPIInstance();
}
