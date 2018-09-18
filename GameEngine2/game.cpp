#include "game.h"

//コンストラクタ
Game::Game()
{
	input = new Input();		//キーボード入力初期化
	//その他の初期化は後でinput->initialize()を呼び出して処理
	paused = false;				//ゲームは一時停止中ではない
	graphics = NULL;
	initialized = false;
}

//デストラクタ
Game::~Game()
{
	deleteAll();		//予約されていたメモリを全て解放
	ShowCursor(true);	//カーソルを表示
}

//Windowsメッセージハンドラ
LRESULT Game::messageHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if (initialized)		//初期化されていない場合、メッセージ処理をしない
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);		//Windowsにこのプログラムを終了するよう伝える
				return 0;
			case WM_KEYDOWN: case WM_SYSKEYDOWN:	//キーが押された
				input->keyDown(wParam);
				return 0;
			case WM_KEYUP: case WM_SYSKEYUP:		//キーが離された
				input->keyUp(wParam);
				return 0;
			case WM_CHAR:							//文字入力された
				input->keyIn(wParam);				
				return 0;
			case WM_MOUSEMOVE:						//マウスが移動した
				input->mouseIn(lParam);
				return 0;
			case WM_INPUT:							//マウスからのローデータ入力
				input->mouseRawIn(lParam);
				return 0;
			case WM_LBUTTONDOWN:					//マウスの左ボタンが押された
				input->setMouseLButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_LBUTTONUP:						//マウスの左ボタンが離された
				input->setMouseLButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONDOWN:					//マウスの中央ボタンが押された
				input->setMouseMButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONUP:						//マウスの中央ボタンが離された
				input->setMouseMButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONDOWN:					//マウスの右ボタンが押された
				input->setMouseRButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONUP:						//マウスの右ボタンが離された
				input->setMouseRButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_XBUTTONDOWN: case WM_XBUTTONUP:	//マウスのXボタンが押された/離された
				input->setMouseXButton(wParam);
				input->mouseIn(lParam);
				return 0;
			case WM_DEVICECHANGE:					//コントローラチェック
				input->checkControllers();
				return 0;
		}
	}

	//Windowsに処理を任せる
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//ゲームを初期化
//エラー時にGameErrorをスロー
void Game::initialize(HWND hw)
{
	hwnd = hw;		//ウィンドウハンドルを保存

	//グラフィックスを初期化
	graphics = new Graphics();
	//GameErrorをスロー
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	//入力を初期化、マウスはキャプチャしない
	input->initialize(hwnd, false);			//GameErrorをスロー

	//高分解能タイマーの準備を試みる
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart);	//開始時間を取得

	initialized = true;
}

//ゲームアイテムをレンダー
void Game::renderGame()
{
	//レンダリングを開始
	if (SUCCEEDED(graphics->beginScene()))
	{
		//renderは継承したクラス側で記述する必要がある純粋仮想関数
		render();			//派生クラスのrender()を呼び出す

		graphics->endScene();
	}
	handleLostGraphicsDevice();

	//バックバッファを画面に表示
	graphics->showBackbuffer();
}

//消失したグラフィックスデバイスを処理
void Game::handleLostGraphicsDevice()
{

	//デバイス消失をテストし、それに応じた処理を実行
	hr = graphics->getDeviceState();
	if (FAILED(hr))				//グラフィックスデバイスが有効でない場合
	{
		//デバイスが消失しており、リセットできる状態にない場合
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);			//CPU時間を明け渡す
			return;
		}
		//デバイスが消失しているがリセットできる状態にある場合
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();		//グラフィックスデバイスのリセットを試みる
			if (FAILED(hr))				//リセットが失敗した場合
				return;
			resetAll();
		}
		else
			return;						//他のデバイスエラー
	}
}

//=============================================================================
// Toggle window or fullscreen mode
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}


//WinMain内のメインのメッセージループで繰り返し呼び出される
void Game::run(HWND hwnd)
{
	if (graphics == NULL)	//グラフィックスが初期化されていない場合
		return;

	//最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	//省電力コード
	//希望フレームレートに対して経過時間が短い場合
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);		//1ミリ秒の分解能をWindowsタイマーに要求
		Sleep(sleepTime);		//sleepTimeの間、CPU解放
		timeEndPeriod(1);		//1ミリ秒のタイマー分解能を終了
		return;
	}

	if (frameTime > 0.0f)
		fps = (fps*0.99f) + (0.01f / frameTime);	//平均fps

	if (frameTime > MAX_FRAME_TIME)					//フレームレートが非常に遅い場合
		frameTime = MAX_FRAME_TIME;					//最大frametimeを制限

	timeStart = timeEnd;

	//update(),ai(),collisions()は純粋仮想関数
	//Gameを継承しているクラス側で記述する
	if (!paused)									//一時停止中でない場合
	{
		update();									//全てのゲームアイテムを更新
		ai();										//人工知能
		collisions();								//衝突処理
		input->vibrateControllers(frameTime);		//コントローラの振動を処理
	}
	renderGame();									//全てのゲームアイテム描画

	input->readControllers();						//コントローラの状態を読み取る

	//入力をクリア
	//全てのキーチェック完了後これを呼び出す
	input->clear(inputNS::KEYS_PRESSED);
}

void Game::releaseAll()
{}

void Game::resetAll()
{}

void Game::deleteAll()
{
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}