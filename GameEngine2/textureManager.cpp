#include "textureManager.h"

//デフォルトコンストラクタ
TextureManager::TextureManager()
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;		//初期化成功時はtrue
}

//デストラクタ
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}

//テクスチャファイルをディスクから読み込む
//実行後：成功した場合はtrue,失敗した場合はfalseを戻す
bool TextureManager::initialize(Graphics *g, const char *f)
{
	try {
		graphics = g;	//graphicsオブジェクト
		file = f;		//テクスチャファイル

		hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
		if (FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...) { return false; }

	initialized = true;		//正常に初期化された場合、true

	return true;
}

//グラフィックスデバイスが消失した場合に呼び出される
void TextureManager::onLostDevice()
{
	if (!initialized)
		return;
	SAFE_RELEASE(texture);
}

//グラフィックスデバイスがリセットされた時に呼び出される
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}