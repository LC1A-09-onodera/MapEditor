#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class HLSLShader
{
public:
    ComPtr<ID3DBlob> vsBlob = nullptr; //頂点シェーダオブジェクト
    ComPtr<ID3DBlob> gsBlob = nullptr; //ジオメトリシェーダー
    ComPtr<ID3DBlob> psBlob = nullptr; //ピクセルシェーダオブジェクト
    ComPtr<ID3DBlob> hsBlob = nullptr; //ハルシェーダー
    ComPtr<ID3DBlob> dsBlob = nullptr; //ドメインシェーダー
};
ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName = L"", LPCSTR Vtarget = "");