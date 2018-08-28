#include "ImageLoader.h"


ImageLoader::ImageLoader()
:m_index(0)
{
}


ImageLoader::~ImageLoader()
{
}


void ImageLoader::AddImageAsync(vector<string>& sFiles)
{
	if (sFiles.size()>0)
	{
		m_index = 0;
		m_FileList = sFiles;
		Load();
	}
}
void ImageLoader::Load()
{
	Director::getInstance()->getTextureCache()->addImageAsync(m_FileList[m_index].c_str(), CC_CALLBACK_1(ImageLoader::LoadOneDone, this));
}
void ImageLoader::LoadOneDone(cocos2d::Texture2D* texture)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("OneImageLoaded");
	if (++m_index< m_FileList.size())
	{
		Load();
	}
	else
	{
		EventCustom lEvent("ImageLoadDone");
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&lEvent);
	}
}