#ifndef KDTREE_H
#define KDTREE_H

#include "structdeclarations.h"
#include "entity.h"
#include <vector>
#include <iostream>
#include <algorithm>

class SceneManager {

};

#define MAXLEVEL 10

// 3-dim kd-tree
enum SPLIT {
    X = 0, Y, Z
};

// explicit specialisation for std::less template class
// this is need for std::sort
template<>  
struct std::less<CustomTriangle<CustomVertex3NormalUVTangentBinormal>>
{
    bool operator()(const CustomTriangle<CustomVertex3NormalUVTangentBinormal>& k1, const CustomTriangle<CustomVertex3NormalUVTangentBinormal>& k2) const
    {
		// searching by the highes value depending on splitting plane
		// both triangle store 3*x, 3*y and 3*z value
		// we sort by its focal point
		D3DXVECTOR3 fp1 = (k1.mP1->pos + k1.mP2->pos + k1.mP3->pos) / 3.0f;
		D3DXVECTOR3 fp2 = (k2.mP1->pos + k2.mP2->pos + k2.mP3->pos) / 3.0f;

		return fp1 < fp2;
    }
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
	KdNodeLeaf(SPLIT plane, int level, KdNode<CustomVertex>* left, KdNode<CustomVertex>* right, CustomTriangle<CustomVertex>* data) : data(data)
    {
		// initializing base class members
		KdNode<CustomVertex>::plane = plane;
		KdNode<CustomVertex>::level = level;
		// leaf nodes have no left and right pointer
		KdNode<CustomVertex>::left = NULL;
		KdNode<CustomVertex>::right = NULL;
	}

	CustomTriangle<CustomVertex>* data;
};

// inner nodes have to store median value
// not sure about data type yet currenty assuming int
template <class CustomVertex>
struct KdNodeInner : KdNode<CustomVertex> {

	// constructor for inner nodes
	KdNodeInner(SPLIT plane, int level, KdNode<CustomVertex>* left, KdNode<CustomVertex>* right, int data) : data(data)
    {
		// initializing base class members
		KdNode<CustomVertex>::plane = plane;
		KdNode<CustomVertex>::level = level;	
		KdNode<CustomVertex>::left = left;
		KdNode<CustomVertex>::right = right;
	}


	int data;         
};

template <typename CustomVertex>
class KdTree {
public:

    KdTree(std::vector<CustomTriangle<CustomVertex>>& list);
    ~KdTree();
private:
 
	KdNode<CustomVertex>* root;

    std::vector<CustomTriangle<CustomVertex>> m_vec;
    std::vector<KdNode<CustomVertex>*> m_nodeList;

    int getMedianIndex(std::vector<CustomTriangle<CustomVertex>>& v);
    int getMedian(std::vector<CustomTriangle<CustomVertex>>& v);

    KdNode<CustomVertex>* insert(std::vector<CustomTriangle<CustomVertex>>& v, int level);

    void setSplittingPlane(std::vector<CustomTriangle<CustomVertex>>& v);
};


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
    if(v.size() == 1 || level >= MAXLEVEL) {

        return 0;
    }

    setSplittingPlane(v);
    std::sort(v.begin(), v.end(), std::less<CustomTriangle<CustomVertex3NormalUVTangentBinormal>>());

    int medianIndex = getMedianIndex(v);

    std::vector<CustomTriangle<CustomVertex>> lower(v.begin(), v.begin() + medianIndex);
    std::vector<CustomTriangle<CustomVertex>> upper(v.begin() + medianIndex, v.end());

	KdNode<CustomVertex>* n = new KdNodeInner<CustomVertex>(Compare<CustomVertex>::getSplittingPlane(), level, NULL, NULL, getMedian(v));

    n->left = insert(lower, level + 1);
    n->right = insert(upper, level + 1);

    m_nodeList.push_back(n);

    return n;
}

template <typename CustomVertex>
int KdTree<CustomVertex>::getMedianIndex(std::vector<CustomTriangle<CustomVertex>>& v) {
    double index = floor(v.size() / 2);
    
	return (int)index;
}

template <typename CustomVertex>
int KdTree<CustomVertex>::getMedian(std::vector<CustomTriangle<CustomVertex>>& v) {
    

	return 1;
}

// TODO::
// calculate bound box about points within vector v
// then get the dimension with the maximum expansion
template <typename CustomVertex>
void KdTree<CustomVertex>::setSplittingPlane(std::vector<CustomTriangle<CustomVertex>>& v) {
  
}

#endif
