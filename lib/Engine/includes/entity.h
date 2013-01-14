#ifndef ENTITY_H
#define ENTITY_H

#include "../includes/structdeclarations.h"

template <class CustomVertex>
struct Entity {
	Entity(unsigned int ID, ID3DXMesh* mesh, std::vector<Material>* mtrls, std::vector<IDirect3DTexture9*>* texs);
	Entity(unsigned int ID, ID3DXMesh* mesh);
	~Entity();

	CustomTriangle<CustomVertex>* getTriangles();
	inline DWORD getTriangleCount() const { return mTriangleCount; }


	unsigned int mID;
	ID3DXMesh* mMesh;
	DWORD mTriangleCount;
	CustomTriangle<CustomVertex>* mTriangles;
	std::vector<Material>* mMtrls;
	std::vector<IDirect3DTexture9*>* mTexs;
};


template <class CustomVertex>
Entity<CustomVertex>::Entity(unsigned int ID,
			   ID3DXMesh* mesh,
			   std::vector<Material>* mtrls,
			   std::vector<IDirect3DTexture9*>* texs) : mID(ID),
														mTriangleCount(0),
														mMesh(mesh),
														mMtrls(mtrls),
														mTexs(texs),
														mTriangles(0)
																													
{
	mTriangleCount = mMesh->GetNumFaces();
}

template <class CustomVertex>
Entity<CustomVertex>::Entity(unsigned int ID,
			   ID3DXMesh* mesh) : mID(ID),
														mTriangleCount(0),
														mMesh(mesh),
														mMtrls(0),
														mTexs(0),
														mTriangles(0)
																													
{
	mTriangleCount = mMesh->GetNumFaces();
}

template <class CustomVertex>
Entity<CustomVertex>::~Entity() {
	mMesh->Release();
	
	if(mTexs != 0) {
		for(std::vector<IDirect3DTexture9*>::iterator it = mTexs->begin(); it != mTexs->end(); ++it) {
			IDirect3DTexture9* mat = *it;
			mat->Release();
		}
		delete mTexs;
	}

	if(mMtrls != 0)
		delete mMtrls;
	

	if(mTriangles)
		delete mTriangles;
}

template <class CustomVertex>
CustomTriangle<CustomVertex>* Entity<CustomVertex>::getTriangles() {

	if(!mTriangles) {
		
		mTriangles = new CustomTriangle<CustomVertex>[mTriangleCount];

		UINT16* indices;
		CustomVertex* vertices;

		mMesh->LockIndexBuffer(D3DLOCK_READONLY, reinterpret_cast<void**>(&indices));
		mMesh->LockVertexBuffer(D3DLOCK_READONLY, reinterpret_cast<void**>(&vertices));
	
	
		for(unsigned int i = 0; i < mMesh->GetNumFaces(); ++i) {
			
			mTriangles[i].mP1 = &vertices[indices[i*3]];
			mTriangles[i].mP2 = &vertices[indices[i*3+1]];
			mTriangles[i].mP3 = &vertices[indices[i*3+2]];

		}
	
		mMesh->UnlockIndexBuffer();
		mMesh->UnlockVertexBuffer();
	}

	return mTriangles;
}

#endif