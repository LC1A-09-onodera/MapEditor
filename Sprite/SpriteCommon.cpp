#include "SpriteCommon.h"

//グラフィックスパイプライン
D3D12_GRAPHICS_PIPELINE_STATE_DESC SpriteCommon::gpipeline;
//スプライト用デスクリプタヒープ
ComPtr<ID3D12DescriptorHeap> SpriteCommon::basicDescHeap;
D3D12_INPUT_ELEMENT_DESC SpriteCommon::inputLayout[2];
ComPtr<ID3D12PipelineState> SpriteCommon::spritePipelineState;
XMMATRIX SpriteCommon::matProjection{};
ComPtr<ID3D12DescriptorHeap> SpriteCommon::DescHeap;
ComPtr<ID3D12Resource> SpriteCommon::TexBuff[SRVCount];


SpriteCommon::SpriteCommon()
{
	
}

void SpriteCommon::Init(ComPtr<ID3DBlob> vsBlob, ComPtr<ID3DBlob> psBlob)
{
    SpriteCommon::inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    SpriteCommon::inputLayout[1] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
    gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
    gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
    gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
    gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;//背面カリングをしない
    gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
    gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
    blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    //共通設定------------------------------------------
    blenddesc.BlendEnable = true;
    blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
    blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;
    //加算合成------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_ONE;
    blenddesc.DestBlend = D3D12_BLEND_ONE;*/
    //減算合成------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
    blenddesc.SrcBlend = D3D12_BLEND_ONE;
    blenddesc.DestBlend = D3D12_BLEND_ONE;*/
    //色の反転------------------------------------------
    /*blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
    blenddesc.DestBlend = D3D12_BLEND_ZERO;*/
    //半透明合成
    blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
    blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
    blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    //頂点レイアウトの設定
    gpipeline.InputLayout.pInputElementDescs = inputLayout;
    gpipeline.InputLayout.NumElements = _countof(inputLayout);
    //図形を三角形に
    gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    gpipeline.NumRenderTargets = 1; // 描画対象は1つ
    gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
    gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング
    /*descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = ConstBufferNum + 1;*/
    //BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
    //射影行列
    matProjection = XMMatrixOrthographicOffCenterLH(0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descHeapDesc.NumDescriptors = SRVCount;
    BaseDirectX::result = BaseDirectX::dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&DescHeap));

}

HRESULT SpriteCommon::LoadSprtieGraph(UINT texNumber, const wchar_t *filename)
{
    //画像データ
    TexMetadata metadata{};
    ScratchImage scratchImg{};
    BaseDirectX::result = LoadFromWICFile(filename, WIC_FLAGS_NONE, &metadata, scratchImg);
    return S_OK;
}
