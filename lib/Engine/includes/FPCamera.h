#pragma once

#include <d3dx9.h>

/*	A first person camera implementation
	See acompanying notes
	Keith Ditchburn May 2007
*/
class CFPCamera
{
private:
	D3DXVECTOR3 m_position; // camera position
	float m_yaw;  // rotation around the y axis
	float m_pitch; // rotation around the x axis
	float m_roll; // rotation around the z axis
	D3DXVECTOR3 m_up,m_look,m_right; // camera axis

	float restrictAngleTo360Range(float angle) const;
public:
	CFPCamera(D3DXVECTOR3 startPos);
	~CFPCamera(void);

	void calculateViewMatrix(D3DXMATRIX& viewMatrix);
	void calculateProjectionMatrix(D3DXMATRIX& projectionMatrix);
	// Gets
	inline float getYaw() const { return m_yaw; }
	inline float getPitch() const { return m_pitch; }
	inline float getRoll() const { return m_roll; }
	D3DXVECTOR3 getPosition() const { return m_position; }	
	inline void setPosition(const D3DXVECTOR3& newPosition) { m_position = newPosition; }

	// Move operations
	inline void moveForward(float amount) { m_position+=m_look*amount; }
	inline void moveRight(float amount) { m_position+=m_right*amount; }
	inline void moveUp(float amount) { m_position+=m_up*amount; }

	// Rotations
	void yaw(float amount); // rotate around x axis
	void pitch(float amount); // rotate around x axis
	void roll(float amount); // rotate around z axis	
};
