#ifndef DXMAKROS_H
#define DXMAKROS_H

#include <DxErr.h>

	//#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)											\
			{													\
			HRESULT hr = x;									\
			if(FAILED(hr))									\
				{												\
				DXTraceA(__FILE__, __LINE__, hr, #x, TRUE);	\
				}												\
			}
	#endif

	#define SAFERELEASECOM(x) { if(x){ x->Release();x = 0; } }

//#endif


#endif