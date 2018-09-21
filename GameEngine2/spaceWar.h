#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "planet.h"
#include "ship.h"

namespace spacewarNS
{
	const char FONT[] = "Arial Bold";	//�t�H���g
	const int FONT_BIG_SIZE = 256;		//�t�H���g�̍���
	const int FONT_SCORE_SIZE = 48;
	const COLOR_ARGB FONT_COLOR = graphicsNS::YELLOW;
	const COLOR_ARGB SHIP1_COLOR = graphicsNS::BLUE;
	const int SCORE_Y = 10;
	const int SCORE1_X = 60;
	const int SHIP1_HEALTHBAR_X = 40;
	const int HEALTHBAR_START_FRAME = 22;
	const int HEALTHBAR_END_FRAME = 22;
	const int HEALTHBAR_HEIGHT = 32;
	const int HEALTHBAR_WIDTH = 64;
	const int COUNT_DOWN_X = GAME_WIDTH / 2 - FONT_BIG_SIZE / 4;
	const int COUNT_DOWN_Y = GAME_HEIGHT / 2 - FONT_BIG_SIZE / 2;
	const int COUNT_DOWN = 2;			//�J�E���g�_�E��
	const int BUF_SIZE = 20;
	const int ROUND_TIME = 5;			//�V�������E���h�J�n�܂ł̎���
	const int HEALTHBAR_TEXTURE_COLS = 4;	//�̗̓o�[��4�񕪃e�N�X�`�����g�p
}

//���̃N���X�̓Q�[���G���W���̃R�A
class SpaceWar : public Game
{
private:
	//�Q�[���A�C�e��
	TextureManager menuTexture, spaceTexture, textures1;
	Ship ship1;					//�F���D�̃N���X
	Image space;						//����̉摜
	Image planet;
	Image moons[4];
	Image menu;							//���j���[�̉摜
	bool menuOn;
	bool countDownOn;					//�J�E���g�_�E�����\�������Ƃ�true
	float countDownTimer;
	TextDX fontBig;						//�Q�[���o�i�[�pDirectX�t�H���g
	TextDX fontScore;
	char buffer[spacewarNS::BUF_SIZE];
	bool ship1Scored;					//���E���h�̉F���D�X�R�A���L�^������true
	bool roundOver;						//���E���h���z������true
	float roundTimer;					//�V�������E���h�J�n�܂ł̎���
	int ship1Score;						//�X�R�A

public:
	//�R���X�g���N�^
	SpaceWar();
	
	//�f�X�g���N�^
	virtual ~SpaceWar();

	//�Q�[����������
	void initialize(HWND hwnd);
	void update();		//Game����̏������z�֐����I�[�o�[���C�h
	void ai();
	void collisions();
	void render();
	void roundStart();	//�V���ȃQ�[�����E���h���J�n
	void releaseAll();
	void resetAll();
};

#endif // !_SPACEWAR_H
