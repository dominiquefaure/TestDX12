#ifndef __COMMAND_ALLOCATOR_H__
#define __COMMAND_ALLOCATOR_H__

#include <d3d12.h>

#include "GpuFence.h"

class CommandAllocator
{
public:

	CommandAllocator();
	~CommandAllocator();

	/*
	* Init the Allocator
	*/
	bool Init( ID3D12Device* a_device , const D3D12_COMMAND_LIST_TYPE& a_type );

	/*
	* Evaluate if all the operations requiring this command allocator are done
	*/
	bool IsCompleted();

	/*
	* Reset the Allocator
	*/
	bool Reset();

	/*
	* Create a new Command List
	*/
	ID3D12GraphicsCommandList* CreateCommandList();

//private:

	// the device used by this Allocator
	ID3D12Device*			m_device;

	// the D3d12 Command Allocator
	ID3D12CommandAllocator*	m_allocator;

	// Type of Commands allowed
	D3D12_COMMAND_LIST_TYPE	m_type;

	// Pointer to the shared Fence used to know if allocators are ready
	GpuFence*				m_syncFence;

	// Fence sync value
	UINT64					m_syncValue;
};

#endif