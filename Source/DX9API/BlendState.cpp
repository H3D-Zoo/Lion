#include "RHIImpl.h"

BlendState::BlendState(bool enable, bool alphaSeperate, H3D::BlendFactor colorSrc, H3D::BlendFactor colorDst, H3D::BlendOperator colorOp, H3D::BlendFactor alphaSrc, H3D::BlendFactor alphaDst, H3D::BlendOperator alphaOp)
	: m_isEnable(enable)
	, m_isAlphaSeperate(alphaSeperate)
	, m_colorSrcFacfor(colorSrc)
	, m_colorDstFacfor(colorDst)
	, m_alphaSrcFacfor(alphaSrc)
	, m_alphaDstFacfor(alphaDst)
	, m_colorOperator(colorOp)
	, m_alphaOperator(alphaOp)
{

}

bool BlendState::IsEnabled() const
{
	return m_isEnable;
}

H3D::BlendFactor BlendState::GetSrcColorFactor() const
{
	return m_colorSrcFacfor;
}

H3D::BlendFactor BlendState::GetDstColorFactor() const
{
	return m_colorDstFacfor;
}

H3D::BlendFactor BlendState::GetSrcAlphaFactor() const
{
	return m_alphaSrcFacfor;
}

H3D::BlendFactor BlendState::GetDstAlphaFactor() const
{
	return m_alphaDstFacfor;
}

H3D::BlendOperator BlendState::GetColorOperator() const
{
	return m_colorOperator;
}

H3D::BlendOperator BlendState::GetAlphaOperator() const
{
	return m_alphaOperator;
}

bool BlendState::IsAlphaSeperate() const
{
	return m_isAlphaSeperate;
}
