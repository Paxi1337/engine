#ifndef __MATRIX3_H
#define __MATRIX3_H

#include "DirectX9.h"


/*
	wrapper class for D3DXMATRIX struct (at least for now)
	implement all necessary operators
	implement functions for frequently used matrix operations (static)
*/


class Matrix3
{

	friend class DirectX9;

public:
	Matrix3();
	~Matrix3();



	// implicit conversion functions
	operator D3DXMATRIX* ();
	operator const D3DXMATRIX* () const;

	// paramater? return?
	static void worldTransform();
	static void viewTransform();
	static void perspectiveProjection();
	static void orthoProjection();


private:
	const D3DXMATRIX mMatrix;
	static int test;
};

#endif