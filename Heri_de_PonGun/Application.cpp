/*
*	アプリケーションの初期化を行うクラス
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <crtdbg.h>
#include <cassert>

LPDIRECT3DDEVICE9		d3dDevice = 0;
LPDIRECTINPUTDEVICE8	dinputDevice = 0;

DWORD FloatToDWORD(float val){ return *((DWORD *)&val); }

//	コンストラクタ
Application::Application(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	DebugLog("アプリケーションを生成しました\n");

	//	アプリケーションの初期化
	Initialize(wndTitle, wndRect, fullscreen_f, hInst, cmdShow);
}

Application::~Application()
{
	DebugLog("アプリケーションを破棄しました。\n");

	// アプリケーションの終了処理をする
	Release();
}

//	アプリケーションの初期化
void Application::Initialize(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	//	ウィンドウの初期化
	if (!InitWindow(wndTitle, wndRect, fullscreen_f, hInst, cmdShow))
		DebugAlert("ウィンドウを\n初期化できませんでした。");

	//	Direct3Dの初期化
	if (!InitDirect3d())
		DebugAlert("Direct3D\nを初期化できませんでした。");

	//	プレゼンテーションパラメータの初期化
	if (!InitPresentParam(wndRect, fullscreen_f))
		DebugAlert("プレゼンテーションパラメータを\n初期化できませんでした。");

	if (!InitD3dDevice())
		DebugAlert("Direct3Dデバイスを\n初期化できませんでした。");

	DebugLog("アプリケーションを初期化しました。\n");
}

//	アプリケーションの解放
void Application::Release()
{
	//	デバッグビルド時に確認用のビープ音を鳴らす
#if defined(DEBUG) | defined(_DEBUG)
	MessageBeep(MB_OK);
#endif

	//	コンソールを閉じる
	CloseConsole();

	SafeRelease(direct3d);
}

//	ループ処理
bool Application::Loop()
{
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (WM_QUIT == msg.message)
		{
			return false;
		}
	}

	return true;
}

//	ウィンドウハンドルの取得
HWND Application::GetWndHandle()
{
	return hWnd;
}

//	ウィンドウプロシージャ
LRESULT CALLBACK Application::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//	Windowからのメッセージを処理する
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

//	ウィンドウの初期化
bool Application::InitWindow(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	//	ウィンドウクラスの初期化
	WNDCLASSEX winClassEx;
	winClassEx.cbSize = sizeof(WNDCLASSEX);
	winClassEx.style = 0;
	winClassEx.lpfnWndProc = WinProc;
	winClassEx.cbClsExtra = 0;
	winClassEx.cbWndExtra = 0;
	winClassEx.hInstance = hInst;
	winClassEx.hIcon = LoadIcon(hInst, (LPCTSTR)IDC_ICON);
	winClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winClassEx.hbrBackground = nullptr;
	winClassEx.lpszMenuName = nullptr;
	winClassEx.lpszClassName = putf("%sWindowClass", wndTitle);
	winClassEx.hIconSm = LoadIcon(winClassEx.hInstance, (LPCTSTR)IDC_ICON);

	//	ウィンドウクラスの登録
	if (!RegisterClassEx(&winClassEx))
		return false;

	//	ウィンドウを作成
	if (fullscreen_f)
	{
		// フルスクリーンモードで表示する場合は
		// 画面いっぱいにウィンドウを広げる
		hWnd = CreateWindow(winClassEx.lpszClassName, wndTitle, WS_OVERLAPPEDWINDOW,
			0, 0, wndRect.right, wndRect.bottom,
			nullptr, nullptr, hInst, nullptr);
	}
	else
	{
		// ウィンドウモードで表示する場合は
		// デフォルトのウィンドウ位置に表示する
		AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);
		hWnd = CreateWindow(winClassEx.lpszClassName, wndTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
			nullptr, nullptr, hInst, nullptr);
	}

	//	ウィンドウを表示する
	if (FAILED(ShowWindow(hWnd, cmdShow)))
		return false;

	//	 WM_PAINTを受け取らないようにする
	ValidateRect(hWnd, 0);
	return true;
}

//	Direct3Dの初期化
bool Application::InitDirect3d()
{
	//	Direct3D9オブジェクトの作成
	if ((direct3d = Direct3DCreate9(D3D_SDK_VERSION)) == 0)
		return false;

	return true;
}

//	プレゼンテーションパラメータの初期化
bool Application::InitPresentParam(RECT wndRect, bool fullscreen_f)
{
	D3DDISPLAYMODE *displayMode = new D3DDISPLAYMODE();

	//	現在のディスプレイモードを取得
	if (FAILED(direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, displayMode)))
		return false;

	//	デバイスのプレゼンテーションパラメータを初期化
	ZeroMemory(&presentParam, sizeof(D3DPRESENT_PARAMETERS));
	presentParam.BackBufferCount = 1;
	if (fullscreen_f)
	{
		//	フルスクリーンの場合
		presentParam.Windowed = false;						//	フルスクリーン表示の設定
		presentParam.BackBufferWidth = wndRect.right;		//	フルスクリーン時の横幅
		presentParam.BackBufferHeight = wndRect.bottom;		//	フルスクリーン時の縦幅
	}
	else
	{
		presentParam.Windowed = true;						//	ウィンドウ内表示の指定
	}
	presentParam.BackBufferFormat = displayMode->Format;	//	カラーモードの指定
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.EnableAutoDepthStencil = true;				//	エラー対策
	presentParam.AutoDepthStencilFormat = D3DFMT_D16;		//	エラー対策

	delete displayMode;
	return true;
}

//	Direct3Dデバイスの初期化
bool Application::InitD3dDevice()
{
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
				return false;

	//レンダリングステータスの設定
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//両面描画モードの設定
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);				//Z比較を行う
	d3dDevice->SetRenderState(D3DRS_DITHERENABLE, true);		//ディザリングを行う

	// ポイントスプライトの設定
	d3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// ポイントスケールの係数
	d3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	d3dDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(1.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));
	return true;
}

//	DirectInputの初期化
bool Application::InitDirectInput(HINSTANCE hInst)
{
	//	DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr)))
		return false;

	return true;
}

//	DirectInputデバイスの初期化
bool Application::InitDinputDevice()
{
	//	DirectInputデバイスを作成
	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &dinputDevice, nullptr)))
		return false;

	//	データフォーマットを設定
	if (FAILED(dinputDevice->SetDataFormat(&c_dfDIMouse2)))
		return false;

	//	協調モードを設定
	if (FAILED(dinputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	//	軸モードを設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(dinputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "設定の失敗", "Direct Input Error", MB_OK);
		return false;
	}

	//	入力制御開始
	dinputDevice->Acquire();

	return true;
}





