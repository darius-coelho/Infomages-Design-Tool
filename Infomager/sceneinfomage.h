#ifndef SCENEINFOMAGE_H
#define SCENEINFOMAGE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QAction>
#include <QGraphicsView>
#include <QKeyEvent>
#include <qmessagebox.h>

#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc.hpp>

#define PI 3.14159265

class SceneInfomage : public QGraphicsScene
{
public:
	SceneInfomage(QObject* parent = 0);
	void setOrigBackdrop(cv::Mat img, QPixmap pix_backdrop);
	void setCurrentBackdrop(cv::Mat img);
	cv::Mat getCurrentBackdrop();
	void refreshBackrop(QPixmap pix_backdrop);
	void removePieLabels();
	void clearBack();

protected:


private:
	//Inputs
	cv::Mat origBckdrp;
	cv::Mat currBckdrp;
	QGraphicsPixmapItem* pxmap_backdrop;
};



#endif // SCENE_H