#include "../includes/curve.h"

Curve::Curve() {

}

Curve::~Curve() {

}

void Curve::initCurve() {
	D3DXVECTOR3 segmentTangent1(1.0f,0.0f,0.0f);
	D3DXVECTOR3 segmentTangent2(1.0f,0.0f,0.0f);
	D3DXVECTOR3 newpos;
	float interpolationValue = 0.001f;
	int countControlPoints = 10;

	D3DXVECTOR3 curvePoints[] = {
		D3DXVECTOR3(-5.0f,3.0f,5.0f)*15,
		D3DXVECTOR3(-8.0f,3.0f,4.0f)*15,
		D3DXVECTOR3(-5.0f,3.0f,0.0f)*15,
		D3DXVECTOR3(-6.0f,3.0f,-6.0f)*15,
		D3DXVECTOR3(0.0f,3.0f,-5.0f)*15,
		D3DXVECTOR3(3.0f,3.0f,-4.0f)*15,
		D3DXVECTOR3(4.0f,3.0f,-3.0f)*15,
		D3DXVECTOR3(5.0f,3.0f,0.0f)*15,
		D3DXVECTOR3(7.0f,3.0f,3.0f)*15,
		D3DXVECTOR3(-5.0f,3.0f,5.0f)*15
	};

	float tensions[] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	float biases[] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	float continuities[] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

	for(float i=0;i<1.0f;i = i + interpolationValue) {
		D3DXVec3Hermite(&newpos,&curvePoints[0], &segmentTangent1, &curvePoints[1], &(calcIngoingTangent(curvePoints[0],curvePoints[1],curvePoints[2],tensions[0], biases[0], continuities[0])),i);
		mPositionCurveNodes.push_back(newpos);
	}

	for(int count=1;count<countControlPoints-2;count++) {
		for(float i=0;i<1.0f;i = i + interpolationValue) {
			D3DXVec3Hermite(&newpos, &curvePoints[count], &(calcOutgoingTangent(curvePoints[count-1],curvePoints[count],curvePoints[count+1],tensions[count-1], biases[count-1], continuities[count-1])), &curvePoints[count+1], &(calcIngoingTangent(curvePoints[count],curvePoints[count+1],curvePoints[count+2],tensions[count], biases[count], continuities[count])), i);
			mPositionCurveNodes.push_back(newpos);
		}
	}

	for(float i=0;i<1.0f;i = i + interpolationValue) {
		D3DXVec3Hermite(&newpos,&curvePoints[countControlPoints-2],&(calcOutgoingTangent(curvePoints[countControlPoints-3],curvePoints[countControlPoints-2],curvePoints[countControlPoints-1],tensions[countControlPoints-3], biases[countControlPoints-3], continuities[countControlPoints-3])),&curvePoints[countControlPoints-1],&segmentTangent2,i);
		mPositionCurveNodes.push_back(newpos);
	}
}

D3DXVECTOR3 Curve::calcIngoingTangent(D3DXVECTOR3 pointBefore, D3DXVECTOR3 pointTangent, D3DXVECTOR3 pointAfter, float tension, float continuity, float bias) {
	D3DXVECTOR3 ingoingTangent = (((1.0f-tension)*(1.0f+bias)*(1.0f-continuity)/2)*(pointTangent-pointBefore))+(((1.0f-tension)*(1.0f-bias)*(1.0f+continuity)/2)*(pointAfter-pointTangent));
	return ingoingTangent;
}

D3DXVECTOR3 Curve::calcOutgoingTangent(D3DXVECTOR3 pointBefore, D3DXVECTOR3 pointTangent, D3DXVECTOR3 pointAfter, float tension, float continuity, float bias) {
	D3DXVECTOR3 outgoingTangent = (((1.0f-tension)*(1.0f+bias)*(1.0f+continuity)/2)*(pointTangent-pointBefore))+(((1.0f-tension)*(1.0f-bias)*(1.0f-continuity)/2)*(pointAfter-pointTangent));
	return outgoingTangent;
}