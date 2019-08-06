#include "QuadTree.h"

QuadTree::~QuadTree()
{
}
QuadTree::QuadTree()
{

}
QuadTree::QuadTree(int level, RECT bound)
{
	//chia lam 4

	this->Bound = bound;
	this->mLevel = level;
}

void QuadTree::Clear()
{
	if (Nodes)
	{
		for (size_t i = 0; i < 4; i++)
		{
			if (Nodes[i])
			{
				Nodes[i]->Clear();
				delete Nodes[i];
				Nodes[i] = nullptr;
			}
		}

		delete[] Nodes;
	}
}

void QuadTree::insertObject(Object *object)
{
	if (Nodes)
	{
		if (Nodes[0]->isContain(object->GetBound()))
			Nodes[0]->insertObject(object);
		if (Nodes[1]->isContain(object->GetBound()))
			Nodes[1]->insertObject(object);
		if (Nodes[2]->isContain(object->GetBound()))
			Nodes[2]->insertObject(object);
		if (Nodes[3]->isContain(object->GetBound()))
			Nodes[3]->insertObject(object);

		return; //Thoát ra luôn
	}

	//Thêm đối tượng vào quadtree
	if (this->isContain(object->GetBound()))
		this->mListObject.push_back(object);


	//Chia nhỏ và chèn đối tượng tiếp 1 là số đối tượng tối thiểu trong 1 node 6 là level tối đa
	if (mListObject.size() > 5 && mLevel < 6)
	{
		split();

		while (!mListObject.empty())
		{
			if (Nodes[0]->isContain(mListObject.back()->GetBound()))
				Nodes[0]->insertObject(mListObject.back());
			if (Nodes[1]->isContain(mListObject.back()->GetBound()))
				Nodes[1]->insertObject(mListObject.back());
			if (Nodes[2]->isContain(mListObject.back()->GetBound()))
				Nodes[2]->insertObject(mListObject.back());
			if (Nodes[3]->isContain(mListObject.back()->GetBound()))
				Nodes[3]->insertObject(mListObject.back());

			mListObject.pop_back();
		}
	}
}

bool QuadTree::isContain(RECT r)
{
	return !(r.right < Bound.left || r.left > Bound.right || r.top < Bound.bottom || r.bottom > Bound.top);
}

void QuadTree::split()
{
	//cat phan region (ranh gioi) ra thanh 4 phan bang nhau
	Nodes = new QuadTree *[4];

	RECT bound;

	int width = (Bound.right - Bound.left) / 2;
	int height = (Bound.top - Bound.bottom) / 2;

	//phan goc trai tren
	bound.left = Bound.left;
	bound.right = bound.left + width;
	bound.top = Bound.top;
	bound.bottom = bound.top - height;
	Nodes[0] = new QuadTree(mLevel + 1, bound);

	//phan goc phai tren
	bound.left = Bound.left + width;
	bound.right = bound.left + width;
	bound.top = Bound.top;
	bound.bottom = bound.top - height;
	Nodes[1] = new QuadTree(mLevel + 1, bound);

	//phan goc trai duoi
	bound.left = Bound.left;
	bound.right = bound.left + width;
	bound.top = Bound.top - height;
	bound.bottom = bound.top - height;
	Nodes[2] = new QuadTree(mLevel + 1, bound);

	//phan goc phai duoi
	bound.left = Bound.left + width;
	bound.right = bound.left + width;
	bound.top = Bound.top - height;
	bound.bottom = bound.top - height;
	Nodes[3] = new QuadTree(mLevel + 1, bound);
}

int QuadTree::getTotalObject()
{
	int total = mListObject.size();

	if (Nodes)
	{
		for (size_t i = 0; i < 4; i++)
		{
			total += Nodes[i]->getTotalObject();
		}
	}

	return total;
}

int QuadTree::getIndex(RECT body)
{
	/*lay vi tri cua Entity
	0: nam trong Node con goc trai tren
	1: nam trong Node con goc phai tren
	2: nam trong Node con goc trai duoi
	3: nam trong Node con goc phai duoi
	-1: bi dinh > 2 node con*/

	float middleVerticle = Bound.left + (Bound.right - Bound.left) / 2.0f;
	float middleHorizontal = Bound.top - (Bound.top - Bound.bottom) / 2.0f;

	if (body.top < Bound.top && body.bottom > middleHorizontal)
	{
		//nam phia ben tren
		if (body.left > Bound.left && body.right < middleVerticle)
		{
			//nam phia ben trai
			return 0;
		}
		else if (body.left > middleVerticle && body.right < Bound.right)
		{
			//nam phia ben phai
			return 1;
		}
	}
	else if (body.top < middleHorizontal && body.bottom > Bound.bottom)
	{
		//nam phia ben duoi
		if (body.left > Bound.left && body.right < middleVerticle)
		{
			//nam phia ben trai
			return 2;
		}
		else if (body.left > middleVerticle && body.right < Bound.right)
		{
			//nam phia ben phai
			return 3;
		}
	}

	return -1;
}

void QuadTree::getAllObject(std::vector<Object*> &ObjectOut, std::vector<RECT> &ListScene, RECT rect)
{
	for (auto child : mListObject)
	{
		RECT r = child->GetBound();
		if (!(r.right < rect.left || r.left > rect.right || r.top < rect.bottom || r.bottom > rect.top) || child->Tag == Object::tag::Port)
		{
			if (child->Tag == Object::Scene)
				ListScene.push_back(r);
			else
				ObjectOut.push_back(child);
		}
	}

	if (Nodes)
	{
		for (size_t i = 0; i < 4; i++)
		{
			Nodes[i]->getAllObject(ObjectOut, ListScene, rect);
		}
	}
}

void QuadTree::getObjectsCollideAble(std::vector<Object*> &ObjectOut, std::vector<Object*> &listwall, std::vector<RECT> &ListScene, RECT rect)
{
	if (Nodes)
	{
		if (Nodes[0]->isContain(rect))
			Nodes[0]->getObjectsCollideAble(ObjectOut, listwall, ListScene, rect);
		if (Nodes[1]->isContain(rect))
			Nodes[1]->getObjectsCollideAble(ObjectOut, listwall, ListScene, rect);
		if (Nodes[2]->isContain(rect))
			Nodes[2]->getObjectsCollideAble(ObjectOut, listwall, ListScene, rect);
		if (Nodes[3]->isContain(rect))
			Nodes[3]->getObjectsCollideAble(ObjectOut, listwall, ListScene, rect);

		return; // Thoát luôn
	}

	//Lấy tất cả đối tượng trong vùng
	if (isContain(rect))
	{
		for (auto child : mListObject)
		{
			bool istrue = true;
			for (size_t i = 0; i < ObjectOut.size(); i++)
			{
				if (child->id == ObjectOut.at(i)->id)
				{
					istrue = false;
					break;
				}
			}
			for (size_t i = 0; i < listwall.size(); i++)
			{
				if (child->id == listwall.at(i)->id)
				{
					istrue = false;
					break;
				}
			}

			if (istrue)
			{
				if (child->Tag == Object::tag::Port || child->Tag == Object::tag::Wall)
				{
						listwall.push_back(child);
				}
					
				else
				{
					RECT r = child->GetBound();
					if (!(r.right < rect.left || r.left > rect.right || r.top < rect.bottom || r.bottom > rect.top) )
					{
						if (child->Tag == Object::Scene)
							ListScene.push_back(r);
						else
							ObjectOut.push_back(child);
					}
				}
			}
		}
	}
}
