// Programming 2D Games
// Copyright (c) 2011,2012 by:
// Charles Kelly
// Chapter 4 graphics.cpp v1.1

#include "graphics.h"

//=============================================================================
// Constructor
//=============================================================================
Graphics::Graphics()
{
    direct3d = NULL;
    device3d = NULL;
	sprite = NULL;
    fullscreen = false;
    width = GAME_WIDTH;    // width & height are replaced in initialize()
    height = GAME_HEIGHT;
    backColor = graphicsNS::BACK_COLOR; // dark blue
}

//=============================================================================
// Destructor
//=============================================================================
Graphics::~Graphics()
{
    releaseAll();
}

//=============================================================================
// Release all
//=============================================================================
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite);
    SAFE_RELEASE(device3d);
    SAFE_RELEASE(direct3d);
}

//=============================================================================
// Initialize DirectX graphics
// throws GameError on error
//=============================================================================
void Graphics::initialize(HWND hw, int w, int h, bool full)
{
    hwnd = hw;
    width = w;
    height = h;
    fullscreen = full;

    //initialize Direct3D
    direct3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (direct3d == NULL)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

    initD3Dpp();        // init D3D presentation parameters
    if(fullscreen)      // if full-screen mode
    {
        if(isAdapterCompatible())   // is the adapter compatible
            // set the refresh rate with a compatible one
            d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
        else
            throw(GameError(gameErrorNS::FATAL_ERROR, 
            "The graphics device does not support the specified resolution and/or format."));
    }

    // determine if graphics card supports harware texturing and lighting and vertex shaders
    D3DCAPS9 caps;
    DWORD behavior;
    result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
    // If device doesn't support HW T&L or doesn't support 1.1 vertex 
    // shaders in hardware, then switch to software vertex processing.
    if( (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
            caps.VertexShaderVersion < D3DVS_VERSION(1,1) )
        behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;  // use software only processing
    else
        behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;  // use hardware only processing

    //create Direct3D device
    result = direct3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hwnd,
        behavior,
        &d3dpp, 
        &device3d);

    if (FAILED(result))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D device"));

	result = D3DXCreateSprite(device3d, &sprite);

	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));
 
}

//=============================================================================
// Initialize D3D presentation parameters
//=============================================================================
void Graphics::initD3Dpp()
{
    try{
        ZeroMemory(&d3dpp, sizeof(d3dpp));              // fill the structure with 0
        // fill in the parameters we need
        d3dpp.BackBufferWidth   = width;
        d3dpp.BackBufferHeight  = height;
        if(fullscreen)                                  // if fullscreen
            d3dpp.BackBufferFormat  = D3DFMT_X8R8G8B8;  // 24 bit color
        else
            d3dpp.BackBufferFormat  = D3DFMT_UNKNOWN;   // use desktop setting
        d3dpp.BackBufferCount   = 1;
        d3dpp.SwapEffect        = D3DSWAPEFFECT_DISCARD;
        d3dpp.hDeviceWindow     = hwnd;
        d3dpp.Windowed          = (!fullscreen);
        d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
    } catch(...)
    {
        throw(GameError(gameErrorNS::FATAL_ERROR, 
                "Error initializing D3D presentation parameters"));

    }
}

//テクスチャをデフォルトのD3Dメモリに読み込む
//エンジンの使用時に内部的にのみ使用
//ゲームテクスチャの読み込みにはTextureManagerクラスを使用する
//実行前：filenameはテクスチャファイルの名前
//		　transcolorは透明として扱う色
//実行後：width,height=テクスチャの寸法
//		　textureはテクスチャを指す
//HRESULTを戻す
HRESULT Graphics::loadTexture(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	//ファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//幅と高さをファイルから取得
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		//ファイルを読み込んで新しいテクスチャを作成
		result = D3DXCreateTextureFromFileEx(
			device3d,			//3Dデバイス
			filename,			//画像ファイル名
			info.Width,			//テクスチャの幅
			info.Height,		//テクスチャの高さ
			1,					//ミップマップレベル(チェーンなしの場合1)
			0,					//使用方法(Usage)
			D3DFMT_UNKNOWN,		//サーフェイスフォーマット(デフォルト)
			D3DPOOL_DEFAULT,	//テクスチャ用のメモリの種類
			D3DX_DEFAULT,		//画像フィルタ
			D3DX_DEFAULT,		//ミップフィルタ
			transcolor,			//透明用の色キー
			&info,				//ビットマップファイル情報
			NULL,				//カラーパレット
			&texture);			//目的のテクスチャ

	}
	catch(...){
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}

	return result;
}

