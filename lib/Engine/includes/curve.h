#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

class Curve {

public:
	Curve();
	~Curve();

	void initCurve();

	inline const std::vector<D3DXVECTOR3>& getCurvePoints() const { return mPositionCurveNodes; }

private:
	std::vector<D3DXVECTOR3> mPositionCurveNodes;

	D3DXVECTOR3 calcIngoingTangent(D3DXVECTOR3 pointBefore, D3DXVECTOR3 pointTangent, D3DXVECTOR3 pointAfter, float tension, float continuity, float bias);
	D3DXVECTOR3 calcOutgoingTangent(D3DXVECTOR3 pointBefore, D3DXVECTOR3 pointTangent, D3DXVECTOR3 pointAfter, float tension, float continuity, float bias);
};

#endif