#include "spaceWar.h"

//�R���X�g���N�^
SpaceWar::SpaceWar()
{}

//�f�X�g���N�^
SpaceWar::~SpaceWar()
{
	releaseAll();		//�S�ẴO���t�B�b�N�X�A�C�e���ɂ���onLostDevice()���Ăяo��
}

//�Q�[����������
//�G���[����GameError���X���[
void SpaceWar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);	//GameError���X���[
	//���_�̃e�N�X�`��
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));
	//�f���̃e�N�X�`��
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
	//�F���D�̃e�N�X�`��
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));
	//���_
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	//�f��
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	//��ʂ̒����ɘf����z�u
	planet.setX(GAME_WIDTH*0.5f - planet.getWidth()*0.5f);
	planet.setY(GAME_HEIGHT*0.5f - planet.getHeight()*0.5f);

	//�F���D
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
	ship.setX(GAME_WIDTH / 4);								//�f���̍��ォ��o��
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);		//�A�j���[�V�����̃t���[��
	ship.setCurrentFrame(SHIP_START_FRAME);					//�J�n�t���[��
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	ship.setDegrees(45.0f);									//�F���D�̊p�x

	return;
}

//�S�ẴQ�[���A�C�e�����X�V
void SpaceWar::update()
{
	if (input->isKeyDown(SHIP_RIGHT_KEY))						//�E�Ɉړ�����ꍇ
	{
		ship.setX(ship.getX() + frameTime * SHIP_SPEED);
		if (ship.getX() > GAME_WIDTH)							//��ʂ̉E�ɂ͂ݏo���ꍇ
			ship.setX((float)-ship.getWidth());					//��ʂ̍��ɔz�u
	}

	if (input->isKeyDown(SHIP_LEFT_KEY))						//���Ɉړ�����ꍇ
	{
		ship.setX(ship.getX() - frameTime * SHIP_SPEED);
		if (ship.getX() < -ship.getWidth())						//��ʂ̍��ɂ͂ݏo���ꍇ
			ship.setX((float)GAME_WIDTH);						//��ʂ̉E�ɔz�u
	}

	if (input->isKeyDown(SHIP_UP_KEY))							//��Ɉړ�����ꍇ
	{
		ship.setY(ship.getY() - frameTime * SHIP_SPEED);
		if (ship.getY() < -ship.getHeight())						//��ʂ̏�ɂ͂ݏo���ꍇ
			ship.setY((float)GAME_HEIGHT);						//��ʂ̉��ɔz�u
	}

	if (input->isKeyDown(SHIP_DOWN_KEY))						//���Ɉړ�����ꍇ
	{
		ship.setY(ship.getY() + frameTime * SHIP_SPEED);
		if (ship.getY() > GAME_HEIGHT)							//��ʂ̉��ɂ͂ݏo���ꍇ
			ship.setY((float)-ship.getHeight());				//��ʂ̏�ɔz�u
	}

	ship.update(frameTime);
}

//�l�H�m�\
void SpaceWar::ai()
{}

//�Փˏ���
void SpaceWar::collisions()
{}

//�Q�[���A�C�e���������_�[
void SpaceWar::render()
{
	graphics->spriteBegin();	//�X�v���C�g�̕`����J�n
	
	nebula.draw();				//���_���V�[���ɒǉ�
	planet.draw();				//�f�����V�[���ɒǉ�
	ship.draw();				//�F���D���V�[���ɒǉ�

	graphics->spriteEnd();		//�X�v���C�g�̕`����I��
}

//�O���t�B�b�N�X�f�o�C�X������������
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽��
//�\�񂳂�Ă����r�f�I��������S�ĉ��
void SpaceWar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();
	Game::releaseAll();
	return;
}

//�O���t�B�b�N�f�o�C�X�����Z�b�g���ꂽ�ꍇ
//�S�ẴT�[�t�F�C�X���č쐬
void SpaceWar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	shipTexture.onResetDevice();
	Game::resetAll();
	return;
}