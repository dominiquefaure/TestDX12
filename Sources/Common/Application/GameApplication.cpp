#include "GameApplication.h"

#include "../Utilities/d3dx12.h"

//---------------------------------------------------------------------------------------------
GameApplication::GameApplication()
{
	m_exitRequested											=	false;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
GameApplication::~GameApplication()
{
	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::SetWindowConfig( WinAppConfig& a_config )
{
	// Set Default values
	a_config.m_className									=	L"VTSRenderSampleClass";
	a_config.m_captionText									=	L"DefaultApplication";
	a_config.m_dwStyle										=	WS_OVERLAPPEDWINDOW;
	a_config.m_dwXPos										=	CW_USEDEFAULT;
	a_config.m_dwYPos										=	0;
	a_config.m_dwWidth										=	CW_USEDEFAULT;
	a_config.m_dwHeight										=	0;


	a_config.m_smallIconResID								=	0;
	a_config.m_iconResID									=	0;

	// Allow custom configuration
	OnSetWindowConfig( a_config );
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::Init( HWND a_handle )
{
/*
	RECT rc;
	GetClientRect(a_handle, &rc);

	RhiManager::CreateInstance();
	RhiManager::GetInstance()->Init( a_handle ,  rc.right - rc.left , rc.bottom - rc.top  );

	InputManager::CreateInstance();
*/
    // initialize direct3d
    if (!InitD3D( a_handle ))
    {
        MessageBox(0, L"Failed to initialize direct3d 12",
            L"Error", MB_OK);
//        Cleanup();
        return ;
    }

	// perform custom inits
	OnInit( );
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
void GameApplication::OnFrame()
{
	PerformUpdates();
	PerformDraw();
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::PerformUpdates()
{
//	InputManager::GetInstance()->Update();

	// Custom Updates	
	OnUpdate();
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::PerformDraw()
{
//	RhiManager::GetInstance()->BeginFrame();
    // we want to wait for the gpu to finish executing the command list before we start releasing everything
    WaitForPreviousFrame();


	UpdatePipeline();

	OnDraw();

	HRESULT hr;

    // create an array of command lists (only one command list here)
    ID3D12CommandList* ppCommandLists[] = { m_commandList->m_commandList };
	
    // execute the array of command lists
    commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	
	fenceValue[frameIndex]									=	m_frameSyncFence->Signal( commandQueue );

	// present the current backbuffer
    hr = swapChain->Present(0, 0);
    if (FAILED(hr))
    {
        m_exitRequested										=	true;
    }

//	RhiManager::GetInstance()->EndFrame();
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
bool GameApplication::InitD3D( HWND a_handle )
{
	HRESULT hr;

   // -- Create the Device -- //

    IDXGIFactory4* dxgiFactory;
    hr														=	CreateDXGIFactory1( IID_PPV_ARGS(&dxgiFactory) );
    if (FAILED(hr))
    {
        return false;
    }

    IDXGIAdapter1* adapter; // adapters are the graphics card (this includes the embedded graphics on the motherboard)

    int adapterIndex = 0; // we'll start looking for directx 12  compatible graphics devices starting at index 0

    bool adapterFound = false; // set this to true when a good one was found

   // find first hardware gpu that supports d3d 12
    while (dxgiFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            // we dont want a software device
                adapterIndex++; // add this line here. Its not currently in the downloadable project
            continue;
        }

        // we want a device that is compatible with direct3d 12 (feature level 11 or higher)
        hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr);
        if (SUCCEEDED(hr))
        {
            adapterFound = true;
            break;
        }

        adapterIndex++;
    }

    if (!adapterFound)
    {
        return false;
    }
    // Create the device
    hr = D3D12CreateDevice( adapter,
							D3D_FEATURE_LEVEL_11_0,
							IID_PPV_ARGS(&device)
							);
    if (FAILED(hr))
    {
        return false;
    }


    // -- Create the Command Queue -- //

    D3D12_COMMAND_QUEUE_DESC cqDesc = {}; // we will be using all the default values

    hr = device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue)); // create the command queue
    if (FAILED(hr))
    {
        return false;
    }

	int Width	=	1024;
	int Height	=	720;
	int frameBufferCount	=	3;
	bool FullScreen	=	false;
    // -- Create the Swap Chain (double/tripple buffering) -- //
    
    DXGI_MODE_DESC backBufferDesc = {}; // this is to describe our display mode
    backBufferDesc.Width = Width; // buffer width
    backBufferDesc.Height = Height; // buffer height
    backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // format of the buffer (rgba 32 bits, 8 bits for each chanel)
    
    // describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1; // multisample count (no multisampling, so we just put 1, since we still need 1 sample)
    
    // Describe and create the swap chain.
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = frameBufferCount; // number of buffers we have
    swapChainDesc.BufferDesc = backBufferDesc; // our back buffer description
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
    swapChainDesc.OutputWindow = a_handle; // handle to our window
    swapChainDesc.SampleDesc = sampleDesc; // our multi-sampling description
    swapChainDesc.Windowed = !FullScreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps
    
    IDXGISwapChain* tempSwapChain;
    
    dxgiFactory->CreateSwapChain(
        commandQueue, // the queue will be flushed once the swap chain is created
        &swapChainDesc, // give it the swap chain description we created above
        &tempSwapChain // store the created swap chain in a temp IDXGISwapChain interface
        );
    
    swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);
    
    frameIndex = swapChain->GetCurrentBackBufferIndex(); 



    // -- Create the Back Buffers (render target views) Descriptor Heap -- //

    // describe an rtv descriptor heap and create
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = frameBufferCount; // number of descriptors for this heap.
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

    // This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
    // otherwise we would set the heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    hr = device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap));
    if (FAILED(hr))
    {
        return false;
    }

    // get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
    // descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
    // device to give us the size. we will use this size to increment a descriptor handle offset
    rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
    // but we cannot literally use it like a c++ pointer.
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

    // Create a RTV for each buffer (double buffering is two buffers, tripple buffering is 3).
    for (int i = 0; i < frameBufferCount; i++)
    {
        // first we get the n'th buffer in the swap chain and store it in the n'th
        // position of our ID3D12Resource array
        hr = swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
        if (FAILED(hr))
        {
            return false;
        }

        // the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
        device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);

        // we increment the rtv handle by the rtv descriptor size we got above
        rtvHandle.Offset(1, rtvDescriptorSize);
    }


	// -- Create the Command Allocators -- //

	for (int i = 0; i < frameBufferCount; i++)
	{
		m_commandAllocators[i]	=	new CommandAllocator();
		if( ! m_commandAllocators[ i ]->Init( device , D3D12_COMMAND_LIST_TYPE_DIRECT ) )
		{
			return false;
		}
	}

    // create the command list with the first allocator
	m_commandList	=	m_commandAllocators[0]->CreateCommandList();
    if( m_commandList == NULL )
    {
        return false;
    }

    // Create the Fence used to sync between each frames
	m_frameSyncFence										=	new GpuFence( device );

	return true;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::WaitForPreviousFrame()
{
    HRESULT hr;

    // swap the current rtv buffer index so we draw on the correct buffer
    frameIndex = swapChain->GetCurrentBackBufferIndex();

	// wait the Sync Fence reach the value
	m_frameSyncFence->Wait( fenceValue[frameIndex] );

}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void GameApplication::CloseHandle( HANDLE a_handle )
{
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
void GameApplication::UpdatePipeline()
{
	HRESULT hr;

    // we can only reset an allocator once the gpu is done with it
    // resetting an allocator frees the memory that the command list was stored in
	if( !m_commandAllocators[ frameIndex]->Reset() )
	{
        m_exitRequested										=	true;
	}


	if(! m_commandList->StartRecord( m_commandAllocators[ frameIndex] ) )
	{
        m_exitRequested										=	true;
	}

	// here we start recording commands into the commandList (which all the commands will be stored in the commandAllocator)

    // transition the "frameIndex" render target from the present state to the render target state so the command list draws to it starting from here
    m_commandList->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // here we again get the handle to our current render target view so we can set it as the render target in the output merger stage of the pipeline
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

    // set the render target for the output merger stage (the output of the pipeline)
    m_commandList->m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Clear the render target by using the ClearRenderTargetView command
    const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    
    // transition the "frameIndex" render target from the render target state to the present state. If the debug layer is enabled, you will receive a
    // warning if present is called on the render target when it's not in the present state
    m_commandList->m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	if( ! m_commandList->FinishRecord() )
    {
        m_exitRequested										=	true;
    }
}
//---------------------------------------------------------------------------------------------
