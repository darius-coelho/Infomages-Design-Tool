#ifndef TITLETEXTITEM_H
#define TITLETEXTITEM_H

#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <qmessagebox.h>
#include <qpen.h>


class TitleTextItem : public QGraphicsTextItem
{
public:
	TitleTextItem(const QString & text, QGraphicsItem * parent = 0);
	QGraphicsRectItem* getRect();
	void updateFont(QFont font);
	void setFillColor(QColor color);
	QColor getFillColor();

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void keyPressEvent(QKeyEvent * event);

private:
	QGraphicsRectItem * titleRect;

};



#endif // TITLETEXTITEM_H