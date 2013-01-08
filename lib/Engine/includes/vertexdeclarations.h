#ifndef VERTEXDECLARATION_H
#define VERTEXDECLARATION_H

#include <d3d9.h>
#include <d3dx9.h>

namespace VertexDeclarations {
	void initVertexDeclarations(IDirect3DDevice9* dev);
	void releaseVertexDeclarations();
	void activateVertexDeclaration(IDirect3DDevice9* dev, IDirect3DVertexDeclaration9* decl);
}

#define CUSTOMVERTEX3NORMAL 1
struct CustomVertex3Normal {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3NORMAL;
};

#define CUSTOMVERTEX3NORMALUV 2
struct CustomVertex3NormalUV {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3NORMALUV;
};

#define CUSTOMVERTEX3NORMALUVTANGENT 3
struct CustomVertex3NormalUVTangent {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	D3DXVECTOR3 tangent;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3NORMALUVTANGENT;
};

#define CUSTOMVERTEX3NORMALUVTANGENTBINORMAL 4
struct CustomVertex3NormalUVTangentBinormal {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	FLOAT u,v;
	D3DXVECTOR3 tangent;
	D3DXVECTOR3 binormal;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3NORMALUVTANGENTBINORMAL;
};

#define CUSTOMVERTEXTRANSFORMEDCOLOR 5
struct CustomVertexTransformedColor {
	D3DXVECTOR4 pos;
	DWORD color;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEXTRANSFORMEDCOLOR;
};

#define CUSTOMVERTEX3COLOR 6
struct CustomVertex3Color {
	D3DXVECTOR3 mVec;
	DWORD mColor;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3COLOR;
};

#define CUSTOMVERTEX3UV 7
struct CustomVertex3UV {
	D3DXVECTOR3 xyz;
	DWORD u,v;
	static IDirect3DVertexDeclaration9* decl;
	static const int format = CUSTOMVERTEX3UV;
};


#endif