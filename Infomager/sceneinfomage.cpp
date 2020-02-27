#include "sceneinfomage.h"

SceneInfomage::SceneInfomage(QObject* parent) : QGraphicsScene(parent)
{
	pxmap_backdrop = new QGraphicsPixmapItem();
}

void SceneInfomage::setOrigBackdrop(cv::Mat img, QPixmap pix_backdrop)
{
	img.copyTo(origBckdrp);
	//origBckdrp = img;	
	setCurrentBackdrop(origBckdrp);
	
	removeItem(pxmap_backdrop);
	pxmap_backdrop->setPixmap(pix_backdrop);
	addItem(pxmap_backdrop);
}

void SceneInfomage::setCurrentBackdrop(cv::Mat img)
{
	img.copyTo(currBckdrp);
}

cv::Mat SceneInfomage::getCurrentBackdrop()
{
	return currBckdrp.clone();
}

void SceneInfomage::refreshBackrop(QPixmap pix_backdrop)
{
	pxmap_backdrop->setPixmap(pix_backdrop);
}

void SceneInfomage::removePieLabels()
{
	foreach(QGraphicsItem* item, items())
	{
		if (item->zValue() == 600)
		{
			removeItem(item);
		}
	}
}

void SceneInfomage::clearBack()
{
	removeItem(pxmap_backdrop);
}