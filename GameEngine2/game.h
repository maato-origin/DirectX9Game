#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include "constants.h"
#include "textDX.h"
#include "console.h"
#include "gameError.h"
#include "messageDialog.h"

namespace gameNS
{
	const char FONT[] = "Courier New";	//フォント
	const int POINT_SIZE = 14;			//文字サイズ
	const COLOR_ARGB FONT_COLOR = SETCOLOR_ARGB(255, 255, 255, 255);
}

class Game
{
protected:
	//共通のゲームプロパティ
	Graphics *graphics;			//Graphicsへのポインタ
	Input *input;				//Inputへのポインタ
	Audio *audio;				//Audioへのポインタ
	Console *console;			//Consoleへのポインタ
	MessageDialog *messageDialog;	//MessageDialogへのポインタ
	HWND hwnd;					//ウィンドウハンドル
	HRESULT hr;					//標準の戻り型
	LARGE_INTEGER timeStart;	//パフォーマンスカウンターの開始値
	LARGE_INTEGER timeEnd;		//パフォーマンスカウンターの終了値　
	LARGE_INTEGER timerFreq;		//パフォーマンスカウンターの周波数
	float frameTime;			//最後のフレームに要した時間
	float fps;					//フレームレート
	TextDX dxFont;				//fps用に使うDirectXフォント
	bool fpsOn;					//trueで画面にfpsを表示
	DWORD sleepTime;			//フレーム間でスリープする時間
	bool paused;				//ゲームが一時停止されている場合 true
	bool initialized;
	std::string command;		//コンソールコマンド

public:
	//コンストラクタ
	Game();
	//デストラクタ
	virtual ~Game();
	//メンバー関数

	//Windowsメッセージハンドら
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//ゲームを初期化
	//実行前：hwndはウィンドウへのハンドル
	virtual void initialize(HWND hwnd);
	//WinMain内のメインのメッセージループでrunを繰り返し呼び出す
	virtual void run(HWND);
	//グラフィックデバイス消失時に呼び出す
	//グラフィックデバイスをリセット可能にするため
	//予約されていたビデオメモリを全て解放
	virtual void releaseAll();
	//全てのサーフェイスを再作成し、全てのエンティティをリセット
	virtual void resetAll();
	//予約されていたメモリをすべて削除
	virtual void deleteAll();

	//コンソールコマンドの手続き
	virtual void consoleCommand();

	//ゲームアイテムをレンダー
	virtual void renderGame();
	//消失したグラフィックスデバイスを処理
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	//Graphicsへのポインタを戻す
	Graphics* getGraphics() { return graphics; }
	//Inputへのポインタを戻す
	Input* getInput() { return input; }
	//ゲームを終了
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
	//Audioへのポインタを戻す
	Audio* getAudio() { return audio; }
	
	//純粋仮想関数の宣言
	//これらの関数はGameを継承するクラス内で記述する

	//ゲームアイテムを更新
	virtual void update() = 0;
	//AI計算を実行
	virtual void ai() = 0;
	//衝突チェック
	virtual void collisions() = 0;

	//グラフィックスをレンダー
	//graphics->spriteBegin();を呼び出す
	//スプライトを描画
	//graphics->spriteEnd();を呼び出す
	//スプライト以外を描画
	virtual void render() = 0;
};

#endif // !_GAME_H