#ifndef __GAME_APPLICATION_H__
#define __GAME_APPLICATION_H__

#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_4.h>

#include "WindowsAppConfig.h"


// number of buffers we want, 2 for double buffering, 3 for tripple buffering
#define FRAME_BUFFER_COUNT	3



class WinMainLauncher;

class GameApplication
{
	friend class WinMainLauncher;

public:

	GameApplication();
	virtual ~GameApplication();

	/*
	* Initialize the Game
	*/
	void Init( HWND a_handle );

	/*
	 * Called every ticks by the main loop perform all the frame operations
 	 */
	void OnFrame();

// pure Virtual methods to be overrided
protected:

	/*
	 * Called at Application initialization, allow to start loading custom data
	 */
	virtual void OnInit(){}

	/*
	 * Called at Application initialization, allow to start loading custom data
	 */
	virtual void OnClose(){}

	/*
	 * Called every ticks to perform the update operations
	 */
	virtual void OnUpdate(){}

	/*
	 *  called every ticks to perform the custom draw operations
	 */
	virtual void OnDraw(){}

	/*
	 * Allow custom config of the Windows creation
	 */
	virtual void OnSetWindowConfig( WinAppConfig& a_config ){}

// Methodss
private:

	/*
	 * Configure the Windows Dialog
	 */
	void SetWindowConfig( WinAppConfig& a_config );

	/*
	 * Perform all the Frame Updates
	 */
	void PerformUpdates();

	/*
	 * PErform all the Draw Operations
	 *
	 */
	void PerformDraw();


protected:

	bool InitD3D( HWND a_handle );

	void WaitForPreviousFrame();
	void CloseHandle( HANDLE a_handle );

	void UpdatePipeline();


	ID3D12Device* device; // direct3d device

	IDXGISwapChain3* swapChain; // swapchain used to switch between render targets

	ID3D12CommandQueue* commandQueue; // container for command lists

	ID3D12DescriptorHeap* rtvDescriptorHeap; // a descriptor heap to hold resources like the render targets

	ID3D12Resource* renderTargets[ FRAME_BUFFER_COUNT ]; // number of render targets equal to buffer count

	ID3D12CommandAllocator* commandAllocator[ FRAME_BUFFER_COUNT ]; // we want enough allocators for each buffer * number of threads (we only have one thread)

	ID3D12GraphicsCommandList* commandList; // a command list we can record commands into, then execute them to render the frame

	ID3D12Fence* fence[ FRAME_BUFFER_COUNT ];    // an object that is locked while our command list is being executed by the gpu. We need as many 
                                         //as we have allocators (more if we want to know when the gpu is finished with an asset)

	HANDLE fenceEvent; // a handle to an event when our fence is unlocked by the gpu

	UINT64 fenceValue[ FRAME_BUFFER_COUNT ]; // this value is incremented each frame. each fence will have its own value

	int frameIndex; // current rtv we are on

	int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)


	bool	m_exitRequested;
};

#endif