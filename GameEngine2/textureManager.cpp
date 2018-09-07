#include "textureManager.h"

//�f�t�H���g�R���X�g���N�^
TextureManager::TextureManager()
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;		//��������������true
}

//�f�X�g���N�^
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}

//�e�N�X�`���t�@�C�����f�B�X�N����ǂݍ���
//���s��F���������ꍇ��true,���s�����ꍇ��false��߂�
bool TextureManager::initialize(Graphics *g, const char *f)
{
	try {
		graphics = g;	//graphics�I�u�W�F�N�g
		file = f;		//�e�N�X�`���t�@�C��

		hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
		if (FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...) { return false; }

	initialized = true;		//����ɏ��������ꂽ�ꍇ�Atrue

	return true;
}

//�O���t�B�b�N�X�f�o�C�X�����������ꍇ�ɌĂяo�����
void TextureManager::onLostDevice()
{
	if (!initialized)
		return;
	SAFE_RELEASE(texture);
}

//�O���t�B�b�N�X�f�o�C�X�����Z�b�g���ꂽ���ɌĂяo�����
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}