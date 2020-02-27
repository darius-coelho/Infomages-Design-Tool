#ifndef SEGMASK_H
#define SEGMASK_H

#include "ui_segMask.h"
#include "sceneseg.h"
#include <QtWidgets/qdialog.h>
#include <qmessagebox.h>
#include <QFontDialog>
#include <QColorDialog>

#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>



class segMask : public QDialog
{
	Q_OBJECT

public:
	segMask(QWidget *parent);
	~segMask();
	SceneSeg* scene_seg;
	
private:
	Ui::segMask ui;
	cv::Mat backdrop;

	//Window Handling
	int m_nMouseClick_X_Coordinate;
	int m_nMouseClick_Y_Coordinate;
	///Functions
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

public slots:
	void onNewImageLoaded(cv::Mat bckdrp);

private slots:
	void on_pushButton_forebox_clicked();
	void on_pushButton_foreground_clicked();
	void on_pushButton_background_clicked();
	void on_pushButton_refine_clicked();
	void on_pushButton_mask_clicked();	
	void on_pushButton_close_clicked();

signals:
	void sendMask(cv::Mat mask);
};

#endif // SEGMASK_H
