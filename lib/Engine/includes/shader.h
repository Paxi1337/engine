#ifndef __SHADER_H
#define __SHADER_H

#include <d3d9.h>
#include <d3dx9.h>

class Shader {
public:
	Shader() : mEffect(NULL), mDev(NULL) {}
	virtual ~Shader() {};
	virtual void loadEffectFile(const char*);
	virtual LPD3DXEFFECT getShader() { return mEffect; }
	// TODO...

protected:
	LPD3DXEFFECT mEffect;
	LPDIRECT3DDEVICE9 mDev;
};




#endif