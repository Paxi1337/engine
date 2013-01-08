#ifndef STRUCTDECLARATION_H
#define STRUCTDECLARATION_H

#include <d3d9.h>
#include <d3dx9.h>


#include <vector>

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);

typedef struct VertexbufferInfo {
	LPDIRECT3DVERTEXBUFFER9 buffer;
	DWORD FVF;
	DWORD vertexCount;
} *PVertexbufferInfo;

// TODO add constructors and init lists for light structs
typedef struct SpotLight {
	D3DXVECTOR3 posW;
	D3DXVECTOR3 dirW;  
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	float spotPower;
} *PSpotlight;

typedef struct DirectionalLight {
	D3DXVECTOR3 dirW;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
} *PDirectionalLight;

typedef struct PointLight {
	D3DXVECTOR3 posW;
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
} *PPointLight;

typedef struct Material {
	Material() : ambient(WHITE), 
				 diffuse(WHITE), 
				 spec(WHITE), 
				 specPower(8.0f) 
	{}

	Material(const D3DXCOLOR& a, const D3DXCOLOR& d, const D3DXCOLOR& s, float power) : ambient(a), 
																				        diffuse(d), 
																						spec(s), 
																						specPower(power)
	{}

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR spec;
	float specPower;
} *PMaterial;



typedef struct Entity {
	Entity(unsigned int ID, ID3DXMesh* mesh, std::vector<Material>* mtrls, std::vector<IDirect3DTexture9*>* texs) : mID(ID),
																													mMesh(mesh),
																													mMtrls(mtrls),
																													mTexs(texs)
																													
	{}

	~Entity() {
		mMesh->Release();
		
		for(std::vector<IDirect3DTexture9*>::iterator it = mTexs->begin(); it != mTexs->end(); ++it) {
			IDirect3DTexture9* mat = *it;
			mat->Release();
		}
		
		delete mMtrls;
		delete mTexs;
	}

	unsigned int mID;
	ID3DXMesh* mMesh;
	std::vector<Material>* mMtrls;
	std::vector<IDirect3DTexture9*>* mTexs;

} *PEntity;


#endif