#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_MEAN

#include "textureManager.h"
#include "constants.h"

class Image
{
	//Image�̃v���p�e�B
protected:
	Graphics *graphics;					//Graphics�ւ̃|�C���^
	TextureManager *textureManager;		//textureManager�ւ̃|�C���^
	SpriteData spriteData;				//Graphics::drawSprite�֐����K�v�Ƃ���f�[�^���i�[����\����
	COLOR_ARGB colorFilter;			//color�p�����[�^��FILTER���w�肳�ꂽ�ꍇ�A�X�v���C�g�̕`�掞�Ƀt�B���^�Ƃ��ēK�p����F
	int cols;							//�����t���[�������X�v���C�g���̗�
	int startFrame;						//�����t���[�������A�j���[�V�����V�[�P���X�̍ŏ��̃t���[���ԍ�
	int endFrame;						//�����t���[�������A�j���[�V�����V�[�P���X�̍ŏ��̃t���[���ԍ�
	int currentFrame;					//�A�j���[�V�����V�[�P���X�̌��݂̃t���[���ԍ�
	float frameDelay;					//�A�j���[�V�����Ԃ̊Ԋu
	float animTimer;					//�A�j���[�V�����̎��Ԃ��v��
	HRESULT hr;							//�W���̖߂�^
	bool loop;							//true�ŃA�j���[�V�����J��Ԃ�
	bool visible;						//true�Ȃ�摜��
	bool initialized;					//������������true
	bool animComplete;					//loop��false����endFrame���������Ă����true

public:
	//�R���X�g���N�^
	Image();

	//�f�X�g���N�^
	virtual ~Image();

	//�擾�֐�

	//SpriteData�\���̂̎Q�Ƃ�߂�
	const virtual SpriteData& getSpriteInfo() { return spriteData; }

	//visible�̒l��߂�
	virtual bool getVisible() { return visible; }

	//X�̈ʒu��߂�
	virtual float getX() { return spriteData.x; }

	//Y�̈ʒu��߂�
	virtual float getY() { return spriteData.y; }

	//scale�̒l��߂�
	virtual float getScale() { return spriteData.scale; }

	//Width��߂�
	virtual float getWidth() { return spriteData.width; }

	//Height��߂�
	virtual float getHeight() { return spriteData.height; }

	//X���W�̒��S��߂�
	virtual float getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }

	//Y���W�̒��S��߂�
	virtual float getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }

	//��]�p�x��߂�
	virtual float getDegrees() { return spriteData.angle*(180.0f / (float)PI); }

	//��]�p�����W�A���Ŗ߂�
	virtual float getRadians() { return spriteData.angle; }

	//�A�j���[�V�����t���[���Ԃ̊Ԋu��߂�
	virtual float getFrameDelay() { return frameDelay; }

	//�J�n�t���[���l��߂�
	virtual int getStartFrame() { return startFrame; }

	//�I���t���[���l��߂�
	virtual int getEndFrame() { return endFrame; }

	//���݂̃t���[���l��߂�
	virtual int getCurrentFrame() { return currentFrame; }

	//Image��RECT�\���̂�߂�
	virtual RECT getSpriteDataRect() { return spriteData.rect; }

	//�A�j���[�V����������Ԃ�߂�
	virtual bool getAnimationComplete() { return animComplete; }

	//colorFilter��߂�
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }

	//�ݒ�֐�
	//X�ʒu��ݒ�
	virtual void setX(float newX) { spriteData.x = newX; }

	//Y�ʒu��ݒ�
	virtual void setY(float newY) { spriteData.y = newY; }

	//�g��k����ݒ�
	virtual void setScale(float s) { spriteData.scale = s; }

	//��]�p�x��ݒ�
	//0�x�͏�����B�p�x�͎��v���ɐi��
	virtual void setDegrees(float deg) { spriteData.angle = deg * ((float)PI / 180.0f); }

	//��]�p�x��ݒ�i���W�A���j
	//0���W�A���͏�����B�p�x�͎��v���ɐi��
	virtual void setRadians(float rad) { spriteData.angle = rad; }

	//������ݒ�
	virtual void setVisible(bool v) { visible = v; }

	//�A�j���[�V�����̃t���[���Ԃ̊Ԋu��ݒ�
	virtual void setFrameDelay(float d) { frameDelay = d; }

	//�A�j���[�V�����̊J�n�t���[���ƏI���t���[����ݒ�
	virtual void setFrames(int s, int e) { startFrame = s; endFrame = e; }

	//�A�j���[�V�����̌��݂̃t���[�����擾
	virtual void setCurrentFrame(int c);

	//currentFrame��`�悷��spriteData.rect��ݒ�
	virtual void setRect();

	//spriteData.rect��r�ɐݒ�
	virtual void setSpriteDataRect(RECT r) { spriteData.rect = r; }

	//�A�j���[�V�������[�v��ݒ�(lp=true�̏ꍇ�Ƀ��[�v)
	virtual void setLoop(bool lp) { loop = lp; }

	//�A�j���[�V���������u�[���l��ݒ�
	virtual void setAnimationComplete(bool a) { animComplete = a; }

	//�F�t�B���^��ݒ�
	virtual void setColorFilter(COLOR_ARGB color) { colorFilter = color; }

	//textureManager��ݒ�
	virtual void setTextureManager(TextureManager *textureM) { textureManager = textureM; }

	//���̑��֐�
	
	//Image��������
	//���s��F���������ꍇ��true,���s�����ꍇ��false��߂�
	//*g��Graphics�ւ̃|�C���^
	//width��Image�̕�(0�̎��e�N�X�`���S�̂̕����g�p)
	//height��Image�̍���(0�̎��e�N�X�`���S�̂̍������g�p)
	//ncols�̓e�N�X�`�����̗�(1�`n�܂�)(0��1�Ɠ���)
	//*textureM��TextureManager�ւ̃|�C���^
	virtual bool initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM);

	//�摜�𐅕������ɔ��]�i���j
	virtual void flipHorizontal(bool flip) { spriteData.flipHorizontal = flip; }

	//�摜�𐂒������ɔ��]
	virtual void flipVertical(bool flip) { spriteData.flipVertical = flip; }

	//color���t�B���^�Ƃ��Ďg���ĉ摜��`��Bcolor�̃f�t�H���g�͔�
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	//�w�肳�ꂽSpriteData���g���ĉ摜��`��
	//���݂�SpriteData.rect���g�p���ăe�N�X�`����I��
	virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE);

	//�A�j���[�V�����̍X�V�BframeTime�͊�l�Ƃ��Ďg����
	virtual void update(float frameTime);
};

#endif // !_IMAGE_H
