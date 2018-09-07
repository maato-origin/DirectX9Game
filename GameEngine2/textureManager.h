#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"

class TextureManager
{
	//TextureManager�̃v���p�e�B
private:
	UINT width;				//�e�N�X�`���̕�
	UINT height;			//�e�N�X�`���̍���
	LP_TEXTURE texture;		//�e�N�X�`���ւ̃|�C���^
	const char *file;		//�t�@�C����
	Graphics *graphics;		//Graphics�ւ̃|�C���^
	bool initialized;		//������������������true��߂�
	HRESULT hr;				//��{�I�ȕԂ�l

public:
	//�R���X�g���N�^
	TextureManager();

	//�f�X�g���N�^
	virtual ~TextureManager();

	//�e�N�X�`���ւ̃|�C���^��߂�
	LP_TEXTURE getTexture() const { return texture; }

	//�e�N�X�`���̕���߂�
	UINT getWidth() const { return width; }

	//�e�N�X�`���̍�����߂�
	UINT getHeight() const { return height; }

	//�e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
	//���s��F���������ꍇ��true,���s�����ꍇ��false��߂�
	virtual bool initialize(Graphics *g, const char *file);

	//�O���t�B�b�N�X�f�o�C�X���������ɌĂяo�����
	virtual void onLostDevice();

	//�O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ�Ƃ��ɌĂяo�����
	virtual void onResetDevice();
};

#endif // !_TEXTUREMANAGER_H
