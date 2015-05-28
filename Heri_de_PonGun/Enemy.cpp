/*
*	敵のクラス
*
*	2015/05/28		Y.Ozawa
*/

#include "Lib.h"
#include "Enemy.h"


extern LPDIRECT3DDEVICE9 d3dDevice;


//	コンストラクタ
Enemy::Enemy()
{
	enemy = new Graphic[ENEMY_MAX];
	model = new Model("Model/alien.x");
	texture = new Texture("Texture/GreenSkin.png");
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
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		Position[i] = D3DXVECTOR3(100, 0, 0);
		Rotation[i] = D3DXVECTOR3(0, 0, 0);
		Scale[i] = D3DXVECTOR3(1, 1, 1);

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
	delete enemy;
	delete model;
	delete texture;
	delete bullet;
}

//	敵の動作
void Enemy::Move()
{

}

//	敵の攻撃
void Enemy::Shot()
{

}

//	敵の描画
void Enemy::Draw()
{

}