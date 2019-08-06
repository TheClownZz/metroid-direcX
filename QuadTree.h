#pragma once

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>
#include "Viewport.h"
#include "Object.h"
#include "Items.h"
#include "Port.h"
#include "Enemy.h"
#define MAX_LEVEL                15
#define MAX_OBJECT_IN_REGION     5
class QuadTree
{
protected:

	int getIndex(RECT body);
	void split(); //thuc hien chia ra cac node
	bool isContain(RECT r);

public:
	QuadTree();
	QuadTree(int level, RECT bound);
	~QuadTree();
	void Clear();
	void insertObject(Object *object);

	/*lay danh sach nhung Entity co kha nang xay ra va cham
	tra ve danh sach cac phan tu nam trong vung va cham */
	void getObjectsCollideAble(std::vector<Object*> &ObjectOut, std::vector<Object*> &listwall, std::vector<RECT> &ListScene, RECT rect);

	void getAllObject(std::vector<Object*> &ObjectOut, std::vector<RECT> &ListScene, RECT rect);

	int getTotalObject();

	RECT Bound;
	int mLevel; //tuong ung voi so node
	std::vector<Object*> mListObject; //danh sach cac phan tu co trong vung va cham (Bound)
	QuadTree **Nodes;

};