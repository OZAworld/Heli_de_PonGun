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

//	コンストラクタ
Player::Player()
{
	camera = new Camera();
	player = new Graphic();
	model = new Model("Model/Heli.x");
	texture = new Texture("Texture/enemy2.png");
	bullet = new Bullet();

	DebugLog("プレイヤーを生成しました。\n");

	InitPlayer();
	InitBullet();
}

//	デストラクタ
Player::~Player()
{
	Release();
}

//	初期化
void Player::InitPlayer()
{
	Position = D3DXVECTOR3(0, 10, 0);
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
		bullet_Count[BULLET_MAX] = 0;
		bullet_Exist[BULLET_MAX] = false;
		bullet_Radius[BULLET_MAX] = D3DXVECTOR3(0.5, 0.5, 0.5);
		bullet_flag = false;
	}
}

//	解放処理
void Player::Release()
{
	delete camera;
	delete player;
	delete model;
	delete texture;
	delete bullet;
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
		if (camera_Rot.x < D3DXToRadian(0))
		{
			camera_Rot.x = D3DXToRadian(0);
		}
		if (camera_Rot.x > D3DXToRadian(30))
		{
			camera_Rot.x = D3DXToRadian(30);
		}

	}

	//プレイヤーをカメラの中心にとらえる
	camera_Pos.x = Position.x - 10 * sinf(cameraAngle);
	camera_Pos.z = Position.z - 10 * cosf(cameraAngle);

	camera_Rot.y += dims.lX * 0.005f;
	Rotation.y = camera_Rot.y + D3DXToRadian(0);
	oldPlayerRot.y = Rotation.y;
	oldCameraRot.x = camera_Rot.x;

}

//	ショット
void Player::Shot()
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
			bullet_Rot[i].x = oldCameraRot.y;

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
			if (bullet_Count[i] < 50)
			{
				bullet_flag = true;
				D3DXVec3Normalize(&bullet_Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 40));
				bullet_Pos[i].x += (sin(bullet_Rot[i].y) * (2.0f + i) * cos(bullet_Rot[i].x)) + bullet_Accel[i].x * 0.7f;
				bullet_Pos[i].z += (cos(bullet_Rot[i].y) * (2.0f + i) * cos(bullet_Rot[i].x)) + bullet_Accel[i].z * 0.7f;
				bullet_Pos[i].y = Position.y + bullet_Accel[i].y * 0.5f;

				bullet->Draw(&bullet_Pos[i]);
			}
			if (bullet_Count[i] > 50)
			{
				bullet_flag = false;
				bullet_Exist[i] = false;
				bullet_Count[i] = 0;
			}
		}
	}
}

//	描画
void Player::Draw()
{
	camera->View(camera_Pos, camera_Rot);
	player->DrawModelTexture(Position, Rotation, Scale, *model, *texture);
}