//テクスチャをシステムメモリへ読み込む(システムメモリはロック可能)
//ピクセルデータへの直接アクセスを可能にする
//実行前：filenameはテクスチャファイルの名前
//transcolorは透明として扱う色
//実行後：widthとheight=テクスチャの寸法
//textureはテクスチャを指す
//HRESULTを戻し、TextureData構造体にデータを格納する
HRESULT Graphics::loadTextureSystemMem(const char *filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture)
{
	//ビットマップファイル情報を読み取るための構造体
	D3DXIMAGE_INFO info;
	result = E_FAIL;		//標準のWindows戻り値

	try {
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		//幅と高さをビットマップファイルから取得
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		width = info.Width;
		height = info.Height;

		//ビットマップ画像ファイルを読み込んで、新しいテクスチャを作成
		result = D3DXCreateTextureFromFileEx(
			device3d,			//3Dデバイス
			filename,			//ビットマップファイルの名前
			info.Width,			//ビットマップ画像の幅
			info.Height,		//ビットマップ画像の高さ
			1,					//ミップマップのレベル
			0,					//使用方法
			D3DFMT_UNKNOWN,		//サーフェイスフォーマット
			D3DPOOL_SYSTEMMEM,	//システムメモリはロック可能
			D3DX_DEFAULT,		//画像フィルタ
			D3DX_DEFAULT,		//ミップフィルタ
			transcolor,			//透明用の色キー
			&info,				//ビットマップファイル情報
			NULL,				//カラーパレット
			&texture);			//目的のテクスチャ
	}
	catch(...){
		throw(GameError(gameErrorNS::FATAL_ERROR, "Erro in Graphics::loadTexture"));
	}

	return result;
}

//=============================================================================
// Create a vertex buffer.
// Pre: verts[] contains vertex data.
//      size = size of verts[]
// Post: &vertexBuffer points to buffer if successful.
//=============================================================================
HRESULT Graphics::createVertexBuffer(VertexC verts[], UINT size, LP_VERTEXBUFFER &vertexBuffer)
{
	// Standard Windows return value
	HRESULT result = E_FAIL;

	// Create a vertex buffer
	result = device3d->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX,
		D3DPOOL_DEFAULT, &vertexBuffer, NULL);
	if (FAILED(result))
		return result;

	void *ptr;
	// must lock buffer before data can be transferred in
	result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
	if (FAILED(result))
		return result;
	memcpy(ptr, verts, size);   // copy vertex data into buffer
	vertexBuffer->Unlock();     // unlock buffer

	return result;
}

//=============================================================================
// Display a quad with alpha transparency using Triangle Fan
// Pre: createVertexBuffer was used to create vertexBuffer containing four
//      vertices defining the quad in clockwise order.
//      g3ddev->BeginScene was called
// Post: Quad is drawn
//=============================================================================
bool Graphics::drawQuad(LP_VERTEXBUFFER vertexBuffer)
{
	HRESULT result = E_FAIL;    // standard Windows return value

	if (vertexBuffer == NULL)
		return false;

	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, true); // enable alpha blend

	device3d->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
	device3d->SetFVF(D3DFVF_VERTEX);
	result = device3d->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	device3d->SetRenderState(D3DRS_ALPHABLENDENABLE, false); // alpha blend off

	if (FAILED(result))
		return false;

	return true;
}

//=============================================================================
// Display the backbuffer
//=============================================================================
HRESULT Graphics::showBackbuffer()
{
    result = E_FAIL;    // default to fail, replace on success
    // Display backbuffer to screen
    result = device3d->Present(NULL, NULL, NULL, NULL);
    return result;
}

