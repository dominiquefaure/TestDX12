#ifndef __COMMON_MACROS_H__
#define __COMMON_MACROS_H__


#ifdef _DEBUG
#define FORCE_INLINE
#else
#define FORCE_INLINE	__forceinline
#endif

#define ALIGN(x) __declspec(align(n))

#define SAFE_DELETE( X ) if( X != NULL )\
{\
	delete X;\
	X = NULL;\
}
#define SAFE_DELETE_ARRAY( X ) if( X != NULL )\
{\
	delete[] X;\
	X = NULL;\
}

#define SAFE_RELEASE( X ) if( X != NULL )\
{\
	X->Release();\
	X = NULL;\
}


#define VERIFYD3D12RESULT(x)	{\
									HRESULT hr = x;\
									if (FAILED(hr))\
									 abort();\
								}


#endif