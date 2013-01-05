#include "../includes/vertexdeclarations.h"
#include "../includes/dxmakros.h"

IDirect3DVertexDeclaration9* CustomVertex3Normal::decl = 0;
IDirect3DVertexDeclaration9* CustomVertex3NormalUV::decl = 0;
IDirect3DVertexDeclaration9* CustomVertex3NormalUVTangent::decl = 0;
IDirect3DVertexDeclaration9* CustomVertex3NormalUVTangentBinormal::decl = 0;

IDirect3DVertexDeclaration9* CustomVertex3Color::decl = 0;
IDirect3DVertexDeclaration9* CustomVertex3UV::decl = 0;

IDirect3DVertexDeclaration9* CustomVertexTransformedColor::decl = 0;

void VertexDeclarations::initVertexDeclarations(IDirect3DDevice9* dev) {
	D3DVERTEXELEMENT9 vertexElementPos3Normal3[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
            D3DDECL_END()
    };   
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3, &CustomVertex3Normal::decl));

	D3DVERTEXELEMENT9 vertexElementPos3Normal3Tex2[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
            {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
            D3DDECL_END()
    };   
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3Tex2, &CustomVertex3NormalUV::decl));

	D3DVERTEXELEMENT9 vertexElementPos3Normal3Tex2Tangent4[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
            {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0}, 
            D3DDECL_END()
    };
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3Tex2Tangent4, &CustomVertex3NormalUVTangent::decl));

	D3DVERTEXELEMENT9 vertexElementPos3Normal3Tex2Tangent3Binormal3[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
            {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
			{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
			{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
            D3DDECL_END()
    };
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3Tex2Tangent3Binormal3, &CustomVertex3NormalUVTangentBinormal::decl));

	D3DVERTEXELEMENT9 vertexElementPos3Color[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
            D3DDECL_END()
    };
	HR(dev->CreateVertexDeclaration(vertexElementPos3Color, &CustomVertex3Color::decl));

	D3DVERTEXELEMENT9 vertexElementPos3Tex2[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
            {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
            D3DDECL_END()
    };
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3Tex2Tangent4, &CustomVertex3UV::decl));

	D3DVERTEXELEMENT9 vertexElementPosTColor[] = {
            {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
            {0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
            D3DDECL_END()
    };
	HR(dev->CreateVertexDeclaration(vertexElementPos3Normal3Tex2Tangent4, &CustomVertexTransformedColor::decl));
}

void VertexDeclarations::activateVertexDeclaration(IDirect3DDevice9* dev, IDirect3DVertexDeclaration9* decl) {
	HR(dev->SetVertexDeclaration(decl));
}

void VertexDeclarations::releaseVertexDeclarations() {
	CustomVertex3Normal::decl->Release();
	CustomVertex3NormalUV::decl->Release();
	CustomVertex3NormalUVTangent::decl->Release();
	CustomVertex3NormalUVTangentBinormal::decl->Release();

	CustomVertex3Color::decl->Release();
	CustomVertex3UV::decl->Release();
	CustomVertexTransformedColor::decl->Release();

	CustomVertex3Normal::decl = 0;
	CustomVertex3NormalUV::decl = 0;
	CustomVertex3NormalUVTangent::decl = 0;
	CustomVertex3NormalUVTangentBinormal::decl = 0;

	CustomVertex3Color::decl = 0;
	CustomVertex3UV::decl = 0;
	CustomVertexTransformedColor::decl = 0;
}