//SpriteData構造体に記述されたスプライトを描画
//color(オプション)はフィルタのように適用される
//デフォルトは白
//実行前:sprite->Begin()を呼び出す
//実行後:sprite->End()を呼び出す
//		 spriteData.rectは描画するspriteData.textureの部分を定義
//		 spriteData.rect.rightは右端+1にする
//		 spriteData.rect.bottomは下端+1にする
void Graphics::drawSprite(const SpriteData &spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)	//テクスチャがない場合
		return;

	//スプライトの中心を特定
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale),
										  (float)(spriteData.height / 2 * spriteData.scale));

	//スプライトの画面位置
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

	//X方向とY方向の拡大縮小
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	if (spriteData.flipHorizontal)	//水平方向に反転する場合
	{
		scaling.x *= -1;			//Xスケールを負にして反転
		//反転された画像の中心を取得
		spriteCenter.x -= (float)(spriteData.width*spriteData.scale);

		//左端を中心に反転が起きるので反転された画像を
		//元と同じ位置に配置するために右へ移動する
		translate.x += (float)(spriteData.width*spriteData.scale);
	}

	if (spriteData.flipVertical)	//垂直方向に反転する場合
	{
		scaling.y *= -1;			//Yスケールを負にして反転
									//反転された画像の中心を取得
		spriteCenter.y -= (float)(spriteData.height*spriteData.scale);

		//上端を中心に反転が起きるので反転された画像を
		//元と同じ位置に配置するために下へ移動する
		translate.y += (float)(spriteData.height*spriteData.scale);
	}

	//スプライトの回転、拡大縮小、配置を行うための行列を作成
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,					//行列
		NULL,						//拡大縮小時、原点は左上
		0.0f,						//拡大縮小時の回転なし
		&scaling,					//拡大縮小の倍率
		&spriteCenter,				//回転の中心
		(float)(spriteData.angle),	//回転の角度
		&translate);				//X,Yの位置

	//行列をスプライトに適用
	sprite->SetTransform(&matrix);

	//スプライトを描画
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//=============================================================================
// Checks the adapter to see if it is compatible with the BackBuffer height,
// width and refresh rate specified in d3dpp. Fills in the pMode structure with
// the format of the compatible mode, if found.
// Pre: d3dpp is initialized.
// Post: Returns true if compatible mode found and pMode structure is filled.
//       Returns false if no compatible mode found.
//=============================================================================
bool Graphics::isAdapterCompatible()
{
    UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, 
                                            d3dpp.BackBufferFormat);
    for(UINT i=0; i<modes; i++)
    {
        result = direct3d->EnumAdapterModes( D3DADAPTER_DEFAULT, 
                                        d3dpp.BackBufferFormat,
                                        i, &pMode);
        if( pMode.Height == d3dpp.BackBufferHeight &&
            pMode.Width  == d3dpp.BackBufferWidth &&
            pMode.RefreshRate >= d3dpp.FullScreen_RefreshRateInHz)
            return true;
    }
    return false;
}

//=============================================================================
// Test for lost device
//=============================================================================
HRESULT Graphics::getDeviceState()
{ 
    result = E_FAIL;    // default to fail, replace on success
    if (device3d == NULL)
        return  result;
    result = device3d->TestCooperativeLevel(); 
    return result;
}

//=============================================================================
// Reset the graphics device
//=============================================================================
HRESULT Graphics::reset()
{
    result = E_FAIL;    // default to fail, replace on success
    initD3Dpp();                        // init D3D presentation parameters
    result = device3d->Reset(&d3dpp);   // attempt to reset graphics device
    return result;
}

//=============================================================================
// Toggle window or fullscreen mode
// Pre: All user created D3DPOOL_DEFAULT surfaces are freed.
// Post: All user surfaces are recreated.
//=============================================================================
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try {
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)      // if already in fullscreen mode
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false) // if already in window mode
				return;
			fullscreen = false; break;
		default:        // default to toggle window/fullscreen
			fullscreen = !fullscreen;
		}
		reset();
		if (fullscreen)  // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else            // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
				SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);   // get size of client area of window
			MoveWindow(hwnd,
				0,                                           // Left
				0,                                           // Top
				GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
				GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
				TRUE);                                       // Repaint the window
		}

	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT,
			SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);   // get size of client area of window
		MoveWindow(hwnd,
			0,                                           // Left
			0,                                           // Top
			GAME_WIDTH + (GAME_WIDTH - clientRect.right),    // Right
			GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), // Bottom
			TRUE);                                       // Repaint the window
	}
}
