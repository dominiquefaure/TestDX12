#ifndef __GPU_FENCE_H__
#define __GPU_FENCE_H__

#include <d3d12.h>

class GpuFence
{

public:

	GpuFence( ID3D12Device* a_device );
	~GpuFence();


	/*
	* Get the Current Fence Value
	*/
	UINT64 GetCurrentValue()const;

	/*
	* Get the Last Completed Fence Value
	*/
	UINT64 GetLastCompleted();

	/*
	* evaluate if the Fence reach the Target Value
	*/
	bool IsCompleted( UINT64 a_fenceValue );


	/*
	* Signal this Fence, automatically increase the current value
	*
	* @return the Signaled Fence Value
	*/
	UINT64 Signal(ID3D12CommandQueue* a_queue );

	/*
	* Wait the Fence reach the Target Value
	*/
	void Wait( UINT64 a_fenceValue );

private:

	ID3D12Fence*	m_fence;

	// Current Fence value
	UINT64			m_currentValue;

	// store the last completed value to avoid exesive call to the fence GetCompletedValue
	UINT64			m_lastCompletedValue;

	// event used for Sync if required
	HANDLE			m_event;
};

#endif