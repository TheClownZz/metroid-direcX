#include "ObjectManager.h"
#include "string.h"
#pragma warning(disable : 4996)

ObjectManager::ObjectManager()
{
}
ObjectManager::~ObjectManager()
{
	delete map;
	delete viewport;
	delete Samus;
	delete gui;
	mQuadTree->Clear();
	delete mQuadTree;

	delete sprite_Samus;
	delete sprite_bullet;
	delete sprite_enemy;
	delete sprite_port;
	delete sprite_item;
	delete sprite_boss;

	delete mKraid;
	delete mRidley;

	delete soundEnter;
	delete soundBrinstar;
	delete soundpowerup;
	delete soundSamusColli;
	delete soundRipperColli;
	delete soundEnemyColli;
}

//Load Data Game
void ObjectManager::InitDT(Graphic* graphic, Sound *sound)
{
	//load nhạc
	this->sound = sound;
	soundEnter = sound->LoadSound(SoundEnter);
	soundBrinstar = sound->LoadSound(SoundBrinstar);
	soundpowerup = sound->LoadSound(SoundPowerUp);
	soundSamusColli = sound->LoadSound(SoundSamusCollision);
	soundRipperColli = sound->LoadSound(SoundRipperCollision);
	soundEnemyColli = sound->LoadSound(SoundEnemyCollision);
	//load sprite
	sprite_Samus = new Sprite(graphic, SamusAranNormalPNG);
	sprite_bullet = new Sprite(graphic, BulletPNG);
	sprite_enemy = new Sprite(graphic, EnemyPNG);
	sprite_port = new Sprite(graphic, PortPNG);
	sprite_item = new Sprite(graphic, ItemsPNG);
	sprite_boss = new Sprite(graphic, BossPNG);
	//load info sprite
	info_Samus;
	info_bullet = new InfoSprite(BulletXML);
	info_enemy = new InfoSprite(EnemyXML);
	info_port = new InfoSprite(PortXML);
	info_item = new InfoSprite(ItemsXML);
	info_boss = new InfoSprite(BossXML);
	//load Objecct
	map = new Map(graphic);
	viewport = new Viewport(512, 1440);
	gui = new GUI(graphic);
	//Samus bắt đầu 640x1250
	Samus = new SamusAran(sprite_Samus, sprite_bullet,info_bullet, sound);
	Samus->SetPositionStart(D3DXVECTOR2(640, 1266));
	//Kraid bắt đầu 3872, 1360
	mKraid = new Kraid(sprite_boss,info_boss, D3DXVECTOR2(3872, 1360));
	//Ridley bắt đầu 3376, 1376
	mRidley = new Ridley(sprite_boss,info_boss, D3DXVECTOR2(3376, 1376));

	//Tạo quadtree và đọc
	mQuadTree = new QuadTree();
	TiXmlDocument doc(QuadTreeXML);
	if (!doc.LoadFile())
	{
		MessageBox(NULL, "Can't load QuadTree.xml file", "Error", MB_OK);
		return;
	}
	TiXmlElement* root = doc.RootElement();
	ReadQuadTree(root, mQuadTree, NULL, -1);
	//Lấy quadtree đầu tiên
	posView = viewport->GetPosition();
	mQuadTree->getObjectsCollideAble(listObjectCollison, listWall, listScene, viewport->GetBoundViewport());

	timepause = 0.0f;
	pause = false;
	allowpause = false;
}

