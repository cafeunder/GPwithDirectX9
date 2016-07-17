#include "SystemUtilities.h"
#include "SystemSetting.h"
#include <d3d9.h>

//�C���X�^���X�n���h��
HINSTANCE hInst;
//D3D9�@�\�̃C���X�^���X���w���|�C���^
PDIRECT3D9 g_pD3D = NULL;
//�r�f�I�J�[�h�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

//D3D�@�\�̏��������s���֐�
HRESULT InitD3D( HWND hWnd ){
	//===D3D9�̃C���X�^���X�𐶐�===//
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	HALT( NULL == g_pD3D );


	//===D3D�@�\�̃p�����[�^�ݒ�===//
	//D3D�@�\�̃p�����[�^���������\���̂𐶐�����
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	//�E�B���h�E���[�h�̎w��iTRUE=�E�B���h�E�EFALSE=�t���X�N���[���j
	d3dpp.Windowed = TRUE;
	//�_�u���o�b�t�@���̐؂�ւ����[�h���w��i�����l�j
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//�o�b�N�o�b�t�@�̃t�H�[�}�b�g���w��i�����l�j
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;


	//===D3D9�̏�����===//
	HRESULT result = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pd3dDevice
	);
	HALT( FAILED( result ) );

	return S_OK;
}

//D3D�C���X�^���X�̉�����s���֐�
void Cleanup(){
	if( NULL != g_pd3dDevice ){
		g_pd3dDevice->Release();
	}

	if( NULL != g_pD3D ){
		g_pD3D->Release();
	}
}

//�`����s���֐�
void Render(){
	if( NULL == g_pd3dDevice ) {
		return;
	}

	//���ۂ̕`�揈��
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 64, 128 ), 1.0f, 0 );
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) ){
		g_pd3dDevice->EndScene();
	}

	//�t�����g�o�b�t�@�֏�������
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

//���b�Z�[�W����������R�[���o�b�N�֐�
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
	switch( msg ){
	case WM_DESTROY:
		//��ʂ̕���{�^���⋭���I����
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	case WM_PAINT:
		//�`�悪�s�����ԁi�ŏ����Ƃ�����ĂȂ��j
		Render();
		ValidateRect( hWnd, NULL );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//�v���Z�X���C��
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	UNREFERENCED_PARAMETER( hInst );
	//�E�B���h�EID
	const char* windowID = "Game Program";


	//===Windows�E�B���h�E�̃p�����[�^�ݒ�===//
	WNDCLASSEX wc = {
		sizeof( WNDCLASSEX ), //���̍\���̂̃T�C�Y�i�����l�j
		CS_CLASSDC, //�i�����l�j
		MsgProc, //���b�Z�[�W����������R�[���o�b�N�֐��i��Œ�`�������́j
		0L, //�i�����l�j
		0L,	//�i�����l�j
        GetModuleHandle( NULL ), //�i�����l�j
		NULL, //�^�X�N�o�[�A�C�R��
		NULL, //�}�E�X�J�[�\��
		NULL, //�i�����l�j
		NULL, //�E�B���h�E�㕔�̃��j���[
        windowID, //�E�B���h�EID
		NULL //�E�B���h�E����̃A�C�R��
	};
	RegisterClassEx( &wc );


	//===Windows�E�B���h�E�̐���===//
	//�E�B���h�E�̃X�^�C�����w��i�T�C�Y�ύX�֎~�j
	DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	//�f�X�N�g�b�v�̏����擾
	RECT rcRect;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect( hDesktop, &rcRect );

	//�E�B���h�E�̈ʒu�ƃT�C�Y���w��
	RECT windowSize = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AdjustWindowRect( &windowSize, windowStyle, FALSE );

	//�E�B���h�E����
	HWND hWnd = CreateWindow(
		windowID, //�E�B���h�EID
		"�^�C�g��", //�E�B���h�E�^�C�g��
		windowStyle, 
			//�E�B���h�E�X�^�C���i�T�C�Y�ύX�֎~�j
		( rcRect.right - WINDOW_WIDTH ) / 2,
			//�f�X�N�g�b�v�ɂ�����x�ʒu�i�Z���^�����O�j
		( rcRect.bottom - WINDOW_HEIGHT ) / 2,
			//�f�X�N�g�b�v�ɂ�����y�ʒu�i�Z���^�����O�j
		windowSize.right-windowSize.left,
			//�E�B���h�E�̉����i�N���C�A���g�̈�̊O�����܂ށj
		windowSize.bottom-windowSize.top,
			//�E�B���h�E�̏c���i�N���C�A���g�̈�̊O�����܂ށj
		NULL, //�i�����l�j
		NULL, //�i�����l�j
		wc.hInstance, //�i�����l�j
		NULL //�i�����l�j
	);


	//===D3D�@�\�̏�����===//
	InitD3D( hWnd );


	//===�E�B���h�E�̉���===//
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	//===���b�Z�[�W���O���[�v===//
	MSG msg;
	while(GetMessage( &msg, NULL, 0, 0 )){
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//�I��
	UnregisterClass( windowID, wc.hInstance );
	return 0;
}