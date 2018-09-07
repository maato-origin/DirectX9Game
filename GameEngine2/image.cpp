#include "image.h"

//デフォルトコンストラクタ
Image::Image()
{
	initialized = false;						//初期化成功時にtrueを設定
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0;
	spriteData.y = 0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;					//複数のフレーム画像の一つを選ぶのに使用
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;					//スプライト用テクスチャ
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0f;							//アニメーションの各フレームを1秒に設定
	animTimer = 0.0;
	visible = true;								//イメージを可視化
	loop = true;								//フレーム繰り返し
	animComplete = false;
	graphics = NULL;							//graphic systemへと繋ぐ
	colorFilter = graphicsNS::WHITE;			//白から変更なし
}

//デストラクタ
Image::~Image()
{}

//Imageを初期化
//実行後：成功時はtrue,失敗時はfalseを戻す
bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM)
{
	try {
		graphics = g;						//graphicsオブジェクト
		textureManager = textureM;			//テクスチャオブジェクトへのポインタ

		spriteData.texture = textureManager->getTexture();
		if (width == 0)
			width = textureManager->getWidth();		//全体の幅を使用
		spriteData.width = width;

		if (height == 0)
			height = textureManager->getHeight();	//全体の高さを使用
		spriteData.height = height;
		cols = ncols;
		if (cols == 0)
			cols = 1;								//0cols は1とみなす

		//currentFrameを描画するspriteData.rectを構成
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		//右端+1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		//下端+1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { return false; }
	initialized = true;								//正常に初期化された場合

	return true;
}

//colorをフィルタとして使って画像を描画
//colorパラメータはオプション
//デフォルトで白が割り当てられている
//実行前：spriteBegin()を呼び出す
//実行後：spriteEnd()を呼び出す
void Image::draw(COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//onReset()が呼び出された時に新しいテクスチャを取得
	spriteData.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)		//フィルタを使って描画する場合
		//colorFilterを使用
		graphics->drawSprite(spriteData, colorFilter);
	else
		//フィルタとしてcolorを使用
		graphics->drawSprite(spriteData, color);
}

//指定されたSpriteDataを使ってこの画像を描画
//現在のSpriteData.rectを使ってテクスチャを選択
//実行前：spriteBegin()を呼び出す
//実行後：spriteEnd()を呼び出す
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//この画像の矩形を使ってテクスチャを選択
	sd.rect = spriteData.rect;
	//onReset()が呼び出された時に新しいテクスチャを取得
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)				//フィルタを使って描画する場合
		graphics->drawSprite(sd, colorFilter);		//colorFilterを使用
	else
		graphics->drawSprite(sd, color);			//フィルタとしてcolorを使用
}

//フレーム毎に1回呼び出す
//frameTimeは移動とアニメーションの速さを制御するために使用
void Image::update(float frameTime)
{
	if (endFrame - startFrame > 0)									//アニメーション化するスプライトの場合
	{
		animTimer += frameTime;										//合計の経過時間
		if (animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame<startFrame || currentFrame>endFrame)
			{
				if (loop == true)									//ループするアニメーションの場合
					currentFrame = startFrame;
				else												//ループしないアニメーションの場合
				{
					currentFrame = endFrame;
					animComplete = true;							//アニメーションの完了
				}
			}
			setRect();												//spriteData.rectの設定
		}
	}
}

//複数フレームを持つ画像内で現在のフレームを設定する
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = 1;
		animComplete = false;
		setRect();				//spriteData.rectを設定
	}
}

//currentFrameを描画するspriteData.rect構造体を設定する
inline void Image::setRect()
{
	//currentFrameを描画するspriteData.rectを構成
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}