//Update Game
void ObjectManager::Update(float gameTime, Keyboard* key)
{
	//Nhấn enter để pause game
	if (key->IsKeyDown(Dik_START) && allowpause)
	{
		allowpause = false;
		pause = !pause;
	}
	if (key->GIsKeyUp(Dik_START))
	{
		allowpause = true;
	}
	//pause game
	timepause -= gameTime;
	if (timepause < 0 && !pause)
	{

		Samus->ChangeAnimation(gameTime, key);
		//Kiểm tra va chạm
		if (posView != viewport->GetPosition())
		{
			posView = viewport->GetPosition();
			listScene.clear();
			listWall.clear();
			RECT viewbound = viewport->GetBoundViewport();
			//List lưu đối tượng quái còn trong màn hình trước đó
			mQuadTree->getObjectsCollideAble(list, listWall, listScene, viewport->GetBoundViewport());
			//Đổi 2 vector
			listObjectCollison.swap(list);
		}
		list.clear();

		//Va chạm Samus với Tường trước
		for (size_t i = 0; i < listWall.size(); i++)
		{
			//Samus
			D3DXVECTOR2 disSamus = Samus->Distance(gameTime);
			Samus->OnCollision(listWall.at(i), disSamus, disSamus);

			//Đạn Samus
			for (int j = 0; j < 8; j++)
			{	//Không kiểm tra khi không vẽ và phải trong trạng thái bắn
				if (Samus->bullet[j]->GetAllowDraw() && Samus->bullet[j]->GetStateBullet() == Bullet::Firing)
					//Kiểm tra va chạm đạn và object[i]
					if (j < BulletMax || (j >= BulletMax && listWall.at(i)->Tag != Object::tag::Wall))
						if (Collision::isCollision(Samus->bullet[j]->GetBound(), listWall.at(i)->GetBound()))
						{
							listWall.at(i)->OnCollision();
							Samus->bullet[j]->OnCollision();
						}
			}

			//Update
			listWall.at(i)->Update(gameTime, key);
		}

		//Va chạm samus
		for (size_t i = 0; i < listObjectCollison.size(); i++)
		{
			//Va chạm với Items
			if (listObjectCollison.at(i)->Tag == Object::tag::Item)
			{
				//kiểm tra va chạm
				if (Collision::isCollision(Samus->GetBound(), listObjectCollison.at(i)->GetBound()))
				{
					listObjectCollison.at(i)->OnCollision();
					if (listObjectCollison.at(i)->GetHeight() == 14)
					{
						sound->StopSound(soundBrinstar);
						sound->PlaySoundA(soundpowerup);
						timepause = 4.0f;
					}
				}
			}

			//Va chạm với Lửa hole
			if (listObjectCollison.at(i)->Tag == Object::tag::Hole)
			{
				//kiểm tra va chạm
				if (Collision::isCollision(Samus->GetBound(), listObjectCollison.at(i)->GetBound()))
				{
					Samus->SetBleedState(0, 1, true);
					Samus->SetVelocityX(Samus->GetVelocity().x / 4);

					sound->PlaySoundA(soundSamusColli);
				}
			}

			//Va chạm với quái
			if (listObjectCollison.at(i)->Tag == Object::tag::Enemy  && listObjectCollison.at(i)->GetHP() > 0)
			{
				listObjectCollison.at(i)->OnCollision();

				//Phải kiểm tra với tường trước 
				for (size_t j = 0; j < listWall.size(); j++)
				{
					D3DXVECTOR2 disEnemy = listObjectCollison.at(i)->Distance(gameTime);
					listObjectCollison.at(i)->OnCollision(listWall.at(j), disEnemy);
				}

				//Samus
				D3DXVECTOR2 disSamus = Samus->Distance(gameTime);
				D3DXVECTOR2 distance = disSamus - listObjectCollison.at(i)->Distance(gameTime);
				Samus->OnCollision(listObjectCollison.at(i), distance, disSamus);

				//Đạn Samus
				for (int j = 0; j < 8; j++)
				{	//Không kiểm tra khi không vẽ và phải trong trạng thái bắn
					if (Samus->bullet[j]->GetAllowDraw() && Samus->bullet[j]->GetStateBullet() == Bullet::Firing)
						if (Collision::isCollision(Samus->bullet[j]->GetBound(), listObjectCollison.at(i)->GetBound()))
						{
							if (listObjectCollison.at(i)->GetName() == "Ripper")
								sound->PlaySoundA(soundRipperColli);
							else
								sound->PlaySoundA(soundEnemyColli);

							listObjectCollison.at(i)->OnCollision(Samus->bullet[j]);
							Samus->bullet[j]->OnCollision();
						}
				}
				//Kiểm tra con quái nào còn trong màn hình và chưa chết
				if (viewport->isContains(listObjectCollison.at(i)->GetBound()) && listObjectCollison.at(i)->GetAllowDraw())
					list.push_back(listObjectCollison.at(i));
			}
			//Update
			listObjectCollison.at(i)->Update(gameTime, key);
		}

		//Boss Kraid
		if (mKraid->GetAllowDraw())
		{
			mKraid->OnCollision(Samus);
			//Samus
			D3DXVECTOR2 disSamus = Samus->Distance(gameTime);
			D3DXVECTOR2 distance = disSamus - mKraid->Distance(gameTime);
			Samus->OnCollision(mKraid, distance, disSamus);

			//Đạn Samus
			for (int i = 0; i < 8; i++)
			{	//Không kiểm tra khi không vẽ và phải trong trạng thái bắn
				if (Samus->bullet[i]->GetAllowDraw() && Samus->bullet[i]->GetStateBullet() == Bullet::Firing)
				{
					if (Collision::isCollision(Samus->bullet[i]->GetBound(), mKraid->GetBound()))
					{
						mKraid->OnCollision(Samus->bullet[i]);
						Samus->bullet[i]->OnCollision();
					}
					//Va chạm đạn boss với đạn Samus
					for (int j = 0; j < 3; j++)
						if (Collision::isCollision(Samus->bullet[i]->GetBound(), mKraid->mlistBullet[j]->GetBound()))
						{
							mKraid->mlistBullet[j]->OnCollision(Samus->bullet[i]);
							Samus->bullet[i]->OnCollision();
						}
				}
			}

			//Va chạm boss với tường
			for (size_t i = 0; i < listWall.size(); i++)
			{
				D3DXVECTOR2 disKraid = mKraid->Distance(gameTime);
				mKraid->OnCollision(listWall.at(i), disKraid);

				if (listWall.at(i)->Tag == Object::Port)
				{
					if (mKraid->GetHP() > 0)
						listWall.at(i)->New();
					else
						listWall.at(i)->OnCollision();
				}
			}
			//va cham dan boss vơi player
			int side;//hướng va chạm
			if (Samus->GetPosition().x < mKraid->GetPosition().x)
				side = 1;
			else
				side = -1;

			for (int i = 0; i < 2; i++)
				if (Collision::isCollision(Samus->GetBound(), mKraid->mlistBumerang[i]->GetBound()))
					Samus->SetBleedState(side, mKraid->mlistBumerang[i]->GetDamage(), true);
			for (int i = 0; i < 3; i++)
				if (Collision::isCollision(Samus->GetBound(), mKraid->mlistBullet[i]->GetBound()))
					Samus->SetBleedState(side, mKraid->mlistBullet[i]->GetDamage(), true);
			//Boss update
			mKraid->Update(gameTime, key);
		}
		//Vào scene thì xuất hiện ra thì tắt
		if (Collision::isCollision(Samus->GetPosition().x, Samus->GetPosition().y, mKraid->sceneboss))
			mKraid->SetAllowDraw(true);
		else
			mKraid->SetAllowDraw(false);


		//Boss Ridley
		if (mRidley->GetAllowDraw())
		{
			mRidley->OnCollision(Samus);
			//Samus
			D3DXVECTOR2 disSamus = Samus->Distance(gameTime);
			D3DXVECTOR2 distance = disSamus - mRidley->Distance(gameTime);
			Samus->OnCollision(mRidley, distance, disSamus);

			//Đạn Samus với boss
			for (int i = 0; i < 8; i++)
			{	//Không kiểm tra khi không vẽ và phải trong trạng thái bắn
				if (Samus->bullet[i]->GetAllowDraw() && Samus->bullet[i]->GetStateBullet() == Bullet::Firing)
				{
					if (Collision::isCollision(Samus->bullet[i]->GetBound(), mRidley->GetBound()))
					{
						mRidley->OnCollision(Samus->bullet[i]);
						Samus->bullet[i]->OnCollision();
					}
					//Va chạm đạn boss với đạn Samus
					for (int j = 0; j < 6; j++)
						if (mRidley->mListBullet[j]->GetAllowDraw())
							if (Collision::isCollision(Samus->bullet[i]->GetBound(), mRidley->mListBullet[j]->GetBound()))
								Samus->bullet[i]->OnCollision();
				}
			}

			//Va chạm boss với tường
			for (size_t i = 0; i < listWall.size(); i++)
			{
				//boss
				D3DXVECTOR2 disRidley = mRidley->Distance(gameTime);
				mRidley->OnCollision(listWall.at(i), disRidley);
				//đạn boss
				for (int j = 0; j < 6; j++)
				{
					D3DXVECTOR2 disRidley = mRidley->mListBullet[j]->Distance(gameTime);
					mRidley->mListBullet[j]->OnCollision(listWall.at(i), disRidley);
				}
				//Tạo cổng chặn Samus khi vào
				if (listWall.at(i)->Tag == Object::Port)
				{
					if (mRidley->GetHP() > 0)
						listWall.at(i)->New();
					else
						listWall.at(i)->OnCollision();
				}
			}

			// va cham dan boss vơi player
			int side;//hướng va chạm
			if (Samus->GetPosition().x < mKraid->GetPosition().x)
				side = 1;
			else
				side = -1;
			for (int i = 0; i < 6; i++)
				if (Collision::isCollision(Samus->GetBound(), mRidley->mListBullet[i]->GetBound()))
					Samus->SetBleedState(1, mRidley->mListBullet[i]->GetDamage(), true);

			mRidley->Update(gameTime, key);
		}
		if (Collision::isCollision(Samus->GetPosition().x, Samus->GetPosition().y, mRidley->sceneboss))
			mRidley->SetAllowDraw(true);
		else
			mRidley->SetAllowDraw(false);

		//Va chạm Samus với Tường trước
		for (size_t i = 0; i < listWall.size(); i++)
		{
			//Samus
			D3DXVECTOR2 disSamus = Samus->Distance(gameTime);
			Samus->OnCollision(listWall.at(i), disSamus, disSamus);
		}

		//Kiểm tra va chạm xong mới update
		Samus->Update(gameTime, key);

		//Update Viewport theo vị trí Samus
		D3DXVECTOR2 posSamus = Samus->GetPosition();
		viewport->Update(gameTime, key, posSamus, listScene);
		//Kiểm tra xem có rơi ra Scene không
		if (posSamus != Samus->GetPosition())
		{
			Samus->SetPosition(posSamus);
		}
	}
	else
	{
		if (timepause > 4.0f)
		{
			Samus->ChangeAnimation(gameTime, key);
			Samus->Update(gameTime, key);
		}
		if (timepause < 0.5f)
			sound->LoopSound(soundBrinstar);
	}
}
//Start
void ObjectManager::Start()
{
	timepause = 7.0f;
	sound->PlaySoundA(soundEnter);
	viewport->SetPosition(D3DXVECTOR2(512, 1440));
	allowpause = false;
	Samus->New();
	mRidley->New();
	mKraid->New();
}
//Kiểm tra kết thúc
int ObjectManager::End()
{
	if (Samus->GetHP() <= 0)
	{
		sound->StopSound(soundBrinstar);
		return 1;
	}
	if (mKraid->GetHP() <= 0)
	{
		sound->StopSound(soundBrinstar);
		return 2;
	}
	//0 là chạy tiếp
	return 0;
}

