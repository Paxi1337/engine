#ifndef __DIRECTX9_H
#define __DIRECTX9_H

// vertexdeclaraion.h includes DirectX9 header files
#include "vertexdeclarations.h"
#include "structdeclarations.h"
#include "dxmakros.h"

#include "App.h"

#include <string>
#include <map>

class DirectX9 {
	friend class Window;

public:
	DirectX9();
	explicit DirectX9(App* currentApp);
	~DirectX9();

	// releases DirectX interfaces
	void release();
	void onRelease();

	// initialized DirectX
	// TODO options for window
	void createDevice(HWND hWindow);

	void attachApp(App* runningApp);

	// TODO toggleFullscreen();

	// get pointer to IDirect3DDevice interface
	inline LPDIRECT3DDEVICE9 getD3D9Device() const { return mDevice; }
	inline D3DPRESENT_PARAMETERS* getDeviceInfo() const { return const_cast<D3DPRESENT_PARAMETERS*>(&mDevInfo); }
	inline const std::map<std::string, DWORD*>& getMSAAModes() const { return mSupportedMSAAModes; }

	// vertex buffer functions
	VertexbufferInfo* createVertexBuffer(const DWORD numberOfVertices, const DWORD FVF, std::string tag);
	// the customVertex FVF has to be exactly as defined in the associated buffer 
	void setVertexBufferData(std::string tag, void* customVertices);

	void resetDevice(D3DPRESENT_PARAMETERS*);
	
	// this function returns the raw video memory pointer given by
	// buffer->Lock(0, 0, (void**)&pVoid, 0); return pVoid;
	// u must not forget to call unlockRawVideoMemoryPtr to tell DirectX9 the memory region locked can be unlocked again
	void* getRawVideoMemoryPtr(std::string vertexBuffer);
	void unlockRawVideoMemoryPtr(std::string vertexBuffer);

	// render given vertex buffer 
	// important: the primitive type much match the type passed in when the buffer was created
	void renderVertexbuffer(D3DPRIMITIVETYPE type, std::string tag);
	// TODO
	//void renderAllVertexbuffers();

	// dispatch input device data
	void dispatchKeyMessage(WPARAM keyCode);
	void dispatchRawMouseInput(RAWINPUT const& rawMouseInput);

	// TODO: shader class
	void loadEffectFromFile(const char* effectFileName);
	inline LPD3DXEFFECT getCurrentEffect() const { return mCurrentEffect; }

	inline void setVertexDeclaration(IDirect3DVertexDeclaration9* decl) { VertexDeclarations::activateVertexDeclaration(mDevice, decl); }

private:

	// calculates the amount of primitives that have to be drawn within render method
	int calcPrimitiveCount(D3DPRIMITIVETYPE renderType, const DWORD numberOfVertices);
	
	// calculates the size of the custom vertex struct defined by the FVF
	int calcCustomStructSize(DWORD FVF);
	
	// calls the user defined onUpdate and onRender method from the current active app
	void renderFrame();

	// within the userDefinedFunction mRunningApp->onUserDefinedFuntcion is called
	// several things could be done in there for example rendering to an off-screen surface
	void userDefinedFunction();
	
	// calls the user defined onCreateDevice method from the current active app
	void onCreateDevice();

	LPDIRECT3DDEVICE9 mDevice; 
	LPDIRECT3D9 mD3D;

	std::map<std::string,VertexbufferInfo*> m_vertexBuffers;
	std::map<std::string, DWORD*> mSupportedMSAAModes;
	
	App* mRunningApp;
	//Camera* m_camera;
	
	int m_windowWidth;
	int m_windowHeight;
	
	LPD3DXEFFECT mCurrentEffect;

	D3DPRESENT_PARAMETERS mDevInfo;
};

#endif