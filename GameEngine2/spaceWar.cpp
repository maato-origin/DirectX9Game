#include "spaceWar.h"

//コンストラクタ
SpaceWar::SpaceWar()
{}

//デストラクタ
SpaceWar::~SpaceWar()
{
	releaseAll();		//全てのグラフィックスアイテムについてonLostDevice()を呼び出す
}

//ゲームを初期化
//エラー時にGameErrorをスロー
void SpaceWar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	//GameErrorをスロー
	//星雲のテクスチャ
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));
	//惑星のテクスチャ
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
	//宇宙船のテクスチャ
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));
	//星雲
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	//惑星
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	//画面の中央に惑星を配置
	planet.setX(GAME_WIDTH*0.5f - planet.getWidth()*0.5f);
	planet.setY(GAME_HEIGHT*0.5f - planet.getHeight()*0.5f);

	//宇宙船
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
	ship.setX(GAME_WIDTH / 4);								//惑星の左上から出発
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);		//アニメーションのフレーム
	ship.setCurrentFrame(SHIP_START_FRAME);					//開始フレーム
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	ship.setDegrees(45.0f);									//宇宙船の角度

	return;
}

//全てのゲームアイテムを更新
void SpaceWar::update()
{
	if (input->isKeyDown(SHIP_RIGHT_KEY))						//右に移動する場合
	{
		ship.setX(ship.getX() + frameTime * SHIP_SPEED);
		if (ship.getX() > GAME_WIDTH)							//画面の右にはみ出す場合
			ship.setX((float)-ship.getWidth());					//画面の左に配置
	}

	if (input->isKeyDown(SHIP_LEFT_KEY))						//左に移動する場合
	{
		ship.setX(ship.getX() - frameTime * SHIP_SPEED);
		if (ship.getX() < -ship.getWidth())						//画面の左にはみ出す場合
			ship.setX((float)GAME_WIDTH);						//画面の右に配置
	}

	if (input->isKeyDown(SHIP_UP_KEY))							//上に移動する場合
	{
		ship.setY(ship.getY() - frameTime * SHIP_SPEED);
		if (ship.getY() < -ship.getHeight())						//画面の上にはみ出す場合
			ship.setY((float)GAME_HEIGHT);						//画面の下に配置
	}

	if (input->isKeyDown(SHIP_DOWN_KEY))						//下に移動する場合
	{
		ship.setY(ship.getY() + frameTime * SHIP_SPEED);
		if (ship.getY() > GAME_HEIGHT)							//画面の下にはみ出す場合
			ship.setY((float)-ship.getHeight());				//画面の上に配置
	}

	ship.update(frameTime);
}

//人工知能
void SpaceWar::ai()
{}

//衝突処理
void SpaceWar::collisions()
{}

//ゲームアイテムをレンダー
void SpaceWar::render()
{
	graphics->spriteBegin();	//スプライトの描画を開始
	
	nebula.draw();				//星雲をシーンに追加
	planet.draw();				//惑星をシーンに追加
	ship.draw();				//宇宙船をシーンに追加

	graphics->spriteEnd();		//スプライトの描画を終了
}

//グラフィックスデバイスが消失した時
//グラフィックスデバイスをリセット可能にするため
//予約されていたビデオメモリを全て解放
void SpaceWar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//グラフィックデバイスがリセットされた場合
//全てのサーフェイスを再作成
void SpaceWar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	shipTexture.onResetDevice();
	Game::resetAll();
	return;
}