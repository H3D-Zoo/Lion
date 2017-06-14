#pragma once

#include <RenderAPI.h>

class APITestBed
{
public:
	bool Init();
	void Deinit();
	void Update();
	void OnResize(unsigned int width, unsigned int height);
private:
	RenderAPI::Device* m_pDevice = nullptr;
	RenderAPI::Context* m_pContext = nullptr;
};