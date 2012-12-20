#ifndef VERTEXDECLARATION_H
#define VERTEXDECLARATION_H

#include <d3d9.h>
#include <d3dx9.h>


void initVertexDeclarations();
void releaseVertexDeclarations();
void activateVertexDeclaration(IDirect3DVertexDeclaration9* decl);

struct CustomVertex3Normal {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* vertexDeclPos3Normal3;
};
#define CUSTOMVERTEX3NORMALFORMAT (D3DFVF_XYZ | D3DFVF_NORMAL)

struct CustomVertex3NormalUV {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	static IDirect3DVertexDeclaration9* vertexDeclPos3Normal3UV;
};
#define CUSTOMVERTEX3NORMALUVFORMAT (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

struct CustomVertex3NormalUVTangent {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	D3DXVECTOR4 tangent;
	static IDirect3DVertexDeclaration9* vertexDeclPos3Normal3UVTangent4;
};
// tangent has not matching FVF..
#define CUSTOMVERTEX3NORMALUVTANGENTFORMAT (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_XYZW)

struct CustomVertexTransformedColor {
	D3DXVECTOR4 pos;
	DWORD color;
	static IDirect3DVertexDeclaration9* vertexDeclPos4TColor;
};
#define CUSTOMVERTEXTRANSFORMEDCOLORFORMAT (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

struct CustomVertex3ColorUV {
	D3DXVECTOR3 xyz;
	DWORD color;
	DWORD u,v;
	static IDirect3DVertexDeclaration9* vertexDeclPos3ColorUV;
};
#define CUSTOMVERTEX3COLORUVFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct CustomVertex3Color {
	D3DXVECTOR3 mVec;
	DWORD mColor;
	static IDirect3DVertexDeclaration9* vertexDeclPos3Color;
};
#define CUSTOMVERTEX3COLORFORMAT (D3DFVF_XYZ | D3DFVF_DIFFUSE)

struct CustomVertex4NormalUV {
	D3DXVECTOR4 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	static IDirect3DVertexDeclaration9* vertexDeclPos4NormalUV;
};
#define CUSTOMVERTEX4NORMALUVFORMAT (D3DFVF_XYZW | D3DFVF_NORMAL | D3DFVF_TEX1)



#endif