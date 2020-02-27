#include "segMask.h"

segMask::segMask(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	scene_seg = new SceneSeg(this);
	connect(this, SIGNAL(sendMask(cv::Mat)),
		this->parent(), SLOT(recieveMask(cv::Mat)));
	scene_seg->setMode(SceneSeg::DrawRect);	
	ui.label_2->setVisible(true);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(false);
}

segMask::~segMask()
{
	scene_seg = 0;
}

cv::Mat qimage_to_mat_refs(QImage &im, int format)
{
	return cv::Mat(im.height(), im.width(), format, im.bits(), im.bytesPerLine());
}

QImage mat_to_qimage_refs(cv::Mat &mat, QImage::Format format)
{
	return QImage(mat.data, mat.cols, mat.rows, mat.step, format);
}

void segMask::onNewImageLoaded(cv::Mat bckdrp)
{
	bckdrp.copyTo(backdrop);	
	QPixmap pix_backdrop = QPixmap::fromImage(mat_to_qimage_refs(backdrop, QImage::Format_RGB888).rgbSwapped());
	scene_seg->setOrigBackdrop(backdrop, pix_backdrop);
		
	//Scene stuff
	ui.graphicsView_seg->setScene(scene_seg);
}

void segMask::on_pushButton_forebox_clicked()
{	
	scene_seg->setMode(SceneSeg::DrawRect);
	ui.label_2->setVisible(true);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(false);
}

void segMask::on_pushButton_foreground_clicked()
{
	scene_seg->setMode(SceneSeg::DrawLineFG);
	ui.label_2->setVisible(false);
	ui.label_3->setVisible(true);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(false);
}

void segMask::on_pushButton_background_clicked()
{
	scene_seg->setMode(SceneSeg::DrawLineBG);
	ui.label_2->setVisible(false);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(true);
	ui.label_5->setVisible(false);
}

void segMask::on_pushButton_refine_clicked()
{
	scene_seg->setMode(SceneSeg::SelectObject);
	ui.label_2->setVisible(false);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(true);
}

void segMask::on_pushButton_mask_clicked()
{
	scene_seg->setMode(SceneSeg::SelectObject);
	ui.label_2->setVisible(false);
	ui.label_3->setVisible(false);
	ui.label_4->setVisible(false);
	ui.label_5->setVisible(true);

	cv::Mat mask;
	scene_seg->segment().copyTo(mask);
	emit this->sendMask(mask);

	cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
	cv::threshold(mask, mask, 100, 255, cv::THRESH_BINARY_INV);
	cv::Mat segRes(backdrop.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	backdrop.copyTo(segRes, mask);
	segRes = segRes / 4;
	cv::threshold(mask, mask, 100, 255, cv::THRESH_BINARY_INV);
	backdrop.copyTo(segRes, mask);

	QPixmap pix_seg = QPixmap::fromImage(mat_to_qimage_refs(segRes, QImage::Format_RGB888));
	scene_seg->refreshBackrop(pix_seg);
	
}

void segMask::on_pushButton_close_clicked()
{
	this->hide();
	delete scene_seg;
	scene_seg = new SceneSeg(this);
}

void segMask::mousePressEvent(QMouseEvent *event) 
{
	m_nMouseClick_X_Coordinate = event->x();
	m_nMouseClick_Y_Coordinate = event->y();	
}

void segMask::mouseMoveEvent(QMouseEvent *event) 
{
	if (m_nMouseClick_Y_Coordinate < 30)
	{
		int moveX = event->globalX() - m_nMouseClick_X_Coordinate;
		int moveY = event->globalY() - m_nMouseClick_Y_Coordinate;
		if (moveX < 0 & moveY < 0)
		{
			move(0, 0);
		}
		else if (moveX < 0)
		{
			move(0, moveY);
		}
		else if (moveY < 0)
		{
			move(moveX, 0);
		}
		else
		{
			move(moveX, moveY);
		}
	}
}