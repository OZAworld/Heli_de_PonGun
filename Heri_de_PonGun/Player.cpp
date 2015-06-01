/*
*	プレイヤーのクラス
*
*	2015/05/27		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Player.h"


extern LPDIRECT3DDEVICE9 d3dDevice;
extern LPDIRECTINPUTDEVICE8 dinputDevice;
D3DXVECTOR3 inputState;					//	プレイヤーの座標を取得
D3DXVECTOR3 bulletState[BULLET_MAX];	//	プレイヤーの弾の座標を取得
D3DXVECTOR3 bombState[BOMB_MAX];		//	プレイヤーの爆弾の座標を取得
D3DXVECTOR3 razerState[RAZER_MAX];		//	プレイヤーのレーザーの座標を取得

//	コンストラクタ
Player::Player()
{
	camera = new Camera();
	player = new Graphic();

	model = new Model[2];
	model[0].LoadMesh("Model/Heli.x");
	texture = new Texture[2];
	texture[0].LoadTexture("Texture/enemy2.png");

	bullet = new Bullet();
	bomb = new Bomb();
	razer = new Razer();

	DebugLog("プレイヤーを生成しました。\n");

	InitPlayer();
	InitBullet();
	InitBomb();
	InitRazer();
}

//	デストラクタ
Player::~Player()
{
	Release();
}

//	初期化
void Player::InitPlayer()
{
	Position = D3DXVECTOR3(1000, 10, 1000);
	Rotation = D3DXVECTOR3(0, D3DXToRadian(90), 0);
	Scale = D3DXVECTOR3(0.05, 0.05, 0.05);

	camera_Pos = D3DXVECTOR3(0, 15, -10);
	camera_Rot = D3DXVECTOR3(D3DXToRadian(10), 0, 0);

	DebugLog("プレイヤーを初期化しました。\n");
}

void Player::InitBullet()
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		bullet_Count[i] = 0;
		bullet_Exist[i] = false;
		bullet_flag = false;
		bullet_death[i] = true;
	}
}

void Player::InitBomb()
{
	bomb_Exist = false;
	bomb_flag = false;
	bomb_death = true;
	bomb_Alpha = false;
	bomb_Count = 0;
	bomb_Size = 3.0f;
	for (int i = 0; i < BOMB_MAX; i++)
	{
		bombExplosion_Exist[i] = false;
		bombExplosion_Count[i] = 0;
		bombExplosion_Death[i] = true;
		bombExplosion_Size = 1.0f;
		bombExplosion_Alpha = true;
	}
}

void Player::InitRazer()
{
	for (int i = 0; i < RAZER_MAX; i++)
	{
		razer_Exist[i] = false;
		razer_flag = false;
		razer_death[i] = true;
		razer_Count[i] = 0;
	}
}

//	解放処理
void Player::Release()
{
	delete camera;
	delete player;
	delete[] model;
	delete[] texture;
	delete bullet;
	delete bomb;
	delete razer;
}

//	動作
void Player::Move()
{
	//移動する	キーボードで操作
	if (GetAsyncKeyState(0x57))
	{
		Position.x += sin(Rotation.y) * 1.0f;
		Position.z += cos(Rotation.y) * 1.0f;
	}
	if (GetAsyncKeyState(0x53))
	{
		Position.x -= sin(Rotation.y) * 1.0f;
		Position.z -= cos(Rotation.y) * 1.0f;
	}
	if (GetAsyncKeyState(0x41))
	{
		Position.x += sin(Rotation.y - D3DXToRadian(90)) * 1.0f;
		Position.z += cos(Rotation.y - D3DXToRadian(90)) * 1.0f;
	}
	if (GetAsyncKeyState(0x44))
	{
		Position.x += sin(Rotation.y + D3DXToRadian(90)) * 1.0f;
		Position.z += cos(Rotation.y + D3DXToRadian(90)) * 1.0f;
	}
	oldPlayerPos.x = Position.x;
	oldPlayerPos.z = Position.z;


	//カメラ移動		マウスの動きに合わせる
	DIMOUSESTATE2 dims;

	if (SUCCEEDED(dinputDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &dims)))
	{
		cameraAngle += dims.lX * 0.005f;
		if (cameraAngle > D3DX_PI)
		{
			cameraAngle -= 2 * D3DX_PI;
		}
		
		camera_Rot.x += dims.lY * 0.005f;
		if (camera_Rot.x < D3DXToRadian(5))
		{
			camera_Rot.x = D3DXToRadian(5);
		}
		if (camera_Rot.x > D3DXToRadian(5))
		{
			camera_Rot.x = D3DXToRadian(5);
		}

	}

	//プレイヤーをカメラの中心にとらえる
	camera_Pos.x = Position.x - 10 * sinf(cameraAngle);
	camera_Pos.z = Position.z - 10 * cosf(cameraAngle);

	camera_Rot.y += dims.lX * 0.005f;
	Rotation.y = camera_Rot.y + D3DXToRadian(0);
	oldPlayerRot.y = Rotation.y;
	oldCameraRot.x = camera_Rot.x;

	inputState = oldPlayerPos;
}

//	ショット
void Player::Shot()
{
	BulletShot();
	BombShot();
	RazerShot();
}

//	描画
void Player::Draw()
{
	camera->View(camera_Pos, camera_Rot);
	player->DrawModelTexture(Position, Rotation, Scale, model[0], texture[0]);
}


//-----------------------------------------------------------------------------------------------------------
//	以下プレイヤーの攻撃関連の動作を記述
//-----------------------------------------------------------------------------------------------------------

//	弾丸のショット
void Player::BulletShot()
{
	if (GetAsyncKeyState(VK_LBUTTON) && bullet_flag == false)
	{
		/*
		*	左クリック＆＆bullet_flagがfalseのときに初期化されるもの
		*/
		srand((unsigned int)time(NULL));

		for (int i = 0; i < BULLET_MAX; i++)
		{
			bullet_Pos[i].x = oldPlayerPos.x;
			bullet_Pos[i].z = oldPlayerPos.z;
			bullet_Rot[i].y = oldPlayerRot.y;
			bullet_Rot[i].x = oldCameraRot.x;

			bullet_Exist[i] = true;
		}
	}

	for (int i = 0; i < BULLET_MAX; i++)
	{
		/*
		*	弾の処理を記述	泡のように見せるため乱数とD3DXVec3Normalizeを使用
		*/
		if (bullet_Exist[i] == true)
		{
			bullet_Count[i]++;
			if (bullet_Count[i] < 30)
			{
				bullet_flag = true;
				bullet_death[i] = false;
				D3DXVec3Normalize(&bullet_Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
				bullet_Pos[i].x += (sin(bullet_Rot[i].y) * (2.0f + i) * cos(bullet_Rot[i].x)) + bullet_Accel[i].x * 0.7f;
				bullet_Pos[i].z += (cos(bullet_Rot[i].y) * (2.0f + i) * cos(bullet_Rot[i].x)) + bullet_Accel[i].z * 0.7f;
				bullet_Pos[i].y = Position.y + bullet_Accel[i].y * 0.5f;

				if (bullet_death[i] == false)
				{
					bullet->Draw(&bullet_Pos[i]);
				}

				Hit();
			}
			if (bullet_Count[i] > 30)
			{
				bullet_flag = false;
				bullet_death[i] = true;
				bullet_Exist[i] = false;
				bullet_Count[i] = 0;
			}
			if (bullet_death[i] == true)
			{
				bullet_Pos[i].y = 1000.0f;
			}
		}
		bulletState[i] = bullet_Pos[i];
	}

}

