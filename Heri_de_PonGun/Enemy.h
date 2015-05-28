/*
*	敵のクラス
*
*	2015/05/28		Y.Ozawa
*/

#pragma once

class Texture;
class Model;
class Graphic;
class Bullet;

//粒子のデーター
struct e_point
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 accel;
	int deathCount;
	bool exist;
};


class Enemy
{
	//-----------------------------------------------------
	//	敵関連
	//-----------------------------------------------------
	Graphic *enemy;							//	Xファイル
	D3DXVECTOR3 Position[ENEMY_MAX];		//	敵の座標
	D3DXVECTOR3 Rotation[ENEMY_MAX];		//	敵の傾き
	D3DXVECTOR3 Scale[ENEMY_MAX];			//	敵の拡大率
	D3DXVECTOR3 oldEnemyPos[ENEMY_MAX];		//	前フレームの敵の座標
	D3DXVECTOR3 oldEnemyRot[ENEMY_MAX];		//	前フレームの敵の傾き


	//-----------------------------------------------------
	//	攻撃関連
	//-----------------------------------------------------
	D3DXVECTOR3 bullet_Pos[BULLET_MAX];			//	弾の座標
	D3DXVECTOR3 bullet_Rot[BULLET_MAX];			//	弾の傾き
	D3DXVECTOR3 bullet_Accel[BULLET_MAX];		//	弾の初速
	D3DXVECTOR3 bullet_Radius[BULLET_MAX];		//	弾の半径
	int			bullet_Count[BULLET_MAX];		//	弾の生存時間
	bool		bullet_Exist[BULLET_MAX];		//	弾が生きてるかどうか
	bool		bullet_flag;					//	弾の制御フラグ

private:

	float cameraAngle;		//カメラの角度

private:

	Model *model;
	Texture *texture;
	Bullet *bullet;

public:

	//	デフォルトコンストラクタ
	Enemy();

	//	デストラクタ
	~Enemy();

	//	敵の動作
	void Move();

	//	描画する
	void Draw();

	//	初期化
	void InitEnemy();
	void InitBullet();

	//	攻撃
	void Shot();

	//	解放
	void Release();
};








