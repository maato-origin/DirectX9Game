#include "spaceWar.h"

//コンストラクタ
SpaceWar::SpaceWar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
}

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

	//DirectXフォントを初期化
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);

	//メニューのテクスチャ
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	//星空のテクスチャ
	if (!spaceTexture.initialize(graphics, SPACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	//ゲームのテクスチャ
	if (!textures1.initialize(graphics, TEXTURES1_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game texture"));

	//メニューの画像
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	//星空の画像
	if (!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
	space.setScale((float)SPACE_SCALE);

	//月の画像群
	for (int i = 0; i < 4; i++)
	{
		if (!moons[i].initialize(graphics, MOON_SIZE, MOON_SIZE, 4, &textures1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moons"));
		moons[i].setFrames(MOON1_FRAME + i, MOON1_FRAME + i);
		moons[i].setCurrentFrame(MOON1_FRAME + i);
		//月は中央で開始
		moons[i].setX(GAME_WIDTH / 2 - MOON_SIZE / 2);
		moons[i].setY(GAME_HEIGHT / 2 - MOON_SIZE / 2);
	}

	//惑星
	if (!planet.initialize(graphics, PLANET_SIZE, PLANET_SIZE, 2, &textures1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	planet.setFrames(PLANET_FRAME, PLANET_FRAME);
	planet.setCurrentFrame(PLANET_FRAME);
	//惑星は中央で開始
	planet.setX(GAME_WIDTH / 2 - PLANET_SIZE / 2);
	planet.setY(GAME_HEIGHT / 2 - PLANET_SIZE / 2);

	//宇宙船1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &textures1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);		//アニメーションのフレーム
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);							//開始フレーム
	ship1.setColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	ship1.setMass(shipNS::MASS);
	ship1.setX(GAME_WIDTH / 2 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
}

//新しいラウンドを開始する
void SpaceWar::roundStart()
{
	space.setX(0);
	space.setY(0);

	//宇宙船を中央で開始させる
	ship1.setX(GAME_WIDTH / 2 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);

	ship1.setDegrees(0);
	countDownTimer = spacewarNS::COUNT_DOWN;
	countDownOn = true;
	roundOver = false;
}

//全てのゲームアイテムを更新
void SpaceWar::update()
{
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			input->clearAll();
			roundStart();
		}
	}
	else if (countDownOn)
	{
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			countDownOn = false;
	}
	else
	{
		if (ship1.getActive())
		{
			if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))	//エンジンがオンの時
			{
				ship1.setEngineOn(true);
				audio->playCue(ENGINE1);
			}
			else
			{
				ship1.setEngineOn(false);
				audio->stopCue(ENGINE1);
			}
			ship1.rotate(shipNS::NONE);
			if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))	//ship1が左に回転した場合
				ship1.rotate(shipNS::LEFT);
			if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadLeft(0))	//ship1が右に回転した場合
				ship1.rotate(shipNS::RIGHT);
		}
		if (roundOver)
		{
			roundTimer -= frameTime;
			if (roundTimer <= 0)
				roundStart();
		}
	}
	//パララックススクロール用にエンティティの位置を更新
	ship1.update(frameTime);
	//惑星をX方向に動かす
	planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
	//惑星をY方向に動かす
	planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
	planet.update(frameTime);

	//衛星を動かす
	for (int i = 0; i < 4; i++)
	{
		//スクロールの速さを衛星ごとに20%遅くする
		moons[i].setX(moons[i].getX() - frameTime * ship1.getVelocity().x*0.2f*(4 - i));
		moons[i].setY(moons[i].getY() - frameTime * ship1.getVelocity().y*0.2f*(4 - i));
	}

	//星空を宇宙船と反対のX方向に移動
	space.setX(space.getX() - frameTime * ship1.getVelocity().x);
	//星空を宇宙船と反対のY方向に移動
	space.setY(space.getY() - frameTime * ship1.getVelocity().y);

	//星空画像を端で繰り返す
	if (space.getX() > 0)
		//星空画像をSPACE_WIDTH分だけ左に動かす
		space.setX(space.getX() - SPACE_WIDTH);
	//星空画像が画面から外れて左にある場合
	if (space.getX() < -SPACE_WIDTH)
		//星空画像をSPACE_WIDTH分だけ右に動かす
		space.setX(space.getX() + SPACE_WIDTH);
	//星空の上端>画面の上端
	if (space.getY() > 0)
		space.setY(space.getY() - SPACE_HEIGHT);
	//星空画像が画面から外れて上にある場合
	if (space.getY() < -SPACE_HEIGHT)
		space.setY(space.getY() + SPACE_HEIGHT);
}

//人工知能
void SpaceWar::ai()
{}

//衝突処理
void SpaceWar::collisions()
{

}

//ゲームアイテムをレンダー
void SpaceWar::render()
{
	float x = space.getX();
	float y = space.getY();

	graphics->spriteBegin();	//スプライトの描画を開始
	
	
	//星空画像を端で繰り返す
	space.draw();				//現在の位置で描画

	//星空画像の右端が見える場合
	if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
	{
		space.setX(space.getX() + SPACE_WIDTH);		//左端に戻る
		space.draw();								//再描画
	}
	//星空画像の下端が見える場合
	if (space.getY() < -SPACE_HEIGHT + (int)GAME_HEIGHT)
	{
		space.setY(space.getY() + SPACE_HEIGHT);	//上端に戻る
		space.draw();								//再描画
		space.setX(x);								//x位置を復元
		//星空画像の右端が見える場合、左端に戻る
		if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
			space.draw();							//再描画
	}
	space.setX(x);									//x位置を復元
	space.setY(y);									//y位置を復元

	for (int i = 3; i >= 0; i--)
		moons[i].draw();							//奥から手前に衛星の描画
	planet.draw();									//惑星の描画

	ship1.draw();									//宇宙船を描画

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X, spacewarNS::COUNT_DOWN_Y);
	}

	graphics->spriteEnd();		//スプライトの描画を終了
}

//グラフィックスデバイスが消失した時
//グラフィックスデバイスをリセット可能にするため
//予約されていたビデオメモリを全て解放
void SpaceWar::releaseAll()
{
	menuTexture.onLostDevice();
	spaceTexture.onLostDevice();
	textures1.onLostDevice();
	fontScore.onLostDevice();
	fontBig.onLostDevice();
	Game::releaseAll();
	return;
}

//グラフィックデバイスがリセットされた場合
//全てのサーフェイスを再作成
void SpaceWar::resetAll()
{
	fontBig.onResetDevice();
	fontScore.onResetDevice();
	textures1.onResetDevice();
	spaceTexture.onResetDevice();
	menuTexture.onResetDevice();
	Game::resetAll();
	return;
}