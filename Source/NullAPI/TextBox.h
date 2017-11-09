#pragma once
#include "../../RenderAPI/RenderAPI.h"
#include "DX9Include.h"
#include <string>
#include "RefCount.hpp"

class TextBox : public RenderAPI::TextBox
{
public:
	TextBox(ID3DXFont* pFont, int screen_x, int screen_y, int width, int height);

	~TextBox();
	virtual void SetPosSize(int x, int y, int width, int height);

	virtual void SetText(const char* str, bool bWordWarp);

	virtual void Clear();

	virtual void SetTextColor(float r, float g, float b, float a);

	virtual void Show(bool bShow);

	virtual void Render();

	virtual void OnLostDevice();

	virtual void OnResetDevice();

	virtual unsigned int AddReference();

	virtual void Release();

private:
	void LazyUpdateRect(RECT& out, LPD3DXFONT font, const RECT& in, UINT dtFormat) const;
	RefCount m_refCount;
	std::string m_text;
	D3DXCOLOR m_color;
	RECT m_rect;
	bool m_wordWarp;
	bool m_isShow;
	LPD3DXFONT m_pFont;
	mutable const char* m_displayText; // dirty flag
};
