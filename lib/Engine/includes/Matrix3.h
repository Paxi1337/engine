#ifndef __MATRIX3_H
#define __MATRIX3_H

#include "DirectX9.h"

class Matrix3
{
public:
	Matrix3();
	~Matrix3();

	// implicit conversion functions
	operator D3DXMATRIX* ();
	operator const D3DXMATRIX* () const;

private:
	const D3DXMATRIX mMatrix;
};

#endif