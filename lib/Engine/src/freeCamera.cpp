#include "../includes/FreeCamera.h"

FreeCamera::FreeCamera(D3DXVECTOR3 startPos) : mPosition(startPos),mYaw(0),mPitch(0),mRoll(0) {
	mUp=D3DXVECTOR3(0.0f,1.0f,0.0f);
	mLook=D3DXVECTOR3(0.0f,0.0f,1.0f);
	mRight=D3DXVECTOR3(1.0f,0.0f,0.0f);
}

FreeCamera::~FreeCamera(void) {
}

void FreeCamera::calculateViewMatrix(D3DXMATRIX& viewMatrix) {
	/* Start with our camera axis pointing down z
	 An alternative method is to just keep adjusting our axis but if we do that the
	 axis can start to lose its orthogonal shape (due to floating point innacuracies).
	 This could be solved by rebuilding the orthogonal shape each time with the following:
	 1. normalising the look vector
	 2. creating the up vector from the cross product of the look and the right
	 3. normalising up
	 4. creating the right vector from the cross product of the look and the up
	 5. normalising right
	*/
	mUp=D3DXVECTOR3(0.0f,1.0f,0.0f);
	mLook=D3DXVECTOR3(0.0f,0.0f,1.0f);
	mRight=D3DXVECTOR3(1.0f,0.0f,0.0f);

	// Yaw is rotation around the y axis (m_up)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX yawMatrix;
	D3DXMatrixRotationAxis(&yawMatrix, &mUp, mYaw);
	// To apply yaw we rotate the m_look & m_right vectors about the m_up vector (using our yaw matrix)
	D3DXVec3TransformCoord(&mLook, &mLook, &yawMatrix); 
	D3DXVec3TransformCoord(&mRight, &mRight, &yawMatrix); 

	// Pitch is rotation around the x axis (m_right)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX pitchMatrix;
	D3DXMatrixRotationAxis(&pitchMatrix, &mRight, mPitch);
	// To apply pitch we rotate the m_look and m_up vectors about the m_right vector (using our pitch matrix)
	D3DXVec3TransformCoord(&mLook, &mLook, &pitchMatrix); 
	D3DXVec3TransformCoord(&mUp, &mUp, &pitchMatrix); 
		
	// Roll is rotation around the z axis (m_look)
	// Create a matrix that can carry out this rotation
	D3DXMATRIX rollMatrix;
	D3DXMatrixRotationAxis(&rollMatrix, &mLook, mRoll);
	// To apply roll we rotate up and right about the look vector (using our roll matrix)
	// Note: roll only really applies for things like aircraft unless you are implementing lean
	D3DXVec3TransformCoord(&mRight, &mRight, &rollMatrix); 
	D3DXVec3TransformCoord(&mUp, &mUp, &rollMatrix); 
	
	// Build the view matrix from the transformed camera axis
	D3DXMatrixIdentity(&viewMatrix);

	viewMatrix._11 = mRight.x; viewMatrix._12 = mUp.x; viewMatrix._13 = mLook.x;
	viewMatrix._21 = mRight.y; viewMatrix._22 = mUp.y; viewMatrix._23 = mLook.y;
	viewMatrix._31 = mRight.z; viewMatrix._32 = mUp.z; viewMatrix._33 = mLook.z;
	
	viewMatrix._41 = - D3DXVec3Dot(&mPosition, &mRight); 
	viewMatrix._42 = - D3DXVec3Dot(&mPosition, &mUp);
	viewMatrix._43 = - D3DXVec3Dot(&mPosition, &mLook);
}

void FreeCamera::calculateProjectionMatrix(D3DXMATRIX& projectionMatrix) {
	float aspect = 4.0f / 3.0f;
	D3DXMatrixPerspectiveFovLH( &projectionMatrix, D3DX_PI/4, aspect, 1.0f, 200.0f );
}

void FreeCamera::calculateProjectionMatrixCustom(D3DXMATRIX& projectionMatrix, float fov, float aspect, int nearPlane, int farPlane) {
	D3DXMatrixPerspectiveFovLH( &projectionMatrix, fov, aspect, nearPlane, farPlane );
}

// Yaw - rotation around y axis
void FreeCamera::yaw(float amount) {
	mYaw+=amount;
	mYaw=restrictAngleTo360Range(mYaw);
}	

// Pitch - rotation around x axis
void FreeCamera::pitch(float amount) {
	mPitch+=amount;
	mPitch=restrictAngleTo360Range(mPitch);
}

// Roll - rotation around z axis
// Note: normally only used for aircraft type cameras rather than land based ones
void FreeCamera::roll(float amount) {
	mRoll+=amount;
	mRoll=restrictAngleTo360Range(mRoll);
}

// Keep the angle in the range 0 to 360 (2*PI)
float FreeCamera::restrictAngleTo360Range(float angle) const {
	while(angle>2*D3DX_PI)
		angle-=2*D3DX_PI;

	while(angle<0)
		angle+=2*D3DX_PI;

	return angle;
}