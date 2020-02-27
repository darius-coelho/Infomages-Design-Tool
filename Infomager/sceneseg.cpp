#include "sceneseg.h"

//Scene Setup
SceneSeg::SceneSeg(QObject* parent) : QGraphicsScene(parent)
{
	pxmap_backdrop = new QGraphicsPixmapItem();
	itemToDraw = 0;
	rectToDraw = 0;
	mouseDown = false;
}

void SceneSeg::setOrigBackdrop(cv::Mat img, QPixmap pix_backdrop)
{
	origBckdrp = img;
	setCurrentBackdrop(img);

	removeItem(pxmap_backdrop);
	pxmap_backdrop->setPixmap(pix_backdrop);
	addItem(pxmap_backdrop);

	msk = cv::Mat(this->height(), this->width(), CV_8UC1);
}

void SceneSeg::setCurrentBackdrop(cv::Mat img)
{
	img.copyTo(currBckdrp);
}

void SceneSeg::refreshBackrop(QPixmap pix_backdrop)
{
	pxmap_backdrop->setPixmap(pix_backdrop);
}

void SceneSeg::clearBack()
{
	removeItem(pxmap_backdrop);
}

//Scene Event Handling

void SceneSeg::setMode(Mode mode)
{
	sceneMode = mode;
	QGraphicsView::DragMode vMode = QGraphicsView::NoDrag;

	if (sceneMode == DrawLineFG || sceneMode == DrawLineBG || sceneMode == DrawRect)
	{
		makeMarkersControllable(false);
		vMode = QGraphicsView::NoDrag;
	}
	else if (sceneMode == SelectObject)
	{
		makeMarkersControllable(true);
		vMode = QGraphicsView::RubberBandDrag;
	}	
	else if (sceneMode == NoMode)
	{
		makeMarkersControllable(false);
	}

	QGraphicsView* mView = views().at(0);

	if (mView)
	{
		mView->setDragMode(vMode);
	}
}

void SceneSeg::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (sceneMode == DrawLineFG || sceneMode == DrawLineBG || sceneMode == DrawRect)
	{
		lorigPoint = event->scenePos();
	}
	mouseDown = true;
	QGraphicsScene::mousePressEvent(event);
}

void SceneSeg::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mouseDown)
	{
		if (sceneMode == DrawLineFG)
		{
			if (!itemToDraw)
			{
				itemToDraw = new QGraphicsLineItem;
				this->addItem(itemToDraw);
				itemToDraw->setPen(QPen(Qt::darkGreen, 3, Qt::SolidLine));
				itemToDraw->setPos(lorigPoint);
			}
			itemToDraw->setLine(0, 0,
				event->scenePos().x() - lorigPoint.x(),
				event->scenePos().y() - lorigPoint.y());
			QLineF l = itemToDraw->line();
			itemToDraw->setZValue(100);
		}
		else if (sceneMode == DrawLineBG)
		{
			if (!itemToDraw)
			{
				itemToDraw = new QGraphicsLineItem;
				this->addItem(itemToDraw);
				itemToDraw->setPen(QPen(Qt::blue, 3, Qt::SolidLine));
				itemToDraw->setPos(lorigPoint);
			}
			itemToDraw->setLine(0, 0,
				event->scenePos().x() - lorigPoint.x(),
				event->scenePos().y() - lorigPoint.y());
			QLineF l = itemToDraw->line();
			itemToDraw->setZValue(75);			
		}
		else if (sceneMode == DrawRect)
		{
			if (!rectToDraw)
			{
				rectToDraw = new QGraphicsRectItem;
				this->addItem(rectToDraw);
				rectToDraw->setPen(QPen(Qt::yellow, 3, Qt::SolidLine));			
			}
			rectToDraw->setZValue(50);
			rectToDraw->setPos(lorigPoint);
			rectToDraw->setRect(0, 0,
				event->scenePos().x() - lorigPoint.x(),
				event->scenePos().y() - lorigPoint.y());
		}
	}
	QGraphicsScene::mouseMoveEvent(event);
}

void SceneSeg::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
//	QMessageBox::information(0, "info", QString("x0: %1, y0: %2\nx1: %3, y1: %4 ").arg(itemToDraw->line().x1()).arg(itemToDraw->line().y1()).arg(itemToDraw->line().x2()).arg(itemToDraw->line().y2()));
	mouseDown = false;
	itemToDraw = 0;

	QGraphicsScene::mouseReleaseEvent(event);
}

void SceneSeg::makeMarkersControllable(bool areControllable)
{
	foreach(QGraphicsItem* item, items())
	{
		if (item->zValue() != 999 && item->type() != QGraphicsTextItem::Type && item->type() != QGraphicsPixmapItem::Type)
		{
			item->setFlag(QGraphicsItem::ItemIsSelectable, areControllable);
			item->setFlag(QGraphicsItem::ItemIsMovable, areControllable);
		}
	}
}

void SceneSeg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete)
	{
		int tmp_val;
		foreach(QGraphicsItem* item, selectedItems())
		{			
			removeItem(item);
			delete item;			
			item = 0;
		}		
	}
	else
	{
		QGraphicsScene::keyPressEvent(event);
	}
}

//Image Segmentation

cv::Mat SceneSeg::segment()
{
	CvRect box;
	int boxSet = 0;

	//Draw Rect on Mask
	if (rectToDraw)
	{
		QRectF r = rectToDraw->boundingRect();
		box.x = rectToDraw->x();
		box.y = rectToDraw->y();
		box.width = r.width();
		box.height = r.height();
		msk.setTo(cv::GC_BGD);
		(msk(box)).setTo(cv::Scalar(cv::GC_PR_FGD));
		boxSet = 1;
	}
	else
	{
		msk.setTo(cv::GC_PR_FGD);
	}
	
	//Draw Lines on Mask
	foreach(QGraphicsItem* item, items())
	{
		if (item->type() == QGraphicsLineItem::Type)
		{
			itemToDraw = (QGraphicsLineItem*) item;
			int x = item->x();
			int y = item->y();
			if (item->zValue() == 100)
			{				
				cv::line(msk, cv::Point(itemToDraw->line().x1()+x, itemToDraw->line().y1()+y), cv::Point(itemToDraw->line().x2()+x, itemToDraw->line().y2()+y), cv::GC_PR_FGD);
			}
			else
			{
				cv::line(msk, cv::Point(itemToDraw->line().x1() + x, itemToDraw->line().y1() + y), cv::Point(itemToDraw->line().x2() + x, itemToDraw->line().y2() + y), cv::GC_BGD);
			}			
		}
	}
	itemToDraw = 0;

	//Perform segmentation
	resize(msk, msk, origBckdrp.size(), 0, 0, CV_INTER_NN);
	grabCut(origBckdrp,    // input image
		msk,   // segmentation result
		box,// rectangle containing foreground 
		bgModel, fgModel, // models
		1,        // number of iterations
		cv::GC_INIT_WITH_MASK);
	
	//maskcreated = 1;
	return msk;
}
