#include "../includes/DirectX9.h"

DirectX9::DirectX9() {

}

DirectX9::~DirectX9() {

}

void DirectX9::init(HWND window) {

}

void DirectX9::release() {

}

void* DirectX9::getRenderDevice() const {


	return reinterpret_cast<void*>(m_pDevice);
}

LPDIRECT3DDEVICE9 DirectX9::getD3D9Device() const {
	return m_pDevice;
}

void DirectX9::setRenderState(const int state, const int value) {
	m_pDevice->SetRenderState(static_cast<D3DRENDERSTATETYPE>(state),value);
}