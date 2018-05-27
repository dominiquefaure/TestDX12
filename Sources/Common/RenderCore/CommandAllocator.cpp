#include "CommandAllocator.h"
#include "../Utilities/CommonMacros.h"
#include "assert.h"

#include "CommandList.h"
#include "CommandAllocator.h"


//---------------------------------------------------------------------------------------------
CommandAllocator::CommandAllocator()
{
	m_allocator												=	nullptr;
	m_syncFence												=	nullptr;
	m_device												=	nullptr;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
CommandAllocator::~CommandAllocator()
{
	SAFE_DELETE( m_syncFence );
	SAFE_RELEASE( m_allocator );

	m_device												=	nullptr;
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
bool CommandAllocator::Init( ID3D12Device* a_device , const D3D12_COMMAND_LIST_TYPE& a_type )
{
	assert( m_allocator == nullptr);

	m_device												=	a_device;
	m_type													=	a_type;

	VERIFYD3D12RESULT(a_device ->CreateCommandAllocator(a_type, IID_PPV_ARGS( &m_allocator )));

	// Create the Fence
	m_syncFence												=	new GpuFence( a_device );

	return true;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool CommandAllocator::IsCompleted()
{
	return m_syncFence->IsCompleted( m_syncValue );
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
CommandList* CommandAllocator::CreateCommandList()
{
	ID3D12GraphicsCommandList* t_list						=	nullptr;

	VERIFYD3D12RESULT( m_device->CreateCommandList(0, m_type, m_allocator, NULL, IID_PPV_ARGS(&t_list)) );

	CommandList* t_commandList								=	new CommandList( t_list );

	return t_commandList;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool CommandAllocator::Reset()
{
	assert( IsCompleted() );

	HRESULT t_result										=	m_allocator->Reset();
	
	if( FAILED( t_result) )
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void CommandAllocator::OnStartRecord( ID3D12GraphicsCommandList* a_commandList )
{
	a_commandList->Reset( m_allocator , nullptr );
}
//---------------------------------------------------------------------------------------------