//	爆弾のショット
void Player::BombShot()
{
	if (GetAsyncKeyState(VK_RBUTTON) && bomb_flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	左クリック＆＆bomb_flagがfalseのときに初期化されるもの
		*/
		bomb_Pos.x = oldPlayerPos.x;
		bomb_Pos.z = oldPlayerPos.z + 0.5f;
		bomb_Pos.y = Position.y + 2.0f;
		bomb_Rot.y = oldPlayerRot.y;
		bomb_Rot.x = oldCameraRot.x;

		bomb_Exist = true;
	}
	if (bomb_Exist == true)
	{
		bomb_Count++;
		if (bomb_Count < 60)
		{
			bomb_flag = true;
			bomb_death = false;
			timeCount += 0.5f;
			bomb_Pos.y -= 1.0f + (timeCount * sin(bomb_Rot.x) - 2.0f);
			bomb_Pos.x += sin(bomb_Rot.y) * 2.0f * cos(bomb_Rot.x);
			bomb_Pos.z += cos(bomb_Rot.y) * 2.0f * cos(bomb_Rot.x);
			oldBombPos = bomb_Pos;
			bomb->Draw(&bomb_Pos, bomb_Size, bomb_Alpha);
		}
		if (bomb_Count > 60)
		{
			bomb_death = true;
		}
		for (int i = 0; i < BOMB_MAX; i++)
		{
			if (bomb_Pos.y < 0)
			{
				bombExplosion_Exist[i] = true;
			}
			if (bombExplosion_Exist[i] == true)
			{
				bombExplosion_Death[i] = false;
				bombExplosion_Count[i]++;
				bombExplosion_Pos[i] = oldBombPos;
				MinRange = D3DXVECTOR3(bomb_Pos.x, -10, bomb_Pos.z);
				MaxRange = D3DXVECTOR3(bomb_Pos.x, 10, bomb_Pos.z);
				D3DXVECTOR3 range = MaxRange - MinRange;
				bombExplosion_Pos[i].x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
				bombExplosion_Pos[i].z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
				bombExplosion_Pos[i].y = (float)((double)rand() / RAND_MAX * range.y) + MinRange.y;

				if (bombExplosion_Count[i] < 30)
				{
					D3DXVec3Normalize(&bombExplosion_Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
					bombExplosion_Pos[i].x += bombExplosion_Accel[i].x * 50.0f;
					bombExplosion_Pos[i].y += rand() % 10 * 0.0008f;
					bombExplosion_Pos[i].z += bombExplosion_Accel[i].z * 50.0f;
					bomb->Draw(&bombExplosion_Pos[i], bombExplosion_Size, bombExplosion_Alpha);		
					Hit();
				}
				if (bombExplosion_Count[i] > 30)
				{
					bombExplosion_Count[i] = 0;
					bombExplosion_Exist[i] = false;
					bomb_flag = false;
					bomb_Exist = false;
					timeCount = 0;
					bomb_Count = 0;
				}
				if (bombExplosion_Death[i] == true)
				{
					bombExplosion_Pos[i].y = 1000.0f;
				}
			}
			bombState[i] = bombExplosion_Pos[i];
		}
	}
}

//	レーザーのショット
void Player::RazerShot()
{
	if (GetAsyncKeyState(VK_SPACE) && razer_flag == false)
	{
		srand((unsigned int)time(NULL));

		/*
		*	左クリック＆＆bomb_flagがfalseのときに初期化されるもの
		*/
		for (int i = 0; i < RAZER_MAX; i++)
		{
			razer_Pos[i].x = oldPlayerPos.x;
			razer_Pos[i].z = oldPlayerPos.z + 0.5f;
			razer_Pos[i].y = Position.y + 2.0f;
			razer_Rot[i].y = oldPlayerRot.y;
			razer_Rot[i].x = oldCameraRot.x;

			razer_Exist[i] = true;
		}
	}
	for (int i = 0; i < RAZER_MAX; i++)
	{
		if (razer_Exist[i] == true)
		{
			razer_Count[i]++;
			if (razer_Count[i] < 500)
			{
				razer_flag = true;
				razer_death[i] = false;
				timeCount += 0.5f;
				D3DXVec3Normalize(&razer_Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
				razer_Pos[i].y -= 0.1f + (timeCount * sin(razer_Rot[i].x) - 2.5f) - razer_Accel[i].y * 0.5f;
				razer_Pos[i].x += sin(razer_Rot[i].y) * 2.0f * cos(razer_Rot[i].x) + razer_Accel[i].x * 1.5f;
				razer_Pos[i].z += cos(razer_Rot[i].y) * 2.0f * cos(razer_Rot[i].x) + razer_Accel[i].z * 0.5f;
				razer->Draw(&razer_Pos[i]);
				Hit();
			}
			if (razer_Pos[i].y < 0)
			{
				razer_Exist[i] = false;
				razer_death[i] = true;

				razer_Count[i] = 0;
				timeCount = 0;
			}

			if (razer_Count[i] > 500)
			{
				razer_flag = false;
				razer_death[i] = true;
				razer_Exist[i] = false;
				razer_Count[i] = 0;
				timeCount = 0;
			}
			if (razer_death[i] == true)
			{
				razer_Pos[i].y = 1000.0f;
			}
		}
		razerState[i] = razer_Pos[i];
	}
}

//	当たり判定
void Player::Hit()
{
	//	敵の座標
	extern D3DXVECTOR3 enemy_Collider[ENEMY_MAX];

	//----------------------------------------------------------------
	//	弾丸の当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BULLET_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			bullet_Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - bullet_Pos[j].x) * (enemy_Collider[i].x - bullet_Pos[j].x) +
				(enemy_Collider[i].y - bullet_Pos[j].y) * (enemy_Collider[i].y - bullet_Pos[j].y) +
				(enemy_Collider[i].z - bullet_Pos[j].z) * (enemy_Collider[i].z - bullet_Pos[j].z) <=
				(bullet_Radius[j] + enemy_Radius[i]) * (bullet_Radius[j] + enemy_Radius[i]))
			{
				bullet_death[j] = true;
			}

		}
	}

	//----------------------------------------------------------------
	//	爆弾の当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < BOMB_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			bombExplosion_Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - bombExplosion_Pos[j].x) * (enemy_Collider[i].x - bombExplosion_Pos[j].x) +
				(enemy_Collider[i].y - bombExplosion_Pos[j].y) * (enemy_Collider[i].y - bombExplosion_Pos[j].y) +
				(enemy_Collider[i].z - bombExplosion_Pos[j].z) * (enemy_Collider[i].z - bombExplosion_Pos[j].z) <=
				(bombExplosion_Radius[j] + enemy_Radius[i]) * (bombExplosion_Radius[j] + enemy_Radius[i]))
			{
				bombExplosion_Death[j] = true;
			}

		}
	}

	//----------------------------------------------------------------
	//	レーザーの当たり判定
	//----------------------------------------------------------------
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		for (int j = 0; j < RAZER_MAX; j++)
		{
			enemy_Radius[i] = 10.0f;
			razer_Radius[j] = 0.5f;

			if ((enemy_Collider[i].x - razer_Pos[j].x) * (enemy_Collider[i].x - razer_Pos[j].x) +
				(enemy_Collider[i].y - razer_Pos[j].y) * (enemy_Collider[i].y - razer_Pos[j].y) +
				(enemy_Collider[i].z - razer_Pos[j].z) * (enemy_Collider[i].z - razer_Pos[j].z) <=
				(razer_Radius[j] + enemy_Radius[i]) * (razer_Radius[j] + enemy_Radius[i]))
			{
				razer_death[j] = true;
			}

		}
	}

}





