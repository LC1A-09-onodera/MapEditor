#pragma once
#include <Windows.h>
#include <vector>
#include <string>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

#pragma warning(push)
#pragma warning(disable:26812)
#include <DirectXTex.h>
#pragma warning(pop)

#include "../BaseDirectX/Input.h"
#include "../WindowsAPI/WinAPI.h"
#include <wrl.h>
#include <d3dx12.h>
#include "../BaseDirectX/BaseDirectX.h"


using namespace Microsoft::WRL;
using namespace DirectX;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Tex
{
public:
    ComPtr<ID3D12Resource> texbuff;
    int texNum = 0;
    void LoadGraph(const wchar_t *graph);
    int Get();
};

class TexManager
{
public:
    static const int TexMax = 512;
    static int texNum;
};

