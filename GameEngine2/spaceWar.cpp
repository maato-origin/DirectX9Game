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
	//メインのゲームテクスチャ
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
	//星雲
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	//惑星
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	//宇宙船1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);		//アニメーションのフレーム
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);							//開始フレーム
	ship1.setX(GAME_WIDTH / 4);													//惑星の左上から出発
	ship1.setY(GAME_HEIGHT / 4);
	ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED));					//VECTOR2(X, Y)

	//宇宙船2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);		//アニメーションのフレーム
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);							//開始フレーム
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);									//惑星の左上から出発
	ship2.setY(GAME_HEIGHT / 4);
	ship2.setVelocity(VECTOR2(-shipNS::SPEED, -shipNS::SPEED));					//VECTOR2(X, Y)

	return;
}

//全てのゲームアイテムを更新
void SpaceWar::update()
{
	/*
	if (input->isKeyDown(SHIP_RIGHT_KEY))						//右に移動する場合
	{
		ship1.setX(ship1.getX() + frameTime * SHIP_SPEED);
		if (ship1.getX() > GAME_WIDTH)							//画面の右にはみ出す場合
			ship1.setX((float)-ship1.getWidth());					//画面の左に配置
	}

	if (input->isKeyDown(SHIP_LEFT_KEY))						//左に移動する場合
	{
		ship1.setX(ship1.getX() - frameTime * SHIP_SPEED);
		if (ship1.getX() < -ship1.getWidth())						//画面の左にはみ出す場合
			ship1.setX((float)GAME_WIDTH);						//画面の右に配置
	}

	if (input->isKeyDown(SHIP_UP_KEY))							//上に移動する場合
	{
		ship1.setY(ship1.getY() - frameTime * SHIP_SPEED);
		if (ship1.getY() < -ship1.getHeight())						//画面の上にはみ出す場合
			ship1.setY((float)GAME_HEIGHT);						//画面の下に配置
	}

	if (input->isKeyDown(SHIP_DOWN_KEY))						//下に移動する場合
	{
		ship1.setY(ship1.getY() + frameTime * SHIP_SPEED);
		if (ship1.getY() > GAME_HEIGHT)							//画面の下にはみ出す場合
			ship1.setY((float)-ship1.getHeight());				//画面の上に配置
	}
	*/

	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
}

//人工知能
void SpaceWar::ai()
{}

//衝突処理
void SpaceWar::collisions()
{
	VECTOR2 collisionVector;
	VECTOR2 minuscollisionVector=collisionVector*-1;
	//宇宙船と惑星の衝突の場合
	if (ship1.collidesWith(planet, collisionVector))
	{
		//惑星から跳ね返る
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		//惑星から跳ね返る
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}

	//宇宙船同士の衝突の場合
	if (ship1.collidesWith(ship2, collisionVector))
	{
		//宇宙船から跳ね返る
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		//ship2のcollisionVectorの方向を変更する
		ship2.bounce(minuscollisionVector, ship1);
		ship2.damage(SHIP);
	}
}

//ゲームアイテムをレンダー
void SpaceWar::render()
{
	graphics->spriteBegin();	//スプライトの描画を開始
	
	nebula.draw();				//星雲をシーンに追加
	planet.draw();				//惑星をシーンに追加
	ship1.draw();				//宇宙船1をシーンに追加
	ship2.draw();				//宇宙船2をシーンに追加

	graphics->spriteEnd();		//スプライトの描画を終了
}

//グラフィックスデバイスが消失した時
//グラフィックスデバイスをリセット可能にするため
//予約されていたビデオメモリを全て解放
void SpaceWar::releaseAll()
{
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	Game::releaseAll();
	return;
}

//グラフィックデバイスがリセットされた場合
//全てのサーフェイスを再作成
void SpaceWar::resetAll()
{
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	Game::resetAll();
	return;
}