#include "BaseDirectX/GameScene.h"
#include "BaseDirectX/Input.h"
#include "Sound/Sound.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameScene scene;
	scene.Init();

	while (true)  // ゲームループ
	{
		scene.SceneManageUpdateAndDraw();

		// DirectX毎フレーム処理　ここまで
		if (Input::Key(DIK_ESCAPE))
		{
			break;
		}
		if (WindowsAPI::Qite)
		{
			break;
		}
	}
	Sound::xAudio2.Reset();
	// ウィンドウクラスを登録解除
	WindowsAPI::Clear();

	return 0;
}
