#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
	//TextureManagerのプロパティ
private:
	UINT width;				//テクスチャの幅
	UINT height;			//テクスチャの高さ
	LP_TEXTURE texture;		//テクスチャへのポインタ
	const char *file;		//ファイル名
	Graphics *graphics;		//Graphicsへのポインタ
	bool initialized;		//初期化が成功した時trueを戻す
	HRESULT hr;				//基本的な返り値

public:
	//コンストラクタ
	TextureManager();

	//デストラクタ
	virtual ~TextureManager();

	//テクスチャへのポインタを戻す
	LP_TEXTURE getTexture() const { return texture; }

	//テクスチャの幅を戻す
	UINT getWidth() const { return width; }

	//テクスチャの高さを戻す
	UINT getHeight() const { return height; }

	//テクスチャファイルをディスクから読み込む
	//実行後：成功した場合はtrue,失敗した場合はfalseを戻す
	virtual bool initialize(Graphics *g, const char *file);

	//グラフィックスデバイスが消失時に呼び出される
	virtual void onLostDevice();

	//グラフィックスデバイスがリセットされたときに呼び出される
	virtual void onResetDevice();
};

#endif // !_TEXTUREMANAGER_H
