/*
*	“G‚ÌƒNƒ‰ƒX
*
*	2015/05/28		Y.Ozawa
*/

#include <time.h>
#include "Lib.h"
#include "Enemy.h"


extern LPDIRECT3DDEVICE9 d3dDevice;


//	ƒRƒ“ƒXƒgƒ‰ƒNƒ^
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/tako.x");
	texture = new Texture("Texture/enemy2.png");
	bullet = new Bullet();

	DebugLog("“G‚ğ¶¬‚µ‚Ü‚µ‚½B\n");

	InitEnemy();
	InitBullet();
}

//	ƒfƒXƒgƒ‰ƒNƒ^
Enemy::~Enemy()
{
	Release();
}

//	“G‚Ì‰Šú‰»
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

		DebugLog("“G‚ğ‰Šú‰»‚µ‚Ü‚µ‚½B\n");
	}
}

//	“G‚Ì’e‚Ì‰Šú‰»
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

//	‰ğ•úˆ—
void Enemy::Release()
{
	delete[] enemy;
	delete model;
	delete texture;
	delete bullet;
}

//	“G‚Ì“®ì
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

//	“G‚ÌUŒ‚
void Enemy::Shot()
{

}

//	“G‚Ì•`‰æ
void Enemy::Draw()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		enemy[i].DrawModelTexture(Position[i], Rotation[i], Scale[i], *model, *texture);
	}
}