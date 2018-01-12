#pragma once
#include "../../RenderAPI/RenderAPI.h"

class BaseAPIInstance : public RenderAPI::APIInstance
{
public:
	BaseAPIInstance();

	// ���ϵͳ����Ĭ��Log����
	virtual RenderAPI::Logger* GetDefaultLogger() const;

	// ��ȡ��ǰϵͳ����ʹ�õ�Log����
	virtual RenderAPI::Logger* GetCurrentLogger() const;

	// �޸�ϵͳʹ�õ�Log�������Զ���log���λ�á�
	// ������Ϊnullptr��ʱ��ϵͳ��ʹ��Ĭ�ϵ�log���������
	virtual void SetCurrentLogger(RenderAPI::Logger*);

protected:
	RenderAPI::Logger* m_currentLogger;
};