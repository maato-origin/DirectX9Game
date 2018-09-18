#include "game.h"

//�R���X�g���N�^
Game::Game()
{
	input = new Input();		//�L�[�{�[�h���͏�����
	//���̑��̏������͌��input->initialize()���Ăяo���ď���
	paused = false;				//�Q�[���͈ꎞ��~���ł͂Ȃ�
	graphics = NULL;
	initialized = false;
}

//�f�X�g���N�^
Game::~Game()
{
	deleteAll();		//�\�񂳂�Ă�����������S�ĉ��
	ShowCursor(true);	//�J�[�\����\��
}

//Windows���b�Z�[�W�n���h��
LRESULT Game::messageHandler(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	if (initialized)		//����������Ă��Ȃ��ꍇ�A���b�Z�[�W���������Ȃ�
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);		//Windows�ɂ��̃v���O�������I������悤�`����
				return 0;
			case WM_KEYDOWN: case WM_SYSKEYDOWN:	//�L�[�������ꂽ
				input->keyDown(wParam);
				return 0;
			case WM_KEYUP: case WM_SYSKEYUP:		//�L�[�������ꂽ
				input->keyUp(wParam);
				return 0;
			case WM_CHAR:							//�������͂��ꂽ
				input->keyIn(wParam);				
				return 0;
			case WM_MOUSEMOVE:						//�}�E�X���ړ�����
				input->mouseIn(lParam);
				return 0;
			case WM_INPUT:							//�}�E�X����̃��[�f�[�^����
				input->mouseRawIn(lParam);
				return 0;
			case WM_LBUTTONDOWN:					//�}�E�X�̍��{�^���������ꂽ
				input->setMouseLButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_LBUTTONUP:						//�}�E�X�̍��{�^���������ꂽ
				input->setMouseLButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONDOWN:					//�}�E�X�̒����{�^���������ꂽ
				input->setMouseMButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_MBUTTONUP:						//�}�E�X�̒����{�^���������ꂽ
				input->setMouseMButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONDOWN:					//�}�E�X�̉E�{�^���������ꂽ
				input->setMouseRButton(true);
				input->mouseIn(lParam);
				return 0;
			case WM_RBUTTONUP:						//�}�E�X�̉E�{�^���������ꂽ
				input->setMouseRButton(false);
				input->mouseIn(lParam);
				return 0;
			case WM_XBUTTONDOWN: case WM_XBUTTONUP:	//�}�E�X��X�{�^���������ꂽ/�����ꂽ
				input->setMouseXButton(wParam);
				input->mouseIn(lParam);
				return 0;
			case WM_DEVICECHANGE:					//�R���g���[���`�F�b�N
				input->checkControllers();
				return 0;
		}
	}

	//Windows�ɏ�����C����
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//�Q�[����������
//�G���[����GameError���X���[
void Game::initialize(HWND hw)
{
	hwnd = hw;		//�E�B���h�E�n���h����ۑ�

	//�O���t�B�b�N�X��������
	graphics = new Graphics();
	//GameError���X���[
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	//���͂��������A�}�E�X�̓L���v�`�����Ȃ�
	input->initialize(hwnd, false);			//GameError���X���[

	//������\�^�C�}�[�̏��������݂�
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart);	//�J�n���Ԃ��擾

	initialized = true;
}

//�Q�[���A�C�e���������_�[
void Game::renderGame()
{
	//�����_�����O���J�n
	if (SUCCEEDED(graphics->beginScene()))
	{
		//render�͌p�������N���X���ŋL�q����K�v�����鏃�����z�֐�
		render();			//�h���N���X��render()���Ăяo��

		graphics->endScene();
	}
	handleLostGraphicsDevice();

	//�o�b�N�o�b�t�@����ʂɕ\��
	graphics->showBackbuffer();
}

//���������O���t�B�b�N�X�f�o�C�X������
void Game::handleLostGraphicsDevice()
{

	//�f�o�C�X�������e�X�g���A����ɉ��������������s
	hr = graphics->getDeviceState();
	if (FAILED(hr))				//�O���t�B�b�N�X�f�o�C�X���L���łȂ��ꍇ
	{
		//�f�o�C�X���������Ă���A���Z�b�g�ł����ԂɂȂ��ꍇ
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);			//CPU���Ԃ𖾂��n��
			return;
		}
		//�f�o�C�X���������Ă��邪���Z�b�g�ł����Ԃɂ���ꍇ
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();		//�O���t�B�b�N�X�f�o�C�X�̃��Z�b�g�����݂�
			if (FAILED(hr))				//���Z�b�g�����s�����ꍇ
				return;
			resetAll();
		}
		else
			return;						//���̃f�o�C�X�G���[
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


//WinMain���̃��C���̃��b�Z�[�W���[�v�ŌJ��Ԃ��Ăяo�����
void Game::run(HWND hwnd)
{
	if (graphics == NULL)	//�O���t�B�b�N�X������������Ă��Ȃ��ꍇ
		return;

	//�Ō�̃t���[������̌o�ߎ��Ԃ��v�Z�AframeTime�ɕۑ�
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	//�ȓd�̓R�[�h
	//��]�t���[�����[�g�ɑ΂��Čo�ߎ��Ԃ��Z���ꍇ
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);		//1�~���b�̕���\��Windows�^�C�}�[�ɗv��
		Sleep(sleepTime);		//sleepTime�̊ԁACPU���
		timeEndPeriod(1);		//1�~���b�̃^�C�}�[����\���I��
		return;
	}

	if (frameTime > 0.0f)
		fps = (fps*0.99f) + (0.01f / frameTime);	//����fps

	if (frameTime > MAX_FRAME_TIME)					//�t���[�����[�g�����ɒx���ꍇ
		frameTime = MAX_FRAME_TIME;					//�ő�frametime�𐧌�

	timeStart = timeEnd;

	//update(),ai(),collisions()�͏������z�֐�
	//Game���p�����Ă���N���X���ŋL�q����
	if (!paused)									//�ꎞ��~���łȂ��ꍇ
	{
		update();									//�S�ẴQ�[���A�C�e�����X�V
		ai();										//�l�H�m�\
		collisions();								//�Փˏ���
		input->vibrateControllers(frameTime);		//�R���g���[���̐U��������
	}
	renderGame();									//�S�ẴQ�[���A�C�e���`��

	input->readControllers();						//�R���g���[���̏�Ԃ�ǂݎ��

	//���͂��N���A
	//�S�ẴL�[�`�F�b�N�����ケ����Ăяo��
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