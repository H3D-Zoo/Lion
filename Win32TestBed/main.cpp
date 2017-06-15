#include "pch.h"
#include "testbed.h"

HINSTANCE hInst;
HWND hWnd;

std::string s_WindowTitle = "Win32 render API testbed";
std::string s_WindowClassName = "Win32_RenderAPI_TestBed";

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int, APITestBed&);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	APITestBed testbed;
	if (!InitInstance(hInstance, nCmdShow, testbed))
	{
		return FALSE;
	}

	MSG msg;
	while (true)
	{
		if (TRUE == PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		testbed.Update();
	}

	testbed.Deinit();
	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = s_WindowClassName.c_str();
	wcex.hIconSm = nullptr;

	return RegisterClassExA(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, APITestBed& testbed)
{
	hInst = hInstance; // 将实例句柄存储在全局变量中

	hWnd = CreateWindowA(
		s_WindowClassName.c_str(),
		s_WindowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	if (testbed.Init(hWnd, 0, 0))
	{
		SetWindowLongPtrA(hWnd, GWLP_USERDATA, (LONG)&testbed);
	}
	else
	{
		testbed.Deinit();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
	{
		APITestBed* testbed = (APITestBed*)GetWindowLongPtrA(hWnd, GWLP_USERDATA);
		testbed->OnResize(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}