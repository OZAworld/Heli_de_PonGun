/*
*	弾丸のクラス
*
*	2015/05/27		Y.Ozawa
*/

#pragma once

class Texture;
class Graphic;

//粒子のデーター
struct bullet_point
{
	D3DXVECTOR3 pos;			//	弾丸の座標
	D3DXVECTOR3 accel;			//	弾丸の初速
	int deathCount;				//	弾丸の生存時間
	bool exist;					//	弾丸が生きているかどうか
};

class Bullet
{	
private:

	Texture *texture;
	Graphic *bullet;
	bullet_point *bulletPoint;

public:

	//	デフォルトコンストラクタ
	Bullet();

	//	デストラクタ
	~Bullet();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[]);

	//	初期化
	void InitBullet();

	//	解放
	void Release();

};