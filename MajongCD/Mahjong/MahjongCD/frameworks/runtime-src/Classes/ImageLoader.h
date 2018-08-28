#pragma once
#include "cocos2d.h"
#include <vector>
#include <string>
USING_NS_CC;

using namespace std;

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	void AddImageAsync(vector<string>& sFiles);
	void Load();
	void LoadOneDone(cocos2d::Texture2D* texture);

private:
	int m_index;
	vector<string> m_FileList;
};

