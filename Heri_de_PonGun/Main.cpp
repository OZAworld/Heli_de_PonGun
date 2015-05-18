/*
*	アプリケーションのエントリーポイントとメインループ
*
*	2015/05/18		Y.Ozawa
*/

#include "Lib.h"
#include <memory>

using namespace std;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int cmdShow)
{
	//	コンソールを開く
	OpenConsole();

	//	アプリケーション実行環境を構築
	unique_ptr<Application> app(new Application("ヘリでポン！ガン！！", Rect(0, 0, 800, 600), false, hInst, cmdShow));
	unique_ptr<Light> light(new Light());

	while (app->Loop())
	{

	}

	return 0;
}