//Vẽ
void ObjectManager::Render()
{
	//Vẽ boss trước 
	mKraid->Render(viewport);
	mRidley->Render(viewport);

	//Vẽ Object
	for (size_t i = 0; i < listObjectCollison.size(); i++)
	{
		//Va chạm với Items với Port thì không vẽ Items
		if (listObjectCollison.at(i)->Tag == Object::tag::Item)
		{
			bool istrue = true;
			for (size_t j = 0; j < listWall.size(); j++)
				if (listWall.at(j)->Tag == Object::Port)
					if (Collision::isCollision(listWall.at(j)->GetBound(), listObjectCollison.at(i)->GetBound()))
					{
						istrue = false;
						break;
					}
			if (istrue)
				listObjectCollison.at(i)->Render(viewport);
		}
		else
		{
			listObjectCollison.at(i)->Render(viewport);
		}
	}

	Samus->Render(viewport);
	//Vẽ Port
	for (size_t i = 0; i < listWall.size(); i++)
		listWall.at(i)->Render(viewport);

	//Vẽ map
	map->Render(viewport);

	//GUI
	gui->Render(Samus);
}




bool IsExist(int id, int indexNode, QuadTree *node, QuadTree *father)
{
	if (!father)
		return false;
	for (int i = 0; i < indexNode; i++)
	{
		for (size_t j = 0; j < father->Nodes[i]->mListObject.size(); j++)
		{
			if (father->Nodes[i]->mListObject.at(j)->id == id)
			{
				node->mListObject.push_back(father->Nodes[i]->mListObject.at(j));
				return true;
			}
		}
	}

	return false;
}

void ObjectManager::ReadQuadTree(TiXmlElement *root, QuadTree *node, QuadTree *father, int indexNode)
{
	if (!root)
		return;
	RECT r;
	int left, top, right, bottom, numObj;
	root->QueryIntAttribute("left", &left);
	root->QueryIntAttribute("top", &top);
	root->QueryIntAttribute("right", &right);
	root->QueryIntAttribute("bottom", &bottom);
	r.left = left;
	r.right = right;
	r.top = top;
	r.bottom = bottom;
	node->Bound = r;
	root->QueryIntAttribute("level", &node->mLevel);
	root->QueryIntAttribute("NumObj", &numObj);
	TiXmlElement *child = root->FirstChildElement();
	for (size_t i = 0; i < numObj; i++)
	{
		int id;
		child->QueryIntAttribute("id", &id);
		if (!IsExist(id, indexNode, node, father))
		{

			float x, y, w, h;
			child->QueryFloatAttribute("x", &x);
			child->QueryFloatAttribute("y", &y);
			child->QueryFloatAttribute("w", &w);
			child->QueryFloatAttribute("h", &h);

			string name = (char*)child->Attribute("name");
			Object::tag tagg = Object::GetTag(name);
			if (tagg == Object::tag::Enemy)
			{
				Enemy *enemy = new Enemy(Samus, sprite_enemy, sprite_item, sprite_bullet, info_bullet, info_enemy, info_item, sound);
				enemy->NewEnemy(D3DXVECTOR2(x, y), enemy->GetType(name));
				enemy->SetName(name);
				enemy->id = id;
				node->mListObject.push_back(enemy);
			}
			else if (tagg == Object::tag::Port)
			{
				Port *port = new Port(map->GetInfoMap(), Samus, sprite_port, info_port,sound);
				port->NewPort(D3DXVECTOR2(x, y), Port::GetType(name));
				port->SetName(name);
				port->SetPositionStart(D3DXVECTOR2(x, y));
				port->id = id;
				node->mListObject.push_back(port);
			}
			else if (tagg == Object::tag::Item)
			{
				Items *item = new Items(Samus, sprite_item,info_item, sound);
				item->NewItems(D3DXVECTOR2(x, y), Items::GetType(name));
				item->SetName(name);
				item->SetPositionStart(D3DXVECTOR2(x, y));
				item->id = id;
				node->mListObject.push_back(item);
			}
			else
			{
				Object *obj = new Object();
				child->QueryIntAttribute("id", &obj->id);
				obj->SetTag(tagg);
				obj->SetPosition(D3DXVECTOR2(x, y));
				obj->GetBound(w, h);
				obj->SetName(name);
				obj->SetPositionStart(D3DXVECTOR2(x, y));
				obj->id = id;
				node->mListObject.push_back(obj);
			}
		}
		child = child->NextSiblingElement();
	}
	if (child)
	{
		node->Nodes = new QuadTree*[4];
		for (int i = 0; i < 4; i++)
		{
			node->Nodes[i] = new QuadTree();
			ReadQuadTree(child, node->Nodes[i], node, i);
			child = child->NextSiblingElement();
		}
	}
}