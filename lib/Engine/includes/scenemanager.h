#ifndef KDTREE_H
#define KDTREE_H

#include "structdeclarations.h"
#include "entity.h"
#include <vector>
#include <iostream>
#include <algorithm>

class SceneManager {

};

#define MAXLEVEL 50

// 3-dim kd-tree
enum SPLIT {
    X = 0, Y, Z
};


// abstract base class for KdNodes
template <class CustomVertex>
struct KdNode {
    SPLIT plane;
    int level;
	KdNode<CustomVertex>* left;
    KdNode<CustomVertex>* right;
};

// leaf nodes store actual data
// therefore a pointer to a CustomTriangle that internally stores 3 CustomVertices from the meshes vertex buffer
template <class CustomVertex>
struct KdNodeLeaf : KdNode<CustomVertex> {
	KdNodeLeaf(SPLIT plane, int level, KdNode<CustomVertex>* left, KdNode<CustomVertex>* right, std::vector<CustomTriangle<CustomVertex>>& data) : data(data)
    {
		// initializing base class members
		KdNode<CustomVertex>::plane = plane;
		KdNode<CustomVertex>::level = level;
		// leaf nodes have no left and right pointer
		KdNode<CustomVertex>::left = NULL;
		KdNode<CustomVertex>::right = NULL;
	}

	std::vector<CustomTriangle<CustomVertex>> data;
};

// inner nodes have to store median value
template <class CustomVertex>
struct KdNodeInner : KdNode<CustomVertex> {

	// constructor for inner nodes
	KdNodeInner(SPLIT plane, int level, KdNode<CustomVertex>* left, KdNode<CustomVertex>* right, D3DXVECTOR3 data) : data(data)
    {
		// initializing base class members
		KdNode<CustomVertex>::plane = plane;
		KdNode<CustomVertex>::level = level;	
		KdNode<CustomVertex>::left = left;
		KdNode<CustomVertex>::right = right;
	}


	D3DXVECTOR3 data;         
};

template <typename CustomVertex>
class KdTree {
public:

    KdTree(std::vector<CustomTriangle<CustomVertex>>& list);
    ~KdTree();

	inline std::vector<D3DXVECTOR3>& getBoundingBoxLines() { return m_boundingBoxLines; };
	inline std::vector<KdNode<CustomVertex>*>& getKdNodes() { return m_nodeList; }

	static SPLIT s_split;

private:
 
	KdNode<CustomVertex>* root;

    std::vector<CustomTriangle<CustomVertex>> m_vec;
    std::vector<KdNode<CustomVertex>*> m_nodeList;

    int getMedianIndex(std::vector<CustomTriangle<CustomVertex>>& v);
	// median should be the point of the triangle that exactly lies on the splitting edge
    D3DXVECTOR3 getMedian(std::vector<CustomTriangle<CustomVertex>>& v);

    KdNode<CustomVertex>* insert(std::vector<CustomTriangle<CustomVertex>>& v, int level);

    void setSplittingPlane(std::vector<CustomTriangle<CustomVertex>>& v);
	
	std::vector<D3DXVECTOR3> m_boundingBoxLines;
	
	int m_currentIndex;
};

template <typename CustomVertex>
SPLIT KdTree<CustomVertex>::s_split = X;


template <typename CustomVertex>
KdTree<CustomVertex>::KdTree(std::vector<CustomTriangle<CustomVertex>>& list) : root(NULL), m_vec(list) {
    if(list.size() <= 0) {
        return;
    }

    insert(m_vec, 0);
}

template <typename CustomVertex>
KdTree<CustomVertex>::~KdTree() {

}

template <typename CustomVertex>
KdNode<CustomVertex>* KdTree<CustomVertex>::insert(std::vector<CustomTriangle<CustomVertex>>& v, int level) {
    // condition to break
    if(v.size() <= 5 || level >= MAXLEVEL) {

		KdNodeLeaf<CustomVertex>* n = new KdNodeLeaf<CustomVertex>(s_split, level, NULL, NULL, v);
		m_nodeList.push_back(n);
        return n;
    }

    setSplittingPlane(v);
    //std::sort(v.begin(), v.end(), std::less<CustomTriangle<CustomVertex3NormalUVTangentBinormal>>());

    int medianIndex = getMedianIndex(v);

    std::vector<CustomTriangle<CustomVertex>> lower(v.begin(), v.begin() + medianIndex);
    std::vector<CustomTriangle<CustomVertex>> upper(v.begin() + medianIndex, v.end());

	KdNode<CustomVertex>* n = new KdNodeInner<CustomVertex>(s_split, level, NULL, NULL, getMedian(v));

    n->left = insert(lower, level + 1);
    n->right = insert(upper, level + 1);

    m_nodeList.push_back(n);

    return n;
}

template <typename CustomVertex>
int KdTree<CustomVertex>::getMedianIndex(std::vector<CustomTriangle<CustomVertex>>& v) {
    int index = ceil(v.size() / 2);
	m_currentIndex = index;
	/*while(index % 3 != 0)
		++index;
    */
	return index;
}

template <typename CustomVertex>
D3DXVECTOR3 KdTree<CustomVertex>::getMedian(std::vector<CustomTriangle<CustomVertex>>& v) {
    CustomTriangle<CustomVertex> tri = v.at(m_currentIndex); 
	
	D3DXVECTOR3 fp1 = (tri.mP1->pos + tri.mP2->pos + tri.mP3->pos) / 3.0f;

	return fp1;
}

// TODO::
// calculate bound box about points within vector v
// then get the dimension with the maximum expansion
template <typename CustomVertex>
void KdTree<CustomVertex>::setSplittingPlane(std::vector<CustomTriangle<CustomVertex>>& v) {
	

	/*if(m_boundingBoxLines.size() == 24)
	return;*/

	// we need the vertices to get the bounding box of the current node
	std::vector<CustomVertex*> vertices;
	for(unsigned int i = 0; i < v.size(); ++i) {
		vertices.push_back(v[i].mP1);
		vertices.push_back(v[i].mP1);
		vertices.push_back(v[i].mP1);
		//vertices.at(i)->pos.x = 0;
	}

	// using lambda compare function
	// [] is called the capture specifier telling the compiler a lamba function is used
	// http://www.cprogramming.com/c++11/c++11-lambda-closures.html
	auto xExtremes = std::minmax_element(vertices.begin(),
									  vertices.end(),
									  [](const CustomVertex* lhs, const CustomVertex* rhs) {
									      return lhs->pos.x < rhs->pos.x;
									  });

	float distX = 0;
	std::vector<CustomVertex*>::iterator minIt = xExtremes.first;
	std::vector<CustomVertex*>::iterator maxIt = xExtremes.second;

	CustomVertex* minData = *minIt;
 	CustomVertex* maxData = *maxIt;
	float minx = minData->pos.x;
	float maxx = maxData->pos.x;
	distX = abs(maxData->pos.x - minData->pos.x);


	auto yExtremes = std::minmax_element(vertices.begin(),
									  vertices.end(),
									  [](const CustomVertex* lhs, const CustomVertex* rhs) {
									      return lhs->pos.y < rhs->pos.y;
									  });

	float distY = 0;
	minIt = yExtremes.first;
	maxIt = yExtremes.second;

	minData = *minIt;
	maxData = *maxIt;
	float miny = minData->pos.y;
	float maxy = maxData->pos.y;

	distY = abs(maxData->pos.y - minData->pos.y);

	auto zExtremes = std::minmax_element(vertices.begin(),
									  vertices.end(),
									  [](const CustomVertex* lhs, const CustomVertex* rhs) {
									      return lhs->pos.z < rhs->pos.z;
									  });

	float distZ = 0;
	minIt = zExtremes.first;
	maxIt = zExtremes.second;

	minData = *minIt;
	maxData = *maxIt;
	float minz = minData->pos.z;
	float maxz = maxData->pos.z;

	distZ = abs(maxz - minz);


	float maxDist = distX;

	s_split = X;

	if(distY > maxDist) {
		s_split = Y;
		maxDist = distY;
	}

	if(distZ > maxDist) {
		s_split = Z;
		maxDist = distZ;
	}

	// the bounding box is defined via 8 points that can be calcuated by permutating the min/max coords
	
	D3DXVECTOR3 lowerFrontLeft(minx, miny, minz);
	D3DXVECTOR3 lowerFrontRight(maxx, miny, minz);
	D3DXVECTOR3 upperFrontRight(maxx, maxy, minz);
	D3DXVECTOR3 upperFrontLeft(minx, maxy, minz);
	D3DXVECTOR3 lowerBackLeft(minx, miny, maxz);
	D3DXVECTOR3 lowerBackRight(maxx, miny, maxz);
	D3DXVECTOR3 upperBackRight(maxx, maxy, maxz);
	D3DXVECTOR3 upperBackLeft(minx, maxy, maxz);

	// lines will be drawed as LINELIST for now
	
	// front quad
	m_boundingBoxLines.push_back(lowerFrontLeft);
	m_boundingBoxLines.push_back(lowerFrontRight);
	m_boundingBoxLines.push_back(lowerFrontRight);
	m_boundingBoxLines.push_back(upperFrontRight);
	m_boundingBoxLines.push_back(upperFrontRight);
	m_boundingBoxLines.push_back(upperFrontLeft);
	m_boundingBoxLines.push_back(upperFrontLeft);
	m_boundingBoxLines.push_back(lowerFrontLeft);

	// backquad
	m_boundingBoxLines.push_back(lowerBackLeft);
	m_boundingBoxLines.push_back(lowerBackRight);
	m_boundingBoxLines.push_back(lowerBackRight);
	m_boundingBoxLines.push_back(upperBackRight);
	m_boundingBoxLines.push_back(upperBackRight);
	m_boundingBoxLines.push_back(upperBackLeft);
	m_boundingBoxLines.push_back(upperBackLeft);
	m_boundingBoxLines.push_back(lowerBackLeft);

	// z lines
	m_boundingBoxLines.push_back(upperFrontLeft);
	m_boundingBoxLines.push_back(upperBackLeft);
	m_boundingBoxLines.push_back(upperFrontRight);
	m_boundingBoxLines.push_back(upperBackRight);

	m_boundingBoxLines.push_back(lowerFrontLeft);
	m_boundingBoxLines.push_back(lowerBackLeft);
	m_boundingBoxLines.push_back(lowerFrontRight);
	m_boundingBoxLines.push_back(lowerBackRight);


}

// explicit specialisation for std::less template class
// this is need for std::sort
template<>  
struct std::less<CustomTriangle<CustomVertex3NormalUVTangentBinormal>>
{
	bool operator()(const CustomTriangle<CustomVertex3NormalUVTangentBinormal>& k1, const CustomTriangle<CustomVertex3NormalUVTangentBinormal>& k2) const
	{
		// searching by the highest value depending on splitting plane
		// both triangle store 3*x, 3*y and 3*z value
		// we sort by its focal point
		D3DXVECTOR3 fp1 = (k1.mP1->pos + k1.mP2->pos + k1.mP3->pos) / 3.0f;
		D3DXVECTOR3 fp2 = (k2.mP1->pos + k2.mP2->pos + k2.mP3->pos) / 3.0f;

		if(KdTree<CustomVertex3NormalUVTangentBinormal>::s_split == X) {
			return fp1.x < fp2.x;
		}
		else if(KdTree<CustomVertex3NormalUVTangentBinormal>::s_split == Y) {
			return fp1.y < fp2.y;
		}
		else {
			return fp1.z < fp2.z;
		}
	}
};

#endif

