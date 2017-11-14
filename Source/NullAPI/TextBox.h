#pragma once
#include <string>
#include "../../RenderAPI/RenderAPI.h"
#include "RefCount.hpp"

class TextBox : public RenderAPI::TextBox
{
public:
	TextBox(int screen_x, int screen_y, int width, int height);

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
	RefCount m_refCount;
	std::string m_text;
	RECT m_rect;
	float m_color[4];
	bool m_wordWarp;
	bool m_isShow;
};
