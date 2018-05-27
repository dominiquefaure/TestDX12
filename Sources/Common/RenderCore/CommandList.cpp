#include "CommandList.h"
#include "CommandAllocator.h"


//---------------------------------------------------------------------------------------------
CommandList::CommandList( ID3D12GraphicsCommandList* a_list )
{
	m_commandList											=	a_list;

	// Start with the Command list in Close state
	m_commandList->Close();
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
CommandList::~CommandList()
{
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool CommandList::StartRecord( CommandAllocator* a_allocator )
{
	// save link to the allocator used by this List
	m_currentAllocator										=	a_allocator;

	a_allocator->OnStartRecord( m_commandList );

	return true;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool CommandList::FinishRecord( )
{
	HRESULT t_result										=	m_commandList->Close();

	if( FAILED( t_result ) )
	{
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------------------------
