#include "BaseDirectX/GameScene.h"
#include "BaseDirectX/Input.h"
#include "Sound/Sound.h"

using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xaudio2.lib")

// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	GameScene scene;
	scene.Init();

	while (true)  // �Q�[�����[�v
	{
		scene.SceneManageUpdateAndDraw();

		// DirectX���t���[�������@�����܂�
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
	// �E�B���h�E�N���X��o�^����
	WindowsAPI::Clear();

	return 0;
}
