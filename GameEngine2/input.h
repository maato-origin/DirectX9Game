#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <Windows.h>
#include <windowsx.h>
#include <string.h>
#include <XInput.h>
#include "constants.h"
#include "gameError.h"

//高精細マウス用
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC		((USHORT)0x01)
#endif // !HID_USAGE_PAGE_GENERIC
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE		((USHORT)0x02)
#endif // !HID_USAGE_GENERIC_MOUSE


namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;		//キー配列サイズ

	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

const DWORD GAMEPAD_THUMBSTICK_DEADZONE = (DWORD)(0.20f * 0X7FFF);	//デッドゾーンとして範囲の20％をデフォルトとする
const DWORD GAMEPAD_TRIGGER_DEADZONE = 30;	//トリガー範囲は0～255
const DWORD MAX_CONTROLLERS = 4;

//state.Gamepad.wButtonsのゲームパッドボタンに対応するビット
const DWORD GAMEPAD_DPAD_UP = 0x0001;
const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
const DWORD GAMEPAD_START_BUTTON = 0x0010;
const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
const DWORD GAMEPAD_A = 0x1000;
const DWORD GAMEPAD_B = 0x2000;
const DWORD GAMEPAD_X = 0x4000;
const DWORD GAMEPAD_Y = 0x8000;

struct ControllerState
{
	XINPUT_STATE state;
	XINPUT_VIBRATION vibration;
	float vibrateTimeLeft;	//ミリ秒
	float vibrateTimeRight;	//ミリ秒
	bool connected;
};

class Input
{
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEN];		//指定のキーが押されている状態の場合true
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];	//指定のキーが押された場合にtrue
	std::string textIn;							//ユーザが入力したテキスト
	char charIn;								//最後に入力された文字
	bool newLine;								//新しい行の開始時にtrue
	int mouseX, mouseY;							//マウスの画面座標
	int mouseRawX, mouseRawY;					//高精細マウスのデータ
	RAWINPUTDEVICE Rid[1];						//高精細マウス用
	bool mouseCaptured;							//マウスがキャプチャされている場合true
	bool mouseLButton;							//左マウスボタンが押されている場合にtrue
	bool mouseMButton;							//中央マウスボタンが押されている場合にtrue
	bool mouseRButton;							//右マウスボタンが押されている場合にtrue
	bool mouseX1Button;							//X1マウスボタンが押されている場合にtrue
	bool mouseX2Button;							//X2マウスボタンが押されている場合にtrue
	ControllerState controllers[MAX_CONTROLLERS];	//コントローラの状態

