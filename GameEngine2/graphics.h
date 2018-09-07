// Programming 2D Games
// Copyright (c) 2011 by:
// Charles Kelly
// Chapter 4 graphics.h v1.0

#ifndef _GRAPHICS_H             // prevent multiple definitions if this 
#define _GRAPHICS_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif
#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// DirectX pointer types
#define LP_TEXTURE LPDIRECT3DTEXTURE9
#define LP_SPRITE LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9

// Color defines
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

namespace graphicsNS
{
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

//SpriteData：スプライトを描画するGraphics::drawSpriteが必要とするプロパティ
struct SpriteData
{
	int width;			//スプライトの幅
	int height;			//スプライトの高さ
	float x;			//画面位置(スプライトの左上隅)
	float y;
	float scale;		//<1で縮小、>1で拡大
	float angle;		//回転角度(ラジアン)
	RECT rect;			//大きなテクスチャから1つの画像を選択するときに使用
	LP_TEXTURE texture;	//テクスチャへのポインタ
	bool flipHorizontal;	//スプライトを水平方向に反転する場合true
	bool flipVertical;	//スプライトを垂直方向に反転する場合true
};

class Graphics
{
private:
    // DirectX pointers and stuff
    LP_3D       direct3d;
    LP_3DDEVICE device3d;
	LP_SPRITE sprite;
    D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE pMode;

    // other variables
    HRESULT     result;         // standard Windows return codes
    HWND        hwnd;
    bool        fullscreen;
    int         width;
    int         height;
    COLOR_ARGB  backColor;      // background color

    // (For internal engine use only. No user serviceable parts inside.)
    // Initialize D3D presentation parameters
    void initD3Dpp();

public:
    // Constructor
    Graphics();

    // Destructor
    virtual ~Graphics();

    // Releases direct3d and device3d.
    void    releaseAll();

	//テクスチャをデフォルトのD3Dメモリに読み込む
	//エンジンの使用時に内部的にのみ使用
	//ゲームテクスチャの読み込みにはTextureManagerクラスを使用する
	//実行前：filenameはテクスチャファイルの名前
	//		　transcolorは透明として扱う色
	//実行後：width,height=テクスチャの寸法
	//		　textureはテクスチャを指す
	//HRESULTを戻す
	HRESULT loadTexture(const char*filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE &texture);

    // Initialize DirectX graphics
    // Throws GameError on error
    // Pre: hw = handle to window
    //      width = width in pixels
    //      height = height in pixels
    //      fullscreen = true for full screen, false for window
    void    initialize(HWND hw, int width, int height, bool fullscreen);

    // Display the offscreen backbuffer to the screen.
    HRESULT showBackbuffer();

    // Checks the adapter to see if it is compatible with the BackBuffer height,
    // width and refresh rate specified in d3dpp. Fills in the pMode structure with
    // the format of the compatible mode, if found.
    // Pre: d3dpp is initialized.
    // Post: Returns true if compatible mode found and pMode structure is filled.
    //       Returns false if no compatible mode found.
    bool    isAdapterCompatible();

	//SpriteData構造体に記述されたスプライトを描画
	//color(オプション)はフィルタのように適用される
	//デフォルトは白
	//実行前:sprite->Begin()を呼び出す
	//実行後:sprite->End()を呼び出す
	//		 spriteData.rectは描画するspriteData.textureの部分を定義
	//		 spriteData.rect.rightは右端+1にする
	//		 spriteData.rect.bottomは下端+1にする
	void drawSprite(const SpriteData &spriteData, COLOR_ARGB color = graphicsNS::WHITE);

    // Reset the graphics device.
    HRESULT reset();

    // get functions
    // Return direct3d.
    LP_3D   get3D()             { return direct3d; }

    // Return device3d.
    LP_3DDEVICE get3Ddevice()   { return device3d; }

    // Return handle to device context (window).
    HDC     getDC()             { return GetDC(hwnd); }

    // Test for lost device
    HRESULT getDeviceState();

    //=============================================================================
    // Inline functions for speed. How much more speed? It depends on the game and
    // computer. Improvements of 3 or 4 percent have been observed.
    //=============================================================================

    // Set color used to clear screen
    void setBackColor(COLOR_ARGB c) {backColor = c;}

    //=============================================================================
    // Clear backbuffer and BeginScene()
    //=============================================================================
    HRESULT beginScene() 
    {
        result = E_FAIL;
        if(device3d == NULL)
            return result;
        // clear backbuffer to backColor
        device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0F, 0);
        result = device3d->BeginScene();          // begin scene for drawing
        return result;
    }

    //=============================================================================
    // EndScene()
    //=============================================================================
    HRESULT endScene() 
    {
        result = E_FAIL;
        if(device3d)
            result = device3d->EndScene();
        return result;
    }

	//描画の開始
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	//描画の終了
	void spriteEnd()
	{
		sprite->End();
	}
};

#endif

