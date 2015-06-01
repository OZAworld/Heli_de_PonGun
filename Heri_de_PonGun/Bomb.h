/*
*	爆弾のクラス
*	
*	2015/05/30		Y.Ozawa
*/

#pragma once


class Texture;
class Graphic;

//粒子のデーター
struct bomb_point
{
	D3DXVECTOR3 pos;			//	弾丸の座標
	D3DXVECTOR3 accel;			//	弾丸の初速
	int deathCount;				//	弾丸の生存時間
	bool exist;					//	弾丸が生きているかどうか
};

class Bomb
{
private:

	Texture *texture;
	Graphic *bomb;
	bomb_point *bombPoint;

public:

	//	デフォルトコンストラクタ
	Bomb();

	//	デストラクタ
	~Bomb();

	//	描画する
	void Draw(D3DXVECTOR3 Pos[], float Size, bool Alpha);

	//	初期化
	void InitBullet();

	//	解放
	void Release();

};













