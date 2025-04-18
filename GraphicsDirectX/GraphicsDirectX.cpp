#include "targetver.h" // 현재 설치된 가장 최신의 SDK 버전을 사용하겠다는 선언
#include <Windows.h>
#include "D3DRenderer.h"

D3DRenderer renderer;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY :
		{
			PostQuitMessage(0); // 종료
			return 0;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	const wchar_t CLASS_NAME[] = L"MyWindowClass";

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hWnd = CreateWindowEx(0,
														     CLASS_NAME,
														     L"DirectX 기초 화면",
														     WS_OVERLAPPEDWINDOW,
														     CW_USEDEFAULT,
														     CW_USEDEFAULT,
														     800,
														     600,
														     nullptr,
														     nullptr,
														     hInstance,
														     nullptr
		);

	if (!hWnd)
	{
		return 0;
	}

	if (!renderer.Init(hWnd))
	{
		MessageBox(hWnd, L"DirectX 초기화 실패", L"에러", MB_OK);
		return -1;
	}

	renderer.CreateTriangleResources();

	ShowWindow(hWnd, nCmdShow);

	

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		renderer.ClearScreen(1.0f, 1.0f, 1.0f, 1.0f);
		renderer.DrawTriangle();
		renderer.Present();
	}

	return 0;

}