public:
	//コンストラクタ
	Input();

	//デストラクタ
	virtual ~Input();

	//マウスとコントローラの入力を初期化
	//マウスをキャプチャする場合capture=trueを設定
	//GameErrorをスロー
	void initialize(HWND hwnd, bool capture);

	//キー押下を保存
	void keyDown(WPARAM);

	//キー押上を保存
	void keyUp(WPARAM);

	//入力された文字を textIn に保存
	void keyIn(WPARAM);

	//指定された仮想キーが押されている場合はtrue, それ以外はfalse を戻す
	bool isKeyDown(UCHAR vkey) const;

	//直近のフレームにおいて指定の仮想キーが押されたことがある場合 trueを戻す
	//キーの押し下げの状態は各フレームの終了時に消去される
	bool wasKeyPressed(UCHAR vkey) const;

	//直近のフレームにおいて何らかのキーが押された場合trueを戻す
	//キーの押し下げの状態は各フレームの終了時に商況される
	bool anyKeyPressed() const;

	//指定されたキーの押し下げをクリア
	void clearKeyPress(UCHAR vkey);

	//指定の入力バッファをクリア
	void clear(UCHAR what);

	//キー、マウス、テキスト入力データをクリア
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); }

	//テキスト入力バッファをクリア
	void clearTextIn() { textIn.clear(); }

	//テキスト入力を文字列として返す
	std::string getTextIn() { return textIn; }

	//最後に入力された文字を戻す
	char getCharIn() { return charIn; }

	//マウスの画面位置を読み取り、mouseXとmouseYに保存
	void mouseIn(LPARAM);

	//マウスからローデータを読み取り、mouseRawXとmouseRawYに保存
	//このルーチンは高精細マウスに対応
	void mouseRawIn(LPARAM);

	//マウスボタンの状態を保存
	void setMouseLButton(bool b) { mouseLButton = b; }

	//マウスボタンの状態を保存
	void setMouseMButton(bool b) { mouseMButton = b; }
	
	//マウスボタンの状態を保存
	void setMouseRButton(bool b) { mouseRButton = b; }

	//マウスボタンの状態を保存
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	//マウスのX位置を戻す
	int getMouseX() const { return mouseX; }

	//マウスのY位置を戻す
	int getMouseY() const { return mouseY; }

	//マウスのX位置の移動のローデータを戻す。左への移動は<0,右への移動は>0
	//高精細マウス対応
	int getMouseRawX() const { return mouseRawX; }

	//マウスのY位置の移動のローデータを戻す。上への移動は<0,下への移動は>0
	//高精細マウス対応
	int getMouseRawY() const { return mouseRawY; }

	//左マウスボタンの状態を戻す
	bool getMouseLButton() const { return mouseLButton; }

	//中央マウスボタンの状態を戻す
	bool getMouseMButton() const { return mouseMButton; }

	//右マウスボタンの状態を戻す
	bool getMouseRButton() const { return mouseRButton; }

	//X1マウスボタンの状態を戻す
	bool getMouseX1Button() const { return mouseX1Button; }

	//X2マウスボタンの状態を戻す
	bool getMouseX2Button() const { return mouseX2Button; }

	//接続されているコントローラをチェック
	void checkControllers();

	//接続されているコントローラの状態を読み取る
	void readControllers();

	//指定のゲームコントローラの状態を戻す
	const ControllerState* getControllerState(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return &controllers[n];
	}

	//コントローラnのボタンの状態を戻す
	const WORD getGamepadButtons(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return controllers[n].state.Gamepad.wButtons;
	}

	//コントローラnの方向パッド上の状態を戻す
	bool getGamepadDPadUp(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_UP) != 0);
	}

	//コントローラnの方向パッド下の状態を戻す
	bool getGamepadDPadDown(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_DOWN) != 0);
	}

	//コントローラnの方向パッド左の状態を戻す
	bool getGamepadDPadLeft(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_LEFT) != 0);
	}

	//コントローラnの方向パッド右の状態を戻す
	bool getGamepadDPadRight(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_DPAD_RIGHT) != 0);
	}

	//コントローラnのSTARTボタンの状態を戻す
	bool getGamepadStart(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_START_BUTTON) != 0);
	}

	//コントローラnのBACKボタンの状態を戻す
	bool getGamepadBack(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_BACK_BUTTON) != 0);
	}

	//コントローラnの左サムボタンの状態を戻す
	bool getGamepadLeftThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_THUMB) != 0);
	}

	//コントローラnの右サムボタンの状態を戻す
	bool getGamepadRightThumb(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_THUMB) != 0);
	}

	//コントローラnの左ショルダーボタンの状態を戻す
	bool getGamepadLeftShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_LEFT_SHOULDER) != 0);
	}

	//コントローラnの右ショルダーボタンの状態を戻す
	bool getGamepadRightShoulder(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_RIGHT_SHOULDER) != 0);
	}

	//コントローラnのAボタンの状態を戻す
	bool getGamepadA(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_A) != 0);
	}

	//コントローラnのBボタンの状態を戻す
	bool getGamepadB(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_B) != 0);
	}

	//コントローラnのXボタンの状態を戻す
	bool getGamepadX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_X) != 0);
	}

	//コントローラnのYボタンの状態を戻す
	bool getGamepadY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return ((controllers[n].state.Gamepad.wButtons & GAMEPAD_Y) != 0);
	}

	//コントローラnの左トリガーの値を戻す
	BYTE getGamepadLeftTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bLeftTrigger);
	}

	//コントローラnの右トリガーの値を戻す
	BYTE getGamepadRightTrigger(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.bRightTrigger);
	}

	//コントローラnの左サムスティック、Xの値を戻す
	SHORT getGamepadThumbLX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLX);
	}

	//コントローラnの左サムスティック、Yの値を戻す
	SHORT getGamepadThumbLY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbLY);
	}

	//コントローラnの右サムスティック、Xの値を戻す
	SHORT getGamepadThumbRY(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRX);
	}

	//コントローラnの右サムスティック、Yの値を戻す
	SHORT getGamepadThumbRX(UINT n)
	{
		if (n > MAX_CONTROLLERS - 1)
			n = MAX_CONTROLLERS - 1;
		return (controllers[n].state.Gamepad.sThumbRY);
	}

	//コントローラnの左モーターを振動
	//左は低周波振動
	//speed:0=off,65535=100%
	//sec:振動させる時間(秒)
	void gamePadVibrateLeft(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wLeftMotorSpeed = speed;
		controllers[n].vibrateTimeLeft = sec;
	}

	//コントローラnの右モーターを振動
	//左は低周波振動
	//speed:0=off,65535=100%
	//sec:振動させる時間(秒)
	void gamePadVibrateRight(UINT n, WORD speed, float sec)
	{
		if (n > MAX_CONTROLLERS)
			n = MAX_CONTROLLERS - 1;
		controllers[n].vibration.wRightMotorSpeed = speed;
		controllers[n].vibrateTimeRight = sec;
	}

	//接続されているコントローラを振動させる
	void vibrateControllers(float frameTime);
};

#endif // !_INPUT_H