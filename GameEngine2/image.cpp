#include "image.h"

//�f�t�H���g�R���X�g���N�^
Image::Image()
{
	initialized = false;						//��������������true��ݒ�
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0;
	spriteData.y = 0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;					//�����̃t���[���摜�̈��I�Ԃ̂Ɏg�p
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;					//�X�v���C�g�p�e�N�X�`��
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0f;							//�A�j���[�V�����̊e�t���[����1�b�ɐݒ�
	animTimer = 0.0;
	visible = true;								//�C���[�W������
	loop = true;								//�t���[���J��Ԃ�
	animComplete = false;
	graphics = NULL;							//graphic system�ւƌq��
	colorFilter = graphicsNS::WHITE;			//������ύX�Ȃ�
}

//�f�X�g���N�^
Image::~Image()
{}

//Image��������
//���s��F��������true,���s����false��߂�
bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM)
{
	try {
		graphics = g;						//graphics�I�u�W�F�N�g
		textureManager = textureM;			//�e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^

		spriteData.texture = textureManager->getTexture();
		if (width == 0)
			width = textureManager->getWidth();		//�S�̂̕����g�p
		spriteData.width = width;

		if (height == 0)
			height = textureManager->getHeight();	//�S�̂̍������g�p
		spriteData.height = height;
		cols = ncols;
		if (cols == 0)
			cols = 1;								//0cols ��1�Ƃ݂Ȃ�

		//currentFrame��`�悷��spriteData.rect���\��
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		//�E�[+1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		//���[+1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { return false; }
	initialized = true;								//����ɏ��������ꂽ�ꍇ

	return true;
}

//color���t�B���^�Ƃ��Ďg���ĉ摜��`��
//color�p�����[�^�̓I�v�V����
//�f�t�H���g�Ŕ������蓖�Ă��Ă���
//���s�O�FspriteBegin()���Ăяo��
//���s��FspriteEnd()���Ăяo��
void Image::draw(COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//onReset()���Ăяo���ꂽ���ɐV�����e�N�X�`�����擾
	spriteData.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)		//�t�B���^���g���ĕ`�悷��ꍇ
		//colorFilter���g�p
		graphics->drawSprite(spriteData, colorFilter);
	else
		//�t�B���^�Ƃ���color���g�p
		graphics->drawSprite(spriteData, color);
}

//�w�肳�ꂽSpriteData���g���Ă��̉摜��`��
//���݂�SpriteData.rect���g���ăe�N�X�`����I��
//���s�O�FspriteBegin()���Ăяo��
//���s��FspriteEnd()���Ăяo��
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	//���̉摜�̋�`���g���ăe�N�X�`����I��
	sd.rect = spriteData.rect;
	//onReset()���Ăяo���ꂽ���ɐV�����e�N�X�`�����擾
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER)				//�t�B���^���g���ĕ`�悷��ꍇ
		graphics->drawSprite(sd, colorFilter);		//colorFilter���g�p
	else
		graphics->drawSprite(sd, color);			//�t�B���^�Ƃ���color���g�p
}

//�t���[������1��Ăяo��
//frameTime�͈ړ��ƃA�j���[�V�����̑����𐧌䂷�邽�߂Ɏg�p
void Image::update(float frameTime)
{
	if (endFrame - startFrame > 0)									//�A�j���[�V����������X�v���C�g�̏ꍇ
	{
		animTimer += frameTime;										//���v�̌o�ߎ���
		if (animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame<startFrame || currentFrame>endFrame)
			{
				if (loop == true)									//���[�v����A�j���[�V�����̏ꍇ
					currentFrame = startFrame;
				else												//���[�v���Ȃ��A�j���[�V�����̏ꍇ
				{
					currentFrame = endFrame;
					animComplete = true;							//�A�j���[�V�����̊���
				}
			}
			setRect();												//spriteData.rect�̐ݒ�
		}
	}
}

//�����t���[�������摜���Ō��݂̃t���[����ݒ肷��
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = 1;
		animComplete = false;
		setRect();				//spriteData.rect��ݒ�
	}
}

//currentFrame��`�悷��spriteData.rect�\���̂�ݒ肷��
inline void Image::setRect()
{
	//currentFrame��`�悷��spriteData.rect���\��
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}