#include "spaceWar.h"

//�R���X�g���N�^
SpaceWar::SpaceWar()
{
	menuOn = true;
	countDownOn = false;
	roundOver = false;
}

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

	//DirectX�t�H���g��������
	fontBig.initialize(graphics, spacewarNS::FONT_BIG_SIZE, false, false, spacewarNS::FONT);
	fontBig.setFontColor(spacewarNS::FONT_COLOR);

	//���j���[�̃e�N�X�`��
	if (!menuTexture.initialize(graphics, MENU_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu texture"));
	//����̃e�N�X�`��
	if (!spaceTexture.initialize(graphics, SPACE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	//�Q�[���̃e�N�X�`��
	if (!textures1.initialize(graphics, TEXTURES1_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game texture"));

	//���j���[�̉摜
	if (!menu.initialize(graphics, 0, 0, 0, &menuTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing menu"));

	//����̉摜
	if (!space.initialize(graphics, 0, 0, 0, &spaceTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
	space.setScale((float)SPACE_SCALE);

	//���̉摜�Q
	for (int i = 0; i < 4; i++)
	{
		if (!moons[i].initialize(graphics, MOON_SIZE, MOON_SIZE, 4, &textures1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing moons"));
		moons[i].setFrames(MOON1_FRAME + i, MOON1_FRAME + i);
		moons[i].setCurrentFrame(MOON1_FRAME + i);
		//���͒����ŊJ�n
		moons[i].setX(GAME_WIDTH / 2 - MOON_SIZE / 2);
		moons[i].setY(GAME_HEIGHT / 2 - MOON_SIZE / 2);
	}

	//�f��
	if (!planet.initialize(graphics, PLANET_SIZE, PLANET_SIZE, 2, &textures1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));
	planet.setFrames(PLANET_FRAME, PLANET_FRAME);
	planet.setCurrentFrame(PLANET_FRAME);
	//�f���͒����ŊJ�n
	planet.setX(GAME_WIDTH / 2 - PLANET_SIZE / 2);
	planet.setY(GAME_HEIGHT / 2 - PLANET_SIZE / 2);

	//�F���D1
	if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &textures1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
	ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);		//�A�j���[�V�����̃t���[��
	ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);							//�J�n�t���[��
	ship1.setColorFilter(SETCOLOR_ARGB(255, 230, 230, 255));
	ship1.setMass(shipNS::MASS);
	ship1.setX(GAME_WIDTH / 2 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);
}

//�V�������E���h���J�n����
void SpaceWar::roundStart()
{
	space.setX(0);
	space.setY(0);

	//�F���D�𒆉��ŊJ�n������
	ship1.setX(GAME_WIDTH / 2 - shipNS::WIDTH);
	ship1.setY(GAME_HEIGHT / 2 - shipNS::HEIGHT);

	ship1.setDegrees(0);
	countDownTimer = spacewarNS::COUNT_DOWN;
	countDownOn = true;
	roundOver = false;
}

//�S�ẴQ�[���A�C�e�����X�V
void SpaceWar::update()
{
	if (menuOn)
	{
		if (input->anyKeyPressed())
		{
			menuOn = false;
			input->clearAll();
			roundStart();
		}
	}
	else if (countDownOn)
	{
		countDownTimer -= frameTime;
		if (countDownTimer <= 0)
			countDownOn = false;
	}
	else
	{
		if (ship1.getActive())
		{
			if (input->isKeyDown(SHIP1_FORWARD_KEY) || input->getGamepadDPadUp(0))	//�G���W�����I���̎�
			{
				ship1.setEngineOn(true);
				audio->playCue(ENGINE1);
			}
			else
			{
				ship1.setEngineOn(false);
				audio->stopCue(ENGINE1);
			}
			ship1.rotate(shipNS::NONE);
			if (input->isKeyDown(SHIP1_LEFT_KEY) || input->getGamepadDPadLeft(0))	//ship1�����ɉ�]�����ꍇ
				ship1.rotate(shipNS::LEFT);
			if (input->isKeyDown(SHIP1_RIGHT_KEY) || input->getGamepadDPadLeft(0))	//ship1���E�ɉ�]�����ꍇ
				ship1.rotate(shipNS::RIGHT);
		}
		if (roundOver)
		{
			roundTimer -= frameTime;
			if (roundTimer <= 0)
				roundStart();
		}
	}
	//�p�����b�N�X�X�N���[���p�ɃG���e�B�e�B�̈ʒu���X�V
	ship1.update(frameTime);
	//�f����X�����ɓ�����
	planet.setX(planet.getX() - frameTime * ship1.getVelocity().x);
	//�f����Y�����ɓ�����
	planet.setY(planet.getY() - frameTime * ship1.getVelocity().y);
	planet.update(frameTime);

	//�q���𓮂���
	for (int i = 0; i < 4; i++)
	{
		//�X�N���[���̑������q�����Ƃ�20%�x������
		moons[i].setX(moons[i].getX() - frameTime * ship1.getVelocity().x*0.2f*(4 - i));
		moons[i].setY(moons[i].getY() - frameTime * ship1.getVelocity().y*0.2f*(4 - i));
	}

	//������F���D�Ɣ��΂�X�����Ɉړ�
	space.setX(space.getX() - frameTime * ship1.getVelocity().x);
	//������F���D�Ɣ��΂�Y�����Ɉړ�
	space.setY(space.getY() - frameTime * ship1.getVelocity().y);

	//����摜��[�ŌJ��Ԃ�
	if (space.getX() > 0)
		//����摜��SPACE_WIDTH���������ɓ�����
		space.setX(space.getX() - SPACE_WIDTH);
	//����摜����ʂ���O��č��ɂ���ꍇ
	if (space.getX() < -SPACE_WIDTH)
		//����摜��SPACE_WIDTH�������E�ɓ�����
		space.setX(space.getX() + SPACE_WIDTH);
	//����̏�[>��ʂ̏�[
	if (space.getY() > 0)
		space.setY(space.getY() - SPACE_HEIGHT);
	//����摜����ʂ���O��ď�ɂ���ꍇ
	if (space.getY() < -SPACE_HEIGHT)
		space.setY(space.getY() + SPACE_HEIGHT);
}

//�l�H�m�\
void SpaceWar::ai()
{}

//�Փˏ���
void SpaceWar::collisions()
{

}

//�Q�[���A�C�e���������_�[
void SpaceWar::render()
{
	float x = space.getX();
	float y = space.getY();

	graphics->spriteBegin();	//�X�v���C�g�̕`����J�n
	
	
	//����摜��[�ŌJ��Ԃ�
	space.draw();				//���݂̈ʒu�ŕ`��

	//����摜�̉E�[��������ꍇ
	if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
	{
		space.setX(space.getX() + SPACE_WIDTH);		//���[�ɖ߂�
		space.draw();								//�ĕ`��
	}
	//����摜�̉��[��������ꍇ
	if (space.getY() < -SPACE_HEIGHT + (int)GAME_HEIGHT)
	{
		space.setY(space.getY() + SPACE_HEIGHT);	//��[�ɖ߂�
		space.draw();								//�ĕ`��
		space.setX(x);								//x�ʒu�𕜌�
		//����摜�̉E�[��������ꍇ�A���[�ɖ߂�
		if (space.getX() < -SPACE_WIDTH + (int)GAME_WIDTH)
			space.draw();							//�ĕ`��
	}
	space.setX(x);									//x�ʒu�𕜌�
	space.setY(y);									//y�ʒu�𕜌�

	for (int i = 3; i >= 0; i--)
		moons[i].draw();							//�������O�ɉq���̕`��
	planet.draw();									//�f���̕`��

	ship1.draw();									//�F���D��`��

	if (menuOn)
		menu.draw();
	if (countDownOn)
	{
		_snprintf_s(buffer, spacewarNS::BUF_SIZE, "%d", (int)(ceil(countDownTimer)));
		fontBig.print(buffer, spacewarNS::COUNT_DOWN_X, spacewarNS::COUNT_DOWN_Y);
	}

	graphics->spriteEnd();		//�X�v���C�g�̕`����I��
}

//�O���t�B�b�N�X�f�o�C�X������������
//�O���t�B�b�N�X�f�o�C�X�����Z�b�g�\�ɂ��邽��
//�\�񂳂�Ă����r�f�I��������S�ĉ��
void SpaceWar::releaseAll()
{
	menuTexture.onLostDevice();
	spaceTexture.onLostDevice();
	textures1.onLostDevice();
	fontScore.onLostDevice();
	fontBig.onLostDevice();
	Game::releaseAll();
	return;
}

//�O���t�B�b�N�f�o�C�X�����Z�b�g���ꂽ�ꍇ
//�S�ẴT�[�t�F�C�X���č쐬
void SpaceWar::resetAll()
{
	fontBig.onResetDevice();
	fontScore.onResetDevice();
	textures1.onResetDevice();
	spaceTexture.onResetDevice();
	menuTexture.onResetDevice();
	Game::resetAll();
	return;
}