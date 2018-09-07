#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

//���̃N���X�̓Q�[���G���W���̃R�A
class SpaceWar : public Game
{
private:
	//�Q�[���A�C�e��
	TextureManager nebulaTexture;		//���_�̃e�N�X�`��
	TextureManager planetTexture;		//�f���̃e�N�X�`��
	TextureManager shipTexture;			//�F���D�̃e�N�X�`��
	Image planet;						//�f���̉摜
	Image nebula;						//���_�̉摜
	Image ship;							//�F���D�̉摜

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
	void releaseAll();
	void resetAll();
};

#endif // !_SPACEWAR_H
