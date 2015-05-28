/*
*	敵のクラス
*
*	2015/05/28		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Enemy.h"


extern LPDIRECT3DDEVICE9 d3dDevice;


//	コンストラクタ
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/tako.x");
	texture = new Texture("Texture/enemy2.png");
	bullet = new Bullet();

	DebugLog("敵を生成しました。\n");

	InitEnemy();
	InitBullet();
}

//	デストラクタ
Enemy::~Enemy()
{
	Release();
}

//	敵の初期化
void Enemy::InitEnemy()
{
	MinRange = D3DXVECTOR3(-250, 0, -250);
	MaxRange = D3DXVECTOR3(250, 10, 250);
	D3DXVECTOR3 range = MaxRange - MinRange;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Position[i].x = (float)((double)rand() / RAND_MAX * range.x) + MinRange.x;
		Position[i].y = 10.0f;
		Position[i].z = (float)((double)rand() / RAND_MAX * range.z) + MinRange.z;
		Rotation[i] = D3DXVECTOR3(0, 0, 0);
		Scale[i] = D3DXVECTOR3(10, 10, 10);

		DebugLog("敵を初期化しました。\n");
	}
}

//	敵の弾の初期化
void Enemy::InitBullet()
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
void Enemy::Release()
{
	delete[] enemy;
	delete model;
	delete texture;
	delete bullet;
}

//	敵の動作
void Enemy::Move()
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		D3DXVec3Normalize(&Accel[i], &D3DXVECTOR3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50));
		Position[i].x += Accel[i].x * 0.08f;
		Position[i].z += Accel[i].z * 0.08f;
		Scale[i] = D3DXVECTOR3(10, 10, 10);
	}
}

//	敵の攻撃
void Enemy::Shot()
{

}

//	敵の描画
void Enemy::Draw()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].DrawModelTexture(Position[i], Rotation[i], Scale[i], *model, *texture);
	}
}