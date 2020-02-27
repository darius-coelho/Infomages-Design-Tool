#include "titleTextItem.h"

TitleTextItem::TitleTextItem(const QString & text, QGraphicsItem * parent) : QGraphicsTextItem(text, parent)
{
	titleRect = new QGraphicsRectItem(this->boundingRect());

	titleRect->setX(this->x());
	titleRect->setY(this->y());
	titleRect->setZValue(999);
	titleRect->setBrush(QBrush(Qt::white));
	titleRect->setPen(QPen(Qt::NoPen));
}

void TitleTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	
}

void TitleTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsTextItem::mouseMoveEvent(event);
	
	titleRect->setX(this->x());
	titleRect->setY(this->y());
	titleRect->setRect(this->boundingRect());	
}

QGraphicsRectItem* TitleTextItem::getRect()
{
	return titleRect;
}

void TitleTextItem::updateFont(QFont font)
{
	this->setFont(font);
	titleRect->setX(this->x());
	titleRect->setY(this->y());
	titleRect->setRect(this->boundingRect());
}

void TitleTextItem::keyPressEvent(QKeyEvent * event)
{
	QGraphicsTextItem::keyPressEvent(event);
	titleRect->setX(this->x());
	titleRect->setY(this->y());
	titleRect->setRect(this->boundingRect());
}

void TitleTextItem::setFillColor(QColor color)
{
	titleRect->setBrush(QBrush(color));	
}

QColor TitleTextItem::getFillColor()
{
	return titleRect->brush().color();
}