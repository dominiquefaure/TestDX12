#ifndef __COMMON_BASE_APPLICATION_H__
#define __COMMON_BASE_APPLICATION_H__

#include<Windows.h>


#include "FleedCore.h"

using namespace FleedCore;

class BaseApplication : public FleedApplication
{
public:

	BaseApplication();
	~BaseApplication();



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


private:



// Fields
private:


};

#endif