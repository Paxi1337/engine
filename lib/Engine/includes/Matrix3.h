#ifndef __RENDERENGINE_H
#define __RENDERENGINE_H

#include "DirectX9.h"

class Matrix3
{
public:
	Matrix3(void);
	~Matrix3(void);

	// implicit conversion functions
	operator D3DXMATRIX* ();
	operator const D3DXMATRIX* () const;

private:
	const D3DXMATRIX mMatrix;
};

#endif