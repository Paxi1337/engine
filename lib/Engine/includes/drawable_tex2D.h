#ifndef DRAWABLE_TEX2D_H
#define DRAWABLE_TEX2D_H

// includes directx9 headers
// TODO: pch!
#include "vertexdeclarations.h"

class DrawableTexture2D {
public:
	DrawableTexture2D(LPDIRECT3DDEVICE9 device, UINT width, UINT height, UINT mipLevels,
		D3DFORMAT texFormat, bool useDepthBuffer,
		D3DFORMAT depthFormat, D3DVIEWPORT9& viewport, bool autoGenMips);
	~DrawableTexture2D();

	LPDIRECT3DTEXTURE9 d3dTex();

	void dumpToFileA(LPCSTR filename) const;
	
	// begin rendering into the texture
	void beginScene();
	// end rendering into the texture
	void endScene();

private:
	// dont allow copying
	DrawableTexture2D(const DrawableTexture2D& rhs);
	DrawableTexture2D& operator=(const DrawableTexture2D& rhs);

private:
	IDirect3DTexture9*    mTex;
	ID3DXRenderToSurface* mRTS;
	IDirect3DSurface9*    mTopSurf;

	UINT         mWidth;
	UINT         mHeight;
	UINT         mMipLevels;
	D3DFORMAT    mTexFormat;
	bool         mUseDepthBuffer;
	D3DFORMAT    mDepthFormat;
	D3DVIEWPORT9 mViewPort;
	// automatically generate mip maps by gpu
	bool         mAutoGenMips;
};

#endif