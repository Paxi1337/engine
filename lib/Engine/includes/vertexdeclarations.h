#ifndef VERTEXDECLARATION_H
#define VERTEXDECLARATION_H

#include <d3d9.h>
#include <d3dx9.h>

namespace VertexDeclarations {
	void initVertexDeclarations(IDirect3DDevice9* dev);
	void releaseVertexDeclarations();
	void activateVertexDeclaration(IDirect3DDevice9* dev, IDirect3DVertexDeclaration9* decl);
}

struct CustomVertex3Normal {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEX3NORMAL 1

struct CustomVertex3NormalUV {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEX3NORMALUV 2

struct CustomVertex3NormalUVTangent {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	D3DXVECTOR4 tangent;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEX3NORMALUVTANGENT 3

struct CustomVertexTransformedColor {
	D3DXVECTOR4 pos;
	DWORD color;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEXTRANSFORMEDCOLOR 4

struct CustomVertex3Color {
	D3DXVECTOR3 mVec;
	DWORD mColor;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEX3COLOR 5

struct CustomVertex3UV {
	D3DXVECTOR3 xyz;
	DWORD u,v;
	static IDirect3DVertexDeclaration9* decl;
};
#define CUSTOMVERTEX3UV 6

#endif