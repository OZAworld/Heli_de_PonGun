/*
*	アプリケーションの初期化を行うクラス
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//	アプリケーション実行環境を制御するクラス
class Application
{
private:

	HWND hWnd;								//	ウィンドウのハンドル

	LPDIRECT3D9 direct3d;					//	Direct3Dオブジェクト
	D3DPRESENT_PARAMETERS presentParam;		//	プレゼンテーションパラメータ
	LPDIRECTINPUT8 directInput;				//	DirectInputオブジェクト

public:

	//	コンストラクタ
	Application(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	デストラクタ
	~Application();

	//	アプリケーションの初期化
	void Initialize(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	アプリケーションの解放
	void Release();

	//	ループ処理
	bool Loop();

	//	ウィンドウハンドルの取得
	HWND GetWndHandle();

private:

	//	ウィンドウプロシージャ
	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//	ウィンドウの初期化
	bool InitWindow(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	Direct3Dの初期化
	bool InitDirect3d();

	//	プレゼンテーションパラメータの初期化
	bool InitPresentParam(RECT wndRect, bool fullscreen_f);

	//	Direct3Dデバイスの初期化
	bool InitD3dDevice();

	//	DirectInputの初期化
	bool InitDirectInput(HINSTANCE hInst);

	//	DirectInputデバイスの初期化
	bool InitDinputDevice();
};








