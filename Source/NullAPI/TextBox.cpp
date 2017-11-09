#include "TextBox.h"
TextBox::TextBox(ID3DXFont* pFont, int screen_x, int screen_y, int width, int height)
	: m_pFont(pFont)
	, m_displayText(NULL)
	, m_wordWarp(true)
	, m_isShow(true)
{
	m_rect.left = screen_x;
	m_rect.top = screen_y;
	m_rect.right = screen_x + width;
	m_rect.bottom = screen_y + height;
}

TextBox::~TextBox()
{
	m_pFont->Release();
	m_pFont = NULL;
}

void TextBox::LazyUpdateRect(RECT & out, LPD3DXFONT font, const RECT & in, UINT dtFormat) const
{
	if (m_displayText != m_text.c_str())
	{
		m_displayText = m_text.c_str();
		out.left = in.left;
		out.top = in.top;

		if (in.top <= in.bottom && in.right <= in.left)
		{// 系统自适应大小
			out.right = 0;
			out.bottom = 0;
		}
		else if (in.left < in.right && in.top < in.bottom)
		{// 用户使用固定大小
			out.right = in.right;
			out.bottom = in.bottom;
		}
		else
		{// 根据宽计算高或者根据高计算宽
			if (in.left < in.right)
			{
				out.right = in.right;
				out.bottom = 0;
			}
			else if (in.top < in.bottom)
			{
				out.right = 0;
				out.bottom = in.bottom;
			}

			if (m_wordWarp)
				dtFormat |= DT_WORDBREAK;
			font->DrawText(NULL, m_displayText, -1, &out, dtFormat | DT_CALCRECT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

void TextBox::SetPosSize(int x, int y, int width, int height)
{
	m_rect.left = x;
	m_rect.top = y;
	m_rect.right = x + width;
	m_rect.bottom = y + height;

	// always dirty
	m_displayText = NULL;
}

void TextBox::SetText(const char * str, bool bWordWarp)
{
	bool dirty = true;
	if (!str)
		m_text.clear();
	else if (strcmp(m_text.c_str(), str) != 0)
		m_text = str;
	else
		dirty = false;

	if (m_wordWarp != bWordWarp)
	{
		m_wordWarp = bWordWarp;
		dirty = true;
	}

	if (dirty)
		m_displayText = NULL;
}

void TextBox::Clear()
{
	SetText("", m_wordWarp);
}

void TextBox::SetTextColor(float r, float g, float b, float a)
{
	m_color.r = r;
	m_color.g = g;
	m_color.b = b;
	m_color.a = a;
}

void TextBox::Show(bool bShow)
{
	m_isShow = bShow;
}

void TextBox::Render()
{
	if (!m_text.empty() && m_isShow)
	{
		UINT dtFormat = DT_LEFT | DT_EXPANDTABS;
		if (m_wordWarp)
			dtFormat |= DT_WORDBREAK;

		LazyUpdateRect(m_rect, m_pFont, m_rect, dtFormat);
		m_pFont->DrawText(NULL, m_displayText, -1, &m_rect, dtFormat | DT_NOCLIP, m_color);
	}
}

void TextBox::OnLostDevice()
{
	m_pFont->OnLostDevice();
}

void TextBox::OnResetDevice()
{
	m_pFont->OnResetDevice();
}

unsigned int TextBox::AddReference()
{
	return ++m_refCount;
}

void TextBox::Release()
{
	if (0 == --m_refCount)
	{
		delete this;
	}
}
