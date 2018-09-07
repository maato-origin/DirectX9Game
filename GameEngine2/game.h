#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

class Game
{
protected:
	//���ʂ̃Q�[���v���p�e�B
	Graphics *graphics;			//�O���t�B�b�N�X�ւ̃|�C���^
	Input *input;				//Input�ւ̃|�C���^
	HWND hwnd;					//�E�B���h�E�n���h��
	HRESULT hr;					//�W���̖߂�^
	LARGE_INTEGER timeStart;	//�p�t�H�[�}���X�J�E���^�[�̊J�n�l
	LARGE_INTEGER timeEnd;		//�p�t�H�[�}���X�J�E���^�[�̏I���l�@
	LARGE_INTEGER timerFreq;		//�p�t�H�[�}���X�J�E���^�[�̎��g��
	float frameTime;			//�Ō�̃t���[���ɗv��������
	float fps;					//�t���[�����[�g
	DWORD sleepTime;			//�t���[���ԂŃX���[�v���鎞��
	bool paused;				//�Q�[�����ꎞ��~����Ă���ꍇ true
	bool initialized;

public:
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	virtual ~Game();
	//�����o�[�֐�

	//Windows���b�Z�[�W�n���h��
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//�Q�[����������
	//���s�O�Fhwnd�̓E�B���h�E�ւ̃n���h��
	virtual void initialize(HWND hwnd);
	//WinMain���̃��C���̃��b�Z�[�W���[�v��run���J��Ԃ��Ăяo��
	virtual void run(HWND);
	//�O���t�B�b�N�f�o�C�X�������ɌĂяo��
	//�O���t�B�b�N�f�o�C�X�����Z�b�g�\�ɂ��邽��
	//�\�񂳂�Ă����r�f�I��������S�ĉ��
	virtual void releaseAll();
	//�S�ẴT�[�t�F�C�X���č쐬���A�S�ẴG���e�B�e�B�����Z�b�g
	virtual void resetAll();
	//�\�񂳂�Ă��������������ׂč폜
	virtual void deleteAll();
	//�Q�[���A�C�e���������_�[
	virtual void renderGame();
	//���������O���t�B�b�N�X�f�o�C�X������
	virtual void handleLostGraphicsDevice();
	//Graphics�ւ̃|�C���^��߂�
	Graphics* getGraphics() { return graphics; }
	//Input�ւ̃|�C���^��߂�
	Input* getInput() { return input; }
	//�Q�[�����I��
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }
	
	//�������z�֐��̐錾
	//�����̊֐���Game���p������N���X���ŋL�q����

	//�Q�[���A�C�e�����X�V
	virtual void update() = 0;
	//AI�v�Z�����s
	virtual void ai() = 0;
	//�Փ˃`�F�b�N
	virtual void collisions() = 0;

	//�O���t�B�b�N�X�������_�[
	//graphics->spriteBegin();���Ăяo��
	//�X�v���C�g��`��
	//graphics->spriteEnd();���Ăяo��
	//�X�v���C�g�ȊO��`��
	virtual void render() = 0;
};

#endif // !_GAME_H