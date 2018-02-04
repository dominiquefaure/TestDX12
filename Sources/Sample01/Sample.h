#pragma once

#include<Windows.h>


#include "FleedCore.h"

using namespace FleedCore;

class Sample : public FleedApplication
{
public:

	Sample();
	~Sample();



// pure Virtual methods to be overrided
protected:

	/*
	 * Called at Application initialization, allow to start loading custom data
	 */
	virtual void OnInit();

	/*
	 * Called at Application initialization, allow to start loading custom data
	 */
	virtual void OnClose();

	/*
	 * Called every ticks to perform the update operations
	 */
	virtual void OnUpdate();

	/*
	 *  called every ticks to perform the custom draw operations
	 */
	virtual void OnDraw();

	
	/*
	 * Configure the Windows Dialog
	 */
	virtual void OnSetWindowConfig( WinAppConfig& a_config );

private:



// Fields
private:


};