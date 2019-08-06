#pragma once
#include"MapObject.h"
#include<string>
#include <vector>
using namespace std;
#include "TinyXML\tinyxml.h"
class MapObjectGroup
{
public:
	string name;
	vector<MapObject*> Objects;
	int NumOnjects;
	MapObjectGroup(TiXmlElement *node);
	~MapObjectGroup();
	void ReadXML(TiXmlElement *node);
};