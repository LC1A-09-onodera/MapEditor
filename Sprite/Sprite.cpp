#include "Sprite.h"


SpriteCommon Sprite::common;

void Sprite::CreateSprite(const wchar_t* graph, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff, bool back, bool TexSize)
{
    tex.LoadGraph(graph);
    texNum = tex.Get();
    this->back = back;
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //頂点シェーダーの読み込みピクセルシェーダーの読み込み頂点レイアウトの読み込み
    // 頂点シェーダの読み込みとコンパイル
    BaseDirectX::result = D3DCompileFromFile(L"Resource/HLSL/spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    if (FAILED(BaseDirectX::result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
        errstr += "\n";

        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // ピクセルシェーダの読み込みとコンパイル
    BaseDirectX::result = D3DCompileFromFile( L"Resource/HLSL/spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    if (FAILED(BaseDirectX::result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char *)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    int sizevb = sizeof(Spritevertices);

    //共通データの初期化
    common.Init(vsBlob,psBlob);

    this->position.m128_f32[0] = position.x;
    this->position.m128_f32[1] = position.y;
    this->position.m128_f32[2] = position.z;

    //デスクリプタレンジ
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
    rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
    rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &BaseDirectX::samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    BaseDirectX::result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    BaseDirectX::result = BaseDirectX::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootSignature));
    common.gpipeline.pRootSignature = spriteRootSignature.Get();

    BaseDirectX::result = BaseDirectX::dev->CreateGraphicsPipelineState(&common.gpipeline, IID_PPV_ARGS(&common.spritePipelineState));

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = common.SRVCount;
    BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&common.DescHeap));
    
    enum{LB,LT,RB,RT};

    if (TexSize == true)
    {
        D3D12_RESOURCE_DESC resDesc;
        if (texBuff == nullptr)
        {
            resDesc = tex.texbuff->GetDesc();
        }
        else
        {
            resDesc = texBuff->GetDesc();
        }

        float wid = (float)resDesc.Width;
        float hei = (float)resDesc.Height;

        Spritevertices[LB].pos = {0.0f, hei, 0.0f};
        Spritevertices[LT].pos = {0.0f, 0.0f,0.0f};
        Spritevertices[RB].pos = {wid, hei, 0.0f};
        Spritevertices[RT].pos = {wid, 0.0f,0.0f};
    }

    //頂点マップ
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizevb), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv *vertMap = nullptr;
    BaseDirectX::result = vertBuff->Map(0, nullptr, (void **)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));
    
    //定数マップ
    ConstBufferDataSP *constMap = nullptr;
    BaseDirectX::result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::CreateSprite(Tex tex, XMFLOAT3 position, ComPtr<ID3D12Resource> texBuff, bool back, bool TexSize)
{
    texNum = tex.Get();
    this->back = back;
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //頂点シェーダーの読み込みピクセルシェーダーの読み込み頂点レイアウトの読み込み
    // 頂点シェーダの読み込みとコンパイル
    BaseDirectX::result = D3DCompileFromFile(L"Resource/HLSL/spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    if (FAILED(BaseDirectX::result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
        errstr += "\n";

        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    // ピクセルシェーダの読み込みとコンパイル
    BaseDirectX::result = D3DCompileFromFile(L"Resource/HLSL/spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    if (FAILED(BaseDirectX::result)) {
        // errorBlobからエラー内容をstring型にコピー
        std::string errstr;
        errstr.resize(errorBlob->GetBufferSize());

        std::copy_n((char*)errorBlob->GetBufferPointer(),
            errorBlob->GetBufferSize(),
            errstr.begin());
        errstr += "\n";
        // エラー内容を出力ウィンドウに表示
        OutputDebugStringA(errstr.c_str());
        exit(1);
    }

    int sizevb = sizeof(Spritevertices);

    //共通データの初期化
    common.Init(vsBlob, psBlob);

    this->position.m128_f32[0] = position.x;
    this->position.m128_f32[1] = position.y;
    this->position.m128_f32[2] = position.z;

    //デスクリプタレンジ
    descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    rootParams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);//定数バッファビューとして初期化
    rootParams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
    rootSignatureDesc.Init_1_0(_countof(rootParams), rootParams, 1, &BaseDirectX::samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    BaseDirectX::result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
    BaseDirectX::result = BaseDirectX::dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootSignature));
    common.gpipeline.pRootSignature = spriteRootSignature.Get();

    BaseDirectX::result = BaseDirectX::dev->CreateGraphicsPipelineState(&common.gpipeline, IID_PPV_ARGS(&common.spritePipelineState));

    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = common.SRVCount;
    BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&common.DescHeap));

    enum { LB, LT, RB, RT };

    if (TexSize == true)
    {
        D3D12_RESOURCE_DESC resDesc;
        if (texBuff == nullptr)
        {
            resDesc = tex.texbuff->GetDesc();
        }
        else
        {
            resDesc = texBuff->GetDesc();
        }

        float wid = (float)resDesc.Width;
        float hei = (float)resDesc.Height;

        Spritevertices[LB].pos = { 0.0f, hei, 0.0f };
        Spritevertices[LT].pos = { 0.0f, 0.0f,0.0f };
        Spritevertices[RB].pos = { wid, hei, 0.0f };
        Spritevertices[RT].pos = { wid, 0.0f,0.0f };
    }

    //頂点マップ
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizevb), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv* vertMap = nullptr;
    BaseDirectX::result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));

    //定数マップ
    ConstBufferDataSP* constMap = nullptr;
    BaseDirectX::result = constBuff->Map(0, nullptr, (void**)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::SpritePipelineEdit()
{
    //パイプラインステート設定
    BaseDirectX::cmdList->SetPipelineState(common.spritePipelineState.Get());
    //ルートシグネチャ
    BaseDirectX::cmdList->SetGraphicsRootSignature(spriteRootSignature.Get());
    //プリミティブ形状
    BaseDirectX::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::SpriteDraw(/* D3D12_GPU_DESCRIPTOR_HANDLE *gpuDescHandleSRV*/)
{
    Move();
    SpritePipelineEdit();
    //デスクリプタヒープの配列
    ID3D12DescriptorHeap *ppHeap[] = {BaseDirectX::basicDescHeap.Get()};
    BaseDirectX::cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
    //頂点バッファのセット
    BaseDirectX::cmdList->IASetVertexBuffers(0, 1, &vbView);
    //定数バッファ
    BaseDirectX::cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
    //シェーダーリソースビュー
    BaseDirectX::cmdList->SetGraphicsRootDescriptorTable(1, BaseDirectX::gpuDescHandleSRV[texNum]);
    //描画コマンド
    BaseDirectX::cmdList->DrawInstanced(4,1,0,0);

    if (back == true)
    {
        BaseDirectX::BackGroundFront();
    }
}

void Sprite::Move()
{
    //ワールド行列更新
    matWorld = XMMatrixIdentity();
    matWorld *= XMMatrixRotationZ(rotation);
    matWorld *= XMMatrixTranslationFromVector(position);
    //転送
    ConstBufferDataSP *constMap = nullptr;
    BaseDirectX::result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->mat = matWorld * common.matProjection;
    constBuff->Unmap(0, nullptr);
}

void Sprite::ChangeSize(float wid, float hei)
{
    VertexPosUv Spritevertices[] = {
        {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
        {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
        {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
        {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //頂点シェーダーの読み込みピクセルシェーダーの読み込み頂点レイアウトの読み込み
    // 頂点シェーダの読み込みとコンパイル
    //BaseDirectX::result = D3DCompileFromFile(L"spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    //if (FAILED(BaseDirectX::result)) {
    //    // errorBlobからエラー内容をstring型にコピー
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char *)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
    //    errstr += "\n";

    //    // エラー内容を出力ウィンドウに表示
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    //// ピクセルシェーダの読み込みとコンパイル
    //BaseDirectX::result = D3DCompileFromFile( L"spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    //if (FAILED(BaseDirectX::result)) {
    //    // errorBlobからエラー内容をstring型にコピー
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char *)errorBlob->GetBufferPointer(),
    //        errorBlob->GetBufferSize(),
    //        errstr.begin());
    //    errstr += "\n";
    //    // エラー内容を出力ウィンドウに表示
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    int sizevb = sizeof(Spritevertices);

    enum{LB,LT,RB,RT};

    Spritevertices[LB].pos = {0.0f, hei, 0.0f};
    Spritevertices[LT].pos = {0.0f, 0.0f,0.0f};
    Spritevertices[RB].pos = {wid, hei, 0.0f};
    Spritevertices[RT].pos = {wid, 0.0f,0.0f};
    

    //頂点マップ
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizevb), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv *vertMap = nullptr;
    BaseDirectX::result = vertBuff->Map(0, nullptr, (void **)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));
    
    //定数マップ
    ConstBufferDataSP *constMap = nullptr;
    BaseDirectX::result = constBuff->Map(0, nullptr, (void **)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}

void Sprite::ChangeSizeOther(float wid, float hei)
{
    VertexPosUv Spritevertices[] = {
           {{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},
           {{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},
           {{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},
           {{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},
    };
    //頂点シェーダーの読み込みピクセルシェーダーの読み込み頂点レイアウトの読み込み
    // 頂点シェーダの読み込みとコンパイル
    //BaseDirectX::result = D3DCompileFromFile(L"spriteVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);

    //if (FAILED(BaseDirectX::result)) {
    //    // errorBlobからエラー内容をstring型にコピー
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char *)errorBlob->GetBufferPointer(), errorBlob->GetBufferSize(), errstr.begin());
    //    errstr += "\n";

    //    // エラー内容を出力ウィンドウに表示
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    //// ピクセルシェーダの読み込みとコンパイル
    //BaseDirectX::result = D3DCompileFromFile( L"spritePixelShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);

    //if (FAILED(BaseDirectX::result)) {
    //    // errorBlobからエラー内容をstring型にコピー
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char *)errorBlob->GetBufferPointer(),
    //        errorBlob->GetBufferSize(),
    //        errstr.begin());
    //    errstr += "\n";
    //    // エラー内容を出力ウィンドウに表示
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    int sizevb = sizeof(Spritevertices);

    enum { LB, LT, RB, RT };

    Spritevertices[LB].pos = { -(wid / 2), hei / 2, 0.0f };
    Spritevertices[LT].pos = { -(wid / 2), -(hei / 2),0.0f };
    Spritevertices[RB].pos = { wid / 2, hei / 2, 0.0f };
    Spritevertices[RT].pos = { wid / 2, -(hei / 2),0.0f };


    //頂点マップ
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizevb), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
    VertexPosUv* vertMap = nullptr;
    BaseDirectX::result = vertBuff->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, Spritevertices, sizeof(Spritevertices));
    vertBuff->Unmap(0, nullptr);
    vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
    vbView.SizeInBytes = sizeof(Spritevertices);
    vbView.StrideInBytes = sizeof(Spritevertices[0]);
    BaseDirectX::result = BaseDirectX::dev->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuff));

    //定数マップ
    ConstBufferDataSP* constMap = nullptr;
    BaseDirectX::result = constBuff->Map(0, nullptr, (void**)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1);
    constMap->mat = XMMatrixOrthographicOffCenterLH(0.0f, static_cast<float>(window_width), static_cast<float>(window_height), 0.0f, 0.0f, 1.0f);
    constBuff->Unmap(0, nullptr);
}
