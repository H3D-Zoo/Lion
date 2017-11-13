#include "TextBox.h"

TextBox::TextBox( int screen_x, int screen_y, int width, int height)
	: m_wordWarp(true)
	, m_isShow(true)
{
	m_rect.left = screen_x;
	m_rect.top = screen_y;
	m_rect.right = screen_x + width;
	m_rect.bottom = screen_y + height;
}

void TextBox::SetPosSize(int x, int y, int width, int height)
{
	m_rect.left = x;
	m_rect.top = y;
	m_rect.right = x + width;
	m_rect.bottom = y + height;
}

void TextBox::SetText(const char* str, bool bWordWarp)
{
	if (str == NULL)
	{
		m_text.clear();
	}
	else
	{
		m_text = str;
	}

	m_wordWarp = bWordWarp;
}

void TextBox::Clear()
{
	SetText("", m_wordWarp);
}

void TextBox::SetTextColor(float r, float g, float b, float a)
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;
}

void TextBox::Show(bool bShow)
{
	m_isShow = bShow;
}

void TextBox::Render()
{

}

void TextBox::OnLostDevice()
{

}

void TextBox::OnResetDevice()
{

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
