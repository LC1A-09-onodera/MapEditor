#include "Shader.h"

ComPtr<ID3DBlob> LoadShader(LPCWSTR VshaderName, LPCSTR Vtarget)
{
    HRESULT result = S_FALSE;
    ComPtr<ID3DBlob> Blob;
    ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

    if (VshaderName != L"")
    {
        // 頂点シェーダの読み込みとコンパイル
        result = D3DCompileFromFile(VshaderName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", Vtarget, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &Blob, &errorBlob);
        if (FAILED(result)) {
            std::string errstr;
            errstr.resize(errorBlob->GetBufferSize());

            std::copy_n((char*)errorBlob->GetBufferPointer(),
                errorBlob->GetBufferSize(),
                errstr.begin());
            errstr += "\n";
            OutputDebugStringA(errstr.c_str());
            exit(1);
        }
    }
	return Blob;
}