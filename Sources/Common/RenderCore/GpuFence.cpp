#include "GpuFence.h"

#include "../Utilities/CommonMacros.h"


//---------------------------------------------------------------------------------------------
GpuFence::GpuFence( ID3D12Device* a_device ): m_fence(NULL) , m_currentValue( 0 )
{
	HRESULT hr = a_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS( &m_fence ) );

	if( FAILED( hr ) )
	{
		// TODO handle this case
	}

	m_event													=	CreateEvent( nullptr, FALSE, FALSE, nullptr );
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
GpuFence::~GpuFence()
{
    // close the fence event
    CloseHandle( m_event );

	SAFE_RELEASE( m_fence );
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
UINT64 GpuFence::GetCurrentValue()const
{
	return m_currentValue;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
UINT64 GpuFence::GetLastCompleted()
{
	m_lastCompletedValue									=	m_fence->GetCompletedValue();
	return m_lastCompletedValue;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool GpuFence::IsCompleted( UINT64 a_fenceValue )
{
	// check using stored value
	if( a_fenceValue <= m_lastCompletedValue )
	{
		return true;
	}

	// check with fence latest value
	return ( a_fenceValue <= GetLastCompleted() );
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
UINT64 GpuFence::Signal(ID3D12CommandQueue* a_queue )
{
	UINT64 t_signalValue									=	m_currentValue;

	// Signal the Current Value
	VERIFYD3D12RESULT( a_queue->Signal( m_fence, m_currentValue ) );


	// update the Last completed
	GetLastCompleted();

	// increase the current value
	m_currentValue ++;

	return t_signalValue;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GpuFence::Wait( UINT64 a_fenceValue )
{
	// if not already completed
	if( !IsCompleted( a_fenceValue) )
	{
		//todo: add lock


		// Set event when the target value has been reached
		HRESULT t_result									=	m_fence->SetEventOnCompletion( a_fenceValue , m_event );

		if( FAILED(t_result) )
		{
			// tODO
		}

		// Wait for the Event to be raised
        WaitForSingleObject( m_event , INFINITE );
	}
}
//---------------------------------------------------------------------------------------------
