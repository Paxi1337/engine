#include "../includes/drawable_tex2D.h"
#include "../includes/dxmakros.h"

DrawableTexture2D::DrawableTexture2D(LPDIRECT3DDEVICE9 device, UINT width, UINT height, UINT mipLevels,
		D3DFORMAT texFormat, bool useDepthBuffer,
		D3DFORMAT depthFormat, D3DVIEWPORT9& viewport,  bool autoGenMips)
: mTex(0), mRTS(0), mTopSurf(0), mWidth(width), mHeight(height), 
  mMipLevels(mipLevels), mTexFormat(texFormat), mUseDepthBuffer(useDepthBuffer),
  mDepthFormat(depthFormat), mViewPort(viewport), mAutoGenMips(autoGenMips)
{
	UINT usage = D3DUSAGE_RENDERTARGET;
	if(mAutoGenMips)
		usage |= D3DUSAGE_AUTOGENMIPMAP;

	HR(D3DXCreateTexture(device, mWidth, mHeight, mMipLevels, usage, mTexFormat, D3DPOOL_DEFAULT, &mTex));
	HR(D3DXCreateRenderToSurface(device, mWidth, mHeight, mTexFormat, mUseDepthBuffer, mDepthFormat, &mRTS));
	HR(mTex->GetSurfaceLevel(0, &mTopSurf));
}

DrawableTexture2D::~DrawableTexture2D() {
	mRTS->Release();
	mTopSurf->Release();
	mTex->Release();
}

LPDIRECT3DTEXTURE9 DrawableTexture2D::d3dTex() {
	return mTex;
}

void DrawableTexture2D::beginScene() {
	HR(mRTS->BeginScene(mTopSurf, &mViewPort));
}

void DrawableTexture2D::endScene() {
	mRTS->EndScene(D3DX_FILTER_NONE);
}

void DrawableTexture2D::dumpToFileA(LPCSTR filename) const {
	D3DXSaveSurfaceToFileA(filename, D3DXIFF_DDS, mTopSurf, NULL, NULL);
}