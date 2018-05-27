#ifndef __COMMAND_LIST_DX12_H__
#define __COMMAND_LIST_DX12_H__

#include <d3d12.h>

class CommandAllocator;


class CommandList
{

	friend class CommandAllocator;

public:

	~CommandList();


	bool StartRecord( CommandAllocator* a_allocator );

	bool FinishRecord( );

// Methods
private:

	CommandList( ID3D12GraphicsCommandList* a_list );

// fields
public:
//private:

	// the Dx12 command List
	ID3D12GraphicsCommandList*	m_commandList;

	CommandAllocator*			m_currentAllocator;
};

#endif