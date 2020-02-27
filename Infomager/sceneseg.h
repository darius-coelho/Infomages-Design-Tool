#ifndef SCENESEG_H
#define SCENESEG_H

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

class SceneSeg : public QGraphicsScene
{
public:
	
	SceneSeg(QObject* parent = 0);
	enum Mode { NoMode, SelectObject, DrawLineBG, DrawLineFG, DrawRect };

	void setMode(Mode mode);
	void setOrigBackdrop(cv::Mat img, QPixmap pix_backdrop);
	void setCurrentBackdrop(cv::Mat img);
	void refreshBackrop(QPixmap pix_backdrop);
	void clearBack();
	cv::Mat segment();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void makeMarkersControllable(bool areControllable);
	void keyPressEvent(QKeyEvent *event);

private:
	Mode sceneMode;
	//Inputs
	cv::Mat origBckdrp;
	cv::Mat currBckdrp;
	QGraphicsPixmapItem* pxmap_backdrop;

	//Grabcut Req
	cv::Mat msk;
	cv::Mat bgModel, fgModel;

	bool mouseDown;

	//Graphics Items
	QPointF lorigPoint;
	QGraphicsLineItem* itemToDraw;
	QGraphicsRectItem* rectToDraw;
	QGraphicsPixmapItem* backdrop;
};

#endif // SCENE_H
