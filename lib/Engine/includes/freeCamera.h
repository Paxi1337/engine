#ifndef FREECAMERA_H
#define FREECAMERA_H

#include <d3dx9.h>

class FreeCamera
{
private:
	D3DXVECTOR3 mPosition; // camera position
	float mYaw;  // rotation around the y axis
	float mPitch; // rotation around the x axis
	float mRoll; // rotation around the z axis
	D3DXVECTOR3 mUp, mLook,mRight; // camera axis

	float restrictAngleTo360Range(float angle) const;
public:
	FreeCamera(D3DXVECTOR3 startPos);
	~FreeCamera(void);

	void calculateViewMatrix(D3DXMATRIX& viewMatrix);
	void calculateProjectionMatrix(D3DXMATRIX& projectionMatrix, float nP, float fP);
	void calculateProjectionMatrixCustom(D3DXMATRIX& projectionMatrix, float fov, float aspect, int nearPlane, int farPlane);
	
	// getter / setter
	inline float getYaw() const { return mYaw; }
	inline float getPitch() const { return mPitch; }
	inline float getRoll() const { return mRoll; }
	inline const D3DXVECTOR3 getPosition() const { return mPosition; }
	inline const D3DXVECTOR3 getLookAt() const { return mLook; }
	inline void setPosition(const D3DXVECTOR3& newPosition) { mPosition = newPosition; }
	inline void setLookAt(const D3DXVECTOR3& newLookAt) { mLook = newLookAt; }
	
	// movement
	inline void moveForward(float amount) { mPosition+=mLook*amount; }
	inline void moveRight(float amount) { mPosition+=mRight*amount; }
	inline void moveUp(float amount) { mPosition+=mUp*amount; }

	// Rotations
	void yaw(float amount); // rotate around x axis
	void pitch(float amount); // rotate around x axis
	void roll(float amount); // rotate around z axis	
};


#endif