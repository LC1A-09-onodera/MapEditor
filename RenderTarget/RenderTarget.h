#pragma once
#include "../Sprite/Sprite.h"

class RenderTarget
{
public:
	//マルチレンダリング行うために配列化
	static const int renderNum = 1;
	ComPtr<ID3D12Resource> texBuff[renderNum];
	static const float clearColor[4];
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	void TexInit();
	void RTVInit();
};
