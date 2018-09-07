#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
	//Imageのプロパティ
protected:
	Graphics *graphics;					//Graphicsへのポインタ
	TextureManager *textureManager;		//textureManagerへのポインタ
	SpriteData spriteData;				//Graphics::drawSprite関数が必要とするデータを格納する構造体
	COLOR_ARGB colorFilter;			//colorパラメータにFILTERが指定された場合、スプライトの描画時にフィルタとして適用する色
	int cols;							//複数フレームを持つスプライト内の列数
	int startFrame;						//複数フレームを持つアニメーションシーケンスの最初のフレーム番号
	int endFrame;						//複数フレームを持つアニメーションシーケンスの最初のフレーム番号
	int currentFrame;					//アニメーションシーケンスの現在のフレーム番号
	float frameDelay;					//アニメーション間の間隔
	float animTimer;					//アニメーションの時間を計る
	HRESULT hr;							//標準の戻り型
	bool loop;							//trueでアニメーション繰り返し
	bool visible;						//trueなら画像可視
	bool initialized;					//初期化成功でtrue
	bool animComplete;					//loopがfalseかつendFrameが完了していればtrue

public:
	//コンストラクタ
	Image();

	//デストラクタ
	virtual ~Image();

	//取得関数

	//SpriteData構造体の参照を戻す
	const virtual SpriteData& getSpriteInfo() { return spriteData; }

	//visibleの値を戻す
	virtual bool getVisible() { return visible; }

	//Xの位置を戻す
	virtual float getX() { return spriteData.x; }

	//Yの位置を戻す
	virtual float getY() { return spriteData.y; }

	//scaleの値を戻す
	virtual float getScale() { return spriteData.scale; }

	//Widthを戻す
	virtual float getWidth() { return spriteData.width; }

	//Heightを戻す
	virtual float getHeight() { return spriteData.height; }

	//X座標の中心を戻す
	virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }

	//Y座標の中心を戻す
	virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }

	//回転角度を戻す
	virtual float getDegrees() { return spriteData.angle*(180.0f / (float)PI); }

	//回転角をラジアンで戻す
	virtual float getRadians() { return spriteData.angle; }

	//アニメーションフレーム間の間隔を戻す
	virtual float getFrameDelay() { return frameDelay; }

	//開始フレーム値を戻す
	virtual int getStartFrame() { return startFrame; }

	//終了フレーム値を戻す
	virtual int getEndFrame() { return endFrame; }

	//現在のフレーム値を戻す
	virtual int getCurrentFrame() { return currentFrame; }

	//ImageのRECT構造体を戻す
	virtual RECT getSpriteDataRect() { return spriteData.rect; }

	//アニメーション完了状態を戻す
	virtual bool getAnimationComplete() { return animComplete; }

	//colorFilterを戻す
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }

	//設定関数
	//X位置を設定
	virtual void setX(float newX) { spriteData.x = newX; }

	//Y位置を設定
	virtual void setY(float newY) { spriteData.y = newY; }

	//拡大縮小を設定
	virtual void setScale(float s) { spriteData.scale = s; }

	//回転角度を設定
	//0度は上向き。角度は時計回りに進む
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }

	//回転角度を設定（ラジアン）
	//0ラジアンは上向き。角度は時計回りに進む
	virtual void setRadians(float rad) { spriteData.angle = rad; }

	//可視性を設定
	virtual void setVisible(bool v) { visible = v; }

	//アニメーションのフレーム間の間隔を設定
	virtual void setFrameDelay(float d) { frameDelay = d; }

	//アニメーションの開始フレームと終了フレームを設定
	virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }

	//アニメーションの現在のフレームを取得
	virtual void setCurrentFrame(int c);

	//currentFrameを描画するspriteData.rectを設定
	virtual void setRect();

	//spriteData.rectをrに設定
	virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }

	//アニメーションループを設定(lp=trueの場合にループ)
	virtual void setLoop(bool lp) { loop = lp; }

	//アニメーション完了ブール値を設定
	virtual void setAnimationComplete(bool a) { animComplete = a; }

	//色フィルタを設定
	virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }

	//textureManagerを設定
	virtual void setTextureManager(TextureManager *textureM) { textureManager = textureM; }

	//その他関数
	
	//Imageを初期化
	//実行後：成功した場合はtrue,失敗した場合はfalseを戻す
	//*gはGraphicsへのポインタ
	//widthはImageの幅(0の時テクスチャ全体の幅を使用)
	//heightはImageの高さ(0の時テクスチャ全体の高さを使用)
	//ncolsはテクスチャ内の列数(1～nまで)(0は1と同じ)
	//*textureMはTextureManagerへのポインタ
	virtual bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);

	//画像を水平方向に反転（鏡）
	virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }

	//画像を垂直方向に反転
	virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }

	//colorをフィルタとして使って画像を描画。colorのデフォルトは白
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	//指定されたSpriteDataを使って画像を描画
	//現在のSpriteData.rectを使用してテクスチャを選択
	virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);

	//アニメーションの更新。frameTimeは基準値として使われる
	virtual void update(float frameTime);
};

#endif // !_IMAGE_H
