/*
*	�A�v���P�[�V�����̏��������s���N���X
*
*	2015/05/17		Y.Ozawa
*/

#include "Lib.h"
#include <crtdbg.h>
#include <cassert>

LPDIRECT3DDEVICE9		d3dDevice = 0;
LPDIRECTINPUTDEVICE8	dinputDevice = 0;

DWORD FloatToDWORD(float val){ return *((DWORD *)&val); }

//	�R���X�g���N�^
Application::Application(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	DebugLog("�A�v���P�[�V�����𐶐����܂���\n");

	//	�A�v���P�[�V�����̏�����
	Initialize(wndTitle, wndRect, fullscreen_f, hInst, cmdShow);
}

Application::~Application()
{
	DebugLog("�A�v���P�[�V������j�����܂����B\n");

	// �A�v���P�[�V�����̏I������������
	Release();
}

//	�A�v���P�[�V�����̏�����
void Application::Initialize(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	//	�E�B���h�E�̏�����
	if (!InitWindow(wndTitle, wndRect, fullscreen_f, hInst, cmdShow))
		DebugAlert("�E�B���h�E��\n�������ł��܂���ł����B");

	//	Direct3D�̏�����
	if (!InitDirect3d())
		DebugAlert("Direct3D\n���������ł��܂���ł����B");

	//	�v���[���e�[�V�����p�����[�^�̏�����
	if (!InitPresentParam(wndRect, fullscreen_f))
		DebugAlert("�v���[���e�[�V�����p�����[�^��\n�������ł��܂���ł����B");

	if (!InitD3dDevice())
		DebugAlert("Direct3D�f�o�C�X��\n�������ł��܂���ł����B");

	DebugLog("�A�v���P�[�V���������������܂����B\n");
}

//	�A�v���P�[�V�����̉��
void Application::Release()
{
	//	�f�o�b�O�r���h���Ɋm�F�p�̃r�[�v����炷
#if defined(DEBUG) | defined(_DEBUG)
	MessageBeep(MB_OK);
#endif

	//	�R���\�[�������
	CloseConsole();

	SafeRelease(direct3d);
}

//	���[�v����
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

//	�E�B���h�E�n���h���̎擾
HWND Application::GetWndHandle()
{
	return hWnd;
}

//	�E�B���h�E�v���V�[�W��
LRESULT CALLBACK Application::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//	Window����̃��b�Z�[�W����������
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

//	�E�B���h�E�̏�����
bool Application::InitWindow(TCHAR *wndTitle, RECT wndRect, bool fullscreen_f, HINSTANCE hInst, int cmdShow)
{
	//	�E�B���h�E�N���X�̏�����
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

	//	�E�B���h�E�N���X�̓o�^
	if (!RegisterClassEx(&winClassEx))
		return false;

	//	�E�B���h�E���쐬
	if (fullscreen_f)
	{
		// �t���X�N���[�����[�h�ŕ\������ꍇ��
		// ��ʂ����ς��ɃE�B���h�E���L����
		hWnd = CreateWindow(winClassEx.lpszClassName, wndTitle, WS_OVERLAPPEDWINDOW,
			0, 0, wndRect.right, wndRect.bottom,
			nullptr, nullptr, hInst, nullptr);
	}
	else
	{
		// �E�B���h�E���[�h�ŕ\������ꍇ��
		// �f�t�H���g�̃E�B���h�E�ʒu�ɕ\������
		AdjustWindowRect(&wndRect, WS_OVERLAPPEDWINDOW, false);
		hWnd = CreateWindow(winClassEx.lpszClassName, wndTitle, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
			nullptr, nullptr, hInst, nullptr);
	}

	//	�E�B���h�E��\������
	if (FAILED(ShowWindow(hWnd, cmdShow)))
		return false;

	//	 WM_PAINT���󂯎��Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);
	return true;
}

//	Direct3D�̏�����
bool Application::InitDirect3d()
{
	//	Direct3D9�I�u�W�F�N�g�̍쐬
	if ((direct3d = Direct3DCreate9(D3D_SDK_VERSION)) == 0)
		return false;

	return true;
}

//	�v���[���e�[�V�����p�����[�^�̏�����
bool Application::InitPresentParam(RECT wndRect, bool fullscreen_f)
{
	D3DDISPLAYMODE *displayMode = new D3DDISPLAYMODE();

	//	���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, displayMode)))
		return false;

	//	�f�o�C�X�̃v���[���e�[�V�����p�����[�^��������
	ZeroMemory(&presentParam, sizeof(D3DPRESENT_PARAMETERS));
	presentParam.BackBufferCount = 1;
	if (fullscreen_f)
	{
		//	�t���X�N���[���̏ꍇ
		presentParam.Windowed = false;						//	�t���X�N���[���\���̐ݒ�
		presentParam.BackBufferWidth = wndRect.right;		//	�t���X�N���[�����̉���
		presentParam.BackBufferHeight = wndRect.bottom;		//	�t���X�N���[�����̏c��
	}
	else
	{
		presentParam.Windowed = true;						//	�E�B���h�E���\���̎w��
	}
	presentParam.BackBufferFormat = displayMode->Format;	//	�J���[���[�h�̎w��
	presentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParam.EnableAutoDepthStencil = true;				//	�G���[�΍�
	presentParam.AutoDepthStencilFormat = D3DFMT_D16;		//	�G���[�΍�

	delete displayMode;
	return true;
}

//	Direct3D�f�o�C�X�̏�����
bool Application::InitD3dDevice()
{
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(direct3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &presentParam, &d3dDevice)))
				return false;

	//�����_�����O�X�e�[�^�X�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);	//���ʕ`�惂�[�h�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_ZENABLE, true);				//Z��r���s��
	d3dDevice->SetRenderState(D3DRS_DITHERENABLE, true);		//�f�B�U�����O���s��

	// �|�C���g�X�v���C�g�̐ݒ�
	d3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// �|�C���g�X�P�[���̌W��
	d3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	d3dDevice->SetRenderState(D3DRS_POINTSIZE, FloatToDWORD(1.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_A, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_B, FloatToDWORD(0.0f));
	d3dDevice->SetRenderState(D3DRS_POINTSCALE_C, FloatToDWORD(1.0f));
	return true;
}

//	DirectInput�̏�����
bool Application::InitDirectInput(HINSTANCE hInst)
{
	//	DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr)))
		return false;

	return true;
}

//	DirectInput�f�o�C�X�̏�����
bool Application::InitDinputDevice()
{
	//	DirectInput�f�o�C�X���쐬
	if (FAILED(directInput->CreateDevice(GUID_SysMouse, &dinputDevice, nullptr)))
		return false;

	//	�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(dinputDevice->SetDataFormat(&c_dfDIMouse2)))
		return false;

	//	�������[�h��ݒ�
	if (FAILED(dinputDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		return false;

	//	�����[�h��ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;

	if (FAILED(dinputDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		MessageBox(NULL, "�ݒ�̎��s", "Direct Input Error", MB_OK);
		return false;
	}

	//	���͐���J�n
	dinputDevice->Acquire();

	return true;
}





