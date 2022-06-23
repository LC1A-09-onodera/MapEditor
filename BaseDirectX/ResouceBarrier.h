#pragma once
#include <d3d12.h>
#include <vector>

static D3D12_RESOURCE_BARRIER barrierDesc{};

class ResouceBarrier
{
public:
	static void BarrierStart(std::vector<ID3D12Resource *> backBuffers,UINT bbIndex)
	{
		barrierDesc.Transition.pResource = backBuffers[bbIndex];
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	}

	static void BarrierEnd()
	{
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	}
};