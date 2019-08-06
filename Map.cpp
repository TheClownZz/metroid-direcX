#include "Map.h"
#include"TinyXML\tinyxml.h"

void WriteNode(TiXmlElement *root, QuadTree *node)
{
	if (node == NULL)
		return;
	root->SetAttribute("level", node->mLevel);
	root->SetAttribute("left", node->Bound.left);
	root->SetAttribute("top", node->Bound.top);
	root->SetAttribute("right", node->Bound.right);
	root->SetAttribute("bottom", node->Bound.bottom);
	root->SetAttribute("NumObj", node->mListObject.size());
	for (size_t i = 0; i < node->mListObject.size(); i++)
	{
		TiXmlElement* child = new TiXmlElement("obj");
		child->SetAttribute("id", node->mListObject.at(i)->id);
		child->SetAttribute("name", (char *)node->mListObject.at(i)->GetName().c_str());
		child->SetAttribute("x", node->mListObject.at(i)->GetPosition().x);
		child->SetAttribute("y", node->mListObject.at(i)->GetPosition().y);
		child->SetAttribute("w", node->mListObject.at(i)->GetWidth());
		child->SetAttribute("h", node->mListObject.at(i)->GetHeight());
		root->LinkEndChild(child);
	}
	if (node->Nodes)
	{
		for (size_t i = 0; i < 4; i++)
		{
			//if (node->Nodes[i] != NULL)
			{
				TiXmlElement* child = new TiXmlElement("node");
				WriteNode(child, node->Nodes[i]);
				root->LinkEndChild(child);
			}
		}
	}

}
void WriteQuadtree(QuadTree *node)
{
	TiXmlDocument doc;
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "utf-8", "");
	doc.LinkEndChild(dec);
	TiXmlElement* root = new TiXmlElement("Node");
	WriteNode(root, node);
	doc.LinkEndChild(root);
	doc.SaveFile("Resource Files/QuadTree.xml");
}


Map::Map()
{
}

Map::Map(Graphic* graphic)
{
	

	info = new InfoMap(MapXML);
	this->tileset = new TileSet(graphic,info->tileCount, info->tileColumns, info->tileWidth, info->tileHeight);
	position = D3DXVECTOR2(0, 0);
	/*
	RECT r;
	r.left = 0;
	r.right = r.left + info->width*info->tileWidth;
	r.bottom = 0;
	r.top = r.bottom + info->height*info->tileHeight;
	if (r.right < r.top)
		r.right = r.top;
	else
		r.top = r.right;
	mQuadTree = new QuadTree(1, r);
	for (int i = 0; i < info->numObjectGroups; i++)
	{
		for (int j = 0; j < info->ObjectGroups.at(i)->NumOnjects; j++)
		{
			Object *obj = new Object();
			float x = info->ObjectGroups.at(i)->Objects.at(j)->x;
			float y = info->ObjectGroups.at(i)->Objects.at(j)->y;
			float w = info->ObjectGroups.at(i)->Objects.at(j)->width;
			float h = info->ObjectGroups.at(i)->Objects.at(j)->height;
			D3DXVECTOR2 pos;
			pos.x = x + w / 2;
			pos.y = info->height*info->tileHeight - y - h / 2;
			obj->SetPosition(pos);
			obj->GetBound(w, h);
			obj->id = info->ObjectGroups.at(i)->Objects.at(j)->id;
			obj->SetName(info->ObjectGroups.at(i)->Objects.at(j)->name);
			mQuadTree->insertObject(obj);
		}
	}
	WriteQuadtree(mQuadTree); 
	*/
}

Map::~Map()
{
	delete tileset;
	delete info;
}

void Map::Update(float gameTime)
{

}

InfoMap* Map::GetInfoMap()
{
	return info;
}

int Map::GetData(D3DXVECTOR2 position)
{
	int w = position.x / info->tileWidth;
	int h = position.y / info->tileHeight;
	return info->data[h][w];
}
void Map::SetData(D3DXVECTOR2 position, int data)
{
	int w = position.x / info->tileWidth;
	int h = position.y / info->tileHeight;
	info->data[h][w] = data;
}

void Map::Render(Viewport * view)
{
	int startH, startW, endH, endW;
	RECT r = view->GetBoundViewport();
	startH = r.bottom / info->tileHeight;
	endH = r.top / info->tileHeight + 1;
	startW = r.left / info->tileWidth;
	endW = r.right / info->tileWidth + 1;
	
	for (int h = startH; h < endH; h++)
		for (int w = startW; w < endW; w++)
		{
			int id = info->data[h][w];
			if (id != 0)
			{
				D3DXVECTOR2 position(w * info->tileWidth, (h+1) * info->tileHeight);
				tileset->Render(view, id, position);
			}
		}
}
