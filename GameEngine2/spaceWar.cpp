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
	//���C���̃Q�[���e�N�X�`��
	if (!gameTextures.initialize(graphics, TEXTURES_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));
	//���_
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));
	//�f��
	if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

	//�F���D1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);		//�A�j���[�V�����̃t���[��
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);							//�J�n�t���[��
	ship1.setX(GAME_WIDTH / 4);													//�f���̍��ォ��o��
	ship1.setY(GAME_HEIGHT / 4);
	ship1.setVelocity(VECTOR2(shipNS::SPEED, -shipNS::SPEED));					//VECTOR2(X, Y)

	//�F���D2
	if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
	ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);		//�A�j���[�V�����̃t���[��
	ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);							//�J�n�t���[��
	ship2.setX(GAME_WIDTH - GAME_WIDTH / 4);									//�f���̍��ォ��o��
	ship2.setY(GAME_HEIGHT / 4);
	ship2.setVelocity(VECTOR2(-shipNS::SPEED, -shipNS::SPEED));					//VECTOR2(X, Y)

	return;
}

//�S�ẴQ�[���A�C�e�����X�V
void SpaceWar::update()
{
	/*
	if (input->isKeyDown(SHIP_RIGHT_KEY))						//�E�Ɉړ�����ꍇ
	{
		ship1.setX(ship1.getX() + frameTime * SHIP_SPEED);
		if (ship1.getX() > GAME_WIDTH)							//��ʂ̉E�ɂ͂ݏo���ꍇ
			ship1.setX((float)-ship1.getWidth());					//��ʂ̍��ɔz�u
	}

	if (input->isKeyDown(SHIP_LEFT_KEY))						//���Ɉړ�����ꍇ
	{
		ship1.setX(ship1.getX() - frameTime * SHIP_SPEED);
		if (ship1.getX() < -ship1.getWidth())						//��ʂ̍��ɂ͂ݏo���ꍇ
			ship1.setX((float)GAME_WIDTH);						//��ʂ̉E�ɔz�u
	}

	if (input->isKeyDown(SHIP_UP_KEY))							//��Ɉړ�����ꍇ
	{
		ship1.setY(ship1.getY() - frameTime * SHIP_SPEED);
		if (ship1.getY() < -ship1.getHeight())						//��ʂ̏�ɂ͂ݏo���ꍇ
			ship1.setY((float)GAME_HEIGHT);						//��ʂ̉��ɔz�u
	}

	if (input->isKeyDown(SHIP_DOWN_KEY))						//���Ɉړ�����ꍇ
	{
		ship1.setY(ship1.getY() + frameTime * SHIP_SPEED);
		if (ship1.getY() > GAME_HEIGHT)							//��ʂ̉��ɂ͂ݏo���ꍇ
			ship1.setY((float)-ship1.getHeight());				//��ʂ̏�ɔz�u
	}
	*/

	planet.update(frameTime);
	ship1.update(frameTime);
	ship2.update(frameTime);
}

//�l�H�m�\
void SpaceWar::ai()
{}

//�Փˏ���
void SpaceWar::collisions()
{
	VECTOR2 collisionVector;
	VECTOR2 minuscollisionVector=collisionVector*-1;
	//�F���D�Ƙf���̏Փ˂̏ꍇ
	if (ship1.collidesWith(planet, collisionVector))
	{
		//�f�����璵�˕Ԃ�
		ship1.bounce(collisionVector, planet);
		ship1.damage(PLANET);
	}
	if (ship2.collidesWith(planet, collisionVector))
	{
		//�f�����璵�˕Ԃ�
		ship2.bounce(collisionVector, planet);
		ship2.damage(PLANET);
	}

	//�F���D���m�̏Փ˂̏ꍇ
	if (ship1.collidesWith(ship2, collisionVector))
	{
		//�F���D���璵�˕Ԃ�
		ship1.bounce(collisionVector, ship2);
		ship1.damage(SHIP);
		//ship2��collisionVector�̕�����ύX����
		ship2.bounce(minuscollisionVector, ship1);
		ship2.damage(SHIP);
	}
}

//�Q�[���A�C�e���������_�[
void SpaceWar::render()
{
	graphics->spriteBegin();	//�X�v���C�g�̕`����J�n
	
	nebula.draw();				//���_���V�[���ɒǉ�
	planet.draw();				//�f�����V�[���ɒǉ�
	ship1.draw();				//�F���D1���V�[���ɒǉ�
	ship2.draw();				//�F���D2���V�[���ɒǉ�

	graphics->spriteEnd();		//�X�v���C�g�̕`����I��
}

//�O���t�B�b�N�X�f�o�C�X������������
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽��
//�\�񂳂�Ă����r�f�I��������S�ĉ��
void SpaceWar::releaseAll()
{
	nebulaTexture.onLostDevice();
	gameTextures.onLostDevice();
	Game::releaseAll();
	return;
}

//�O���t�B�b�N�f�o�C�X�����Z�b�g���ꂽ�ꍇ
//�S�ẴT�[�t�F�C�X���č쐬
void SpaceWar::resetAll()
{
	gameTextures.onResetDevice();
	nebulaTexture.onResetDevice();
	Game::resetAll();
	return;
}