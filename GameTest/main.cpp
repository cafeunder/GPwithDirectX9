#include "SystemUtilities.h"
#include "SystemSetting.h"
#include <d3d9.h>

//インスタンスハンドル
HINSTANCE hInst;
//D3D9機能のインスタンスを指すポインタ
PDIRECT3D9 g_pD3D = NULL;
//ビデオカードへのポインタ
LPDIRECT3DDEVICE9 g_pd3dDevice = NULL;

//D3D機能の初期化を行う関数
HRESULT InitD3D( HWND hWnd ){
	//===D3D9のインスタンスを生成===//
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	HALT( NULL == g_pD3D );


	//===D3D機能のパラメータ設定===//
	//D3D機能のパラメータが入った構造体を生成する
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );

	//ウィンドウモードの指定（TRUE=ウィンドウ・FALSE=フルスクリーン）
	d3dpp.Windowed = TRUE;
	//ダブルバッファ時の切り替えモードを指定（推奨値）
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//バックバッファのフォーマットを指定（推奨値）
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;


	//===D3D9の初期化===//
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

//D3Dインスタンスの解放を行う関数
void Cleanup(){
	if( NULL != g_pd3dDevice ){
		g_pd3dDevice->Release();
	}

	if( NULL != g_pD3D ){
		g_pD3D->Release();
	}
}

//描画を行う関数
void Render(){
	if( NULL == g_pd3dDevice ) {
		return;
	}

	//実際の描画処理
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 64, 128 ), 1.0f, 0 );
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) ){
		g_pd3dDevice->EndScene();
	}

	//フロントバッファへ書き込み
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

//メッセージを処理するコールバック関数
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
	switch( msg ){
	case WM_DESTROY:
		//画面の閉じるボタンや強制終了時
		Cleanup();
		PostQuitMessage( 0 );
		return 0;
	case WM_PAINT:
		//描画が行える状態（最小化とかされてない）
		Render();
		ValidateRect( hWnd, NULL );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//プロセスメイン
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	UNREFERENCED_PARAMETER( hInst );
	//ウィンドウID
	const char* windowID = "Game Program";


	//===Windowsウィンドウのパラメータ設定===//
	WNDCLASSEX wc = {
		sizeof( WNDCLASSEX ), //この構造体のサイズ（推奨値）
		CS_CLASSDC, //（推奨値）
		MsgProc, //メッセージを処理するコールバック関数（上で定義したもの）
		0L, //（推奨値）
		0L,	//（推奨値）
        GetModuleHandle( NULL ), //（推奨値）
		NULL, //タスクバーアイコン
		NULL, //マウスカーソル
		NULL, //（推奨値）
		NULL, //ウィンドウ上部のメニュー
        windowID, //ウィンドウID
		NULL //ウィンドウ左上のアイコン
	};
	RegisterClassEx( &wc );


	//===Windowsウィンドウの生成===//
	//ウィンドウのスタイルを指定（サイズ変更禁止）
	DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	//デスクトップの情報を取得
	RECT rcRect;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect( hDesktop, &rcRect );

	//ウィンドウの位置とサイズを指定
	RECT windowSize = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	AdjustWindowRect( &windowSize, windowStyle, FALSE );

	//ウィンドウ生成
	HWND hWnd = CreateWindow(
		windowID, //ウィンドウID
		"タイトル", //ウィンドウタイトル
		windowStyle, 
			//ウィンドウスタイル（サイズ変更禁止）
		( rcRect.right - WINDOW_WIDTH ) / 2,
			//デスクトップにおけるx位置（センタリング）
		( rcRect.bottom - WINDOW_HEIGHT ) / 2,
			//デスクトップにおけるy位置（センタリング）
		windowSize.right-windowSize.left,
			//ウィンドウの横幅（クライアント領域の外側も含む）
		windowSize.bottom-windowSize.top,
			//ウィンドウの縦幅（クライアント領域の外側も含む）
		NULL, //（推奨値）
		NULL, //（推奨値）
		wc.hInstance, //（推奨値）
		NULL //（推奨値）
	);


	//===D3D機能の初期化===//
	InitD3D( hWnd );


	//===ウィンドウの可視化===//
	ShowWindow( hWnd, SW_SHOWDEFAULT );
	UpdateWindow( hWnd );

	//===メッセージングループ===//
	MSG msg;
	while(GetMessage( &msg, NULL, 0, 0 )){
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	//終了
	UnregisterClass( windowID, wc.hInstance );
	return 0;
}