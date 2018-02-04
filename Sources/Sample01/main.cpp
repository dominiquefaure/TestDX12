
#include "Sample.h"



WinMainLauncher	g_launcher;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	Sample t_sample;
	return g_launcher.Run( &t_sample , hInstance , nCmdShow );

	return 0;
}

