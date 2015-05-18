/*
*	�A�v���P�[�V�����̏��������s���N���X
*
*	2015/05/17		Y.Ozawa
*/

#pragma once

//	�A�v���P�[�V�������s���𐧌䂷��N���X
class Application
{
private:

	HWND hWnd;								//	�E�B���h�E�̃n���h��

	LPDIRECT3D9 direct3d;					//	Direct3D�I�u�W�F�N�g
	D3DPRESENT_PARAMETERS presentParam;		//	�v���[���e�[�V�����p�����[�^
	LPDIRECTINPUT8 directInput;				//	DirectInput�I�u�W�F�N�g

public:

	//	�R���X�g���N�^
	Application(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	�f�X�g���N�^
	~Application();

	//	�A�v���P�[�V�����̏�����
	void Initialize(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	�A�v���P�[�V�����̉��
	void Release();

	//	���[�v����
	bool Loop();

	//	�E�B���h�E�n���h���̎擾
	HWND GetWndHandle();

private:

	//	�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//	�E�B���h�E�̏�����
	bool InitWindow(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow);

	//	Direct3D�̏�����
	bool InitDirect3d();

	//	�v���[���e�[�V�����p�����[�^�̏�����
	bool InitPresentParam(RECT wndRect, bool fullscreen_f);

	//	Direct3D�f�o�C�X�̏�����
	bool InitD3dDevice();

	//	DirectInput�̏�����
	bool InitDirectInput(HINSTANCE hInst);

	//	DirectInput�f�o�C�X�̏�����
	bool InitDinputDevice();
};








