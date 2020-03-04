#include "infomager.h"
#include <QValueAxis>
#include <Windows.h>
//QMessageBox::information(0, "info", QString("Popup message"));


Infomager::Infomager(QWidget *parent)
	: QMainWindow(parent)
{
	write_text_to_log_file("InfomagerStart\n");
	ui.setupUi(this);
	segM = new segMask(this);
	segM->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	segM->hide();

	distH = new distortionHelp(this);
	distH->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
	distH->hide();

	scene_infomage = new SceneInfomage(this);
	scene_preview = new QGraphicsScene(this);
	
	loaded_img = false;
	dataLoaded = false;
	maskSet = false;

	chart = 0;
	chartTitle = 0;
	chartType = Pie;
	chartX = 10;
	chartY = 10;

	//Pie Chart Init
	pieType = Rad;
	divType = Radial;
	pieRot = 0;
	ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieArc->setStyleSheet("background: rgb(65, 65, 65);");

	ui.label_fillRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_fillHori->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillVerti->setStyleSheet("background: rgb(65, 65, 65);");

	pieColorButtons.push_back(ui.pushButton_pieColor_1);
	pieColorButtons.push_back(ui.pushButton_pieColor_2);
	pieColorButtons.push_back(ui.pushButton_pieColor_3);
	pieColorButtons.push_back(ui.pushButton_pieColor_4);
	pieColorButtons.push_back(ui.pushButton_pieColor_5);
	pieColorButtons.push_back(ui.pushButton_pieColor_6);
	pieColorButtons.push_back(ui.pushButton_pieColor_7);
	pieColorButtons.push_back(ui.pushButton_pieColor_8);
	pieColorButtons.push_back(ui.pushButton_pieColor_9);
	pieColorButtons.push_back(ui.pushButton_pieColor_10);
	pieColorButtons.push_back(ui.pushButton_pieColor_11);
	pieColorButtons.push_back(ui.pushButton_pieColor_12);
	pieColorButtons.push_back(ui.pushButton_pieColor_13);
	pieColorButtons.push_back(ui.pushButton_pieColor_14);
	pieColorButtons.push_back(ui.pushButton_pieColor_15);

	for (int i = 0;i < pieColorButtons.size();i++)
	{
		pieColorButtons[i]->setVisible(true);
	}
	
	//Line Chart Init
	lineThick = 2;
	lineColor = QRgb(0x3675db);
	lineGlowThick = 4;
	lineGlowColor = QRgb(0x000000);
	lineGlowColor.setAlpha(150);
	xStep = 1;
	lineSpline = false;
	ui.label_lineLine->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_lineSpline->setStyleSheet("background: rgb(65, 65, 65);");

	//Bar Chart Init
	barType = BarHeight;
	barDirection = BarVerti;
	ui.label_barArea->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_barLength->setStyleSheet("background: rgb(240, 240, 240);");
	barThick = 1;
	barColor = QRgb(0x3675db);
	barGlowThick = 1;
	barGlowColor = QRgb(0x000000);
	barGlowColor.setAlpha(150);
	barOverlay = true;
	barPercentage = true;
	//ui.label_barPercentage->setStyleSheet("background: rgb(240, 240, 240);");
	//ui.label_barValue->setStyleSheet("background: rgb(65, 65, 65);");
	ui.groupBox_BarFill->setVisible(true);
	ui.groupBox_BarOverlay->setVisible(true);

	ui.graphicsView_infomage->setScene(scene_infomage);
	ui.graphicsView_chart->setScene(scene_preview);

	ui.comboBoxPiFiltImg_2->addItem("None");
	ui.comboBoxPiFiltImg_2->addItem("Stylization");
	ui.comboBoxPiFiltImg_2->addItem("Smooth");
	ui.comboBoxPiFiltImg_2->addItem("Grayscale");	

	ui.label_pieFill->setVisible(true);
	ui.label_barFill->setVisible(false);
	ui.label_lineOverlay->setVisible(true);
	ui.label_barOverlay->setVisible(false);

	ui.groupBox_PieFill->setVisible(true);
	ui.groupBox_BarFill->setVisible(false);
	ui.label_lineOverlay->setVisible(true);
	ui.label_barOverlay->setVisible(false);
	
	ui.groupBox_fill->setVisible(true);
	ui.label_FillStyle->setVisible(true);
	ui.groupBox_overlay->setVisible(false);
	ui.label_OverlayStyle->setVisible(false);

	ui.label_fontColor->setStyleSheet("background: rgb(0, 0, 0);");
	ui.label_fontFill->setStyleSheet("background: rgb(255, 255, 255);");
	ui.label_lineColor->setStyleSheet("background: rgb(54, 117, 219);");
	ui.label_lineGlowColor->setStyleSheet("background: rgb(0, 0, 0);");
	ui.label_lineXColor->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_lineYColor->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_barColor->setStyleSheet("background: rgb(54, 117, 219);");
	ui.label_barMaskColor->setStyleSheet("background: rgb(54, 117, 219);");
	ui.label_barGlowColor->setStyleSheet("background: rgb(0, 0, 0);");
	ui.label_barXColor->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_barYColor->setStyleSheet("background: rgb(240, 240, 240);");

	ui.graphicsView_infomage->setRenderHints(QPainter::HighQualityAntialiasing);
	ui.graphicsView_chart->setRenderHints(QPainter::Antialiasing);


	//Setup Thubnail list
	ui.listWidget->setMovement(QListView::Static);
	ui.listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.listWidget->setViewMode(QListView::IconMode);
	ui.listWidget->setWrapping(false);
	ui.listWidget->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal {border: none;background: rgb(104,104,104);height: 15px;margin: 0px 30px 0px 30px;} QScrollBar::handle:horizontal{background: gray; min-width: 20px;} QScrollBar::add-line:horizontal{background: none; border:none;} QScrollBar::sub-line:horizontal{background: none; border:none; } QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal{background: none;}");

	move(100, 100);

	connect(this, SIGNAL(sendBackDrop(cv::Mat)), segM, SLOT(onNewImageLoaded(cv::Mat)));	
	connect(this, SIGNAL(sendDistortedVals(std::vector<std::string>, std::vector<double>, std::vector<double>)), distH, SLOT(onValsLoaded(std::vector<std::string>, std::vector<double>, std::vector<double>)));
}

Infomager::~Infomager()
{

}

//Image Loading
cv::Mat qimage_to_mat_ref(QImage &im, int format)
{
	return cv::Mat(im.height(), im.width(), format, im.bits(), im.bytesPerLine());
}

QImage mat_to_qimage_ref(cv::Mat &mat, QImage::Format format)
{
	return QImage(mat.data, mat.cols, mat.rows, mat.step, format);
}

void Infomager::on_loadButtonImg_clicked()
{
	write_text_to_log_file("LoadImg\n");
	//Loading file
	QString fname = QFileDialog::getOpenFileName(this, tr("Open File"), "C:/Users/Darius/Documents/Infomager/", "Image files (*.png; *.jpg);");
	
	if (fname != NULL)
	{
		filt.setOrigBackdrop(load_img(fname.toLatin1().data()));
		img = load_img(fname.toLatin1().data());
		img.copyTo(filtimg);
		cv::Mat tmp;
		img.copyTo(tmp);

		QPixmap pix_backdrop;
		if (img.cols > ui.graphicsView_infomage->width() || img.rows > ui.graphicsView_infomage->height())
		{
			pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(tmp, QImage::Format_RGB888).rgbSwapped().scaled(ui.graphicsView_infomage->width(), ui.graphicsView_infomage->height(), Qt::KeepAspectRatio));
		}
		else
		{
			pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(tmp, QImage::Format_RGB888).rgbSwapped());
		}
		
		imgW = pix_backdrop.width();
		imgH = pix_backdrop.height();
		
		ui.spinBox_barChartWidth->setValue(imgW - imgW/3);
		ui.spinBox_lineChartWidth->setValue(imgW - imgW/3);
		ui.spinBox_barChartHeight->setValue(imgH - imgH/2);
		ui.spinBox_lineChartHeight->setValue(imgH - imgH/2);

		scene_infomage->setOrigBackdrop(load_img(fname.toLatin1().data()), pix_backdrop);
		loaded_img = true;

		if (chartType == Line)
		{
			if (dataLoaded)
			{
				this->drawLineChart();			
				linearReg();
			}
		}
		if (chartType == Pie & pieType == OverLay)
		{
			if (dataLoaded)
			{
				this->drawOverlayPieChart();
				findHoughCircle();
			}
		}
	}	
}

void Infomager::imageSearch()
{
	//return;
	QString val;
	ui.listWidget->clear();
	imgLinks.clear();
	//****************Make Google Image Search Request****************
	// create custom temporary event loop on stack
	QEventLoop eventLoopGoggle;

	// "quit()" the event-loop, when the network request "finished()"
	QNetworkAccessManager mgrGoogle;
	QObject::connect(&mgrGoogle, SIGNAL(finished(QNetworkReply*)), &eventLoopGoggle, SLOT(quit()));

	// the HTTP request
	QString query = QString::fromStdString(t_title);
	query.replace(" ", "+");	
	//QNetworkRequest reqGoogle(QUrl(QString("Add you google api link here").arg(query)));	
	QNetworkReply *replyGoogle = mgrGoogle.get(reqGoogle);
	eventLoopGoggle.exec(); // blocks stack until "finished()" has been called

	if (replyGoogle->error() == QNetworkReply::NoError) {
		//HTTP request success		
		val = QString(replyGoogle->readAll());		
		delete replyGoogle;
	}
	else {
		//HTTP request failure
		QMessageBox::information(0, "info", QString("Google Request Failed: %1").arg(replyGoogle->errorString()));
		delete replyGoogle;
		return;
	}
	
	//****************READ JSON REPLY****************
	
	//Load entire JSON Object:
	QJsonDocument resDoc = QJsonDocument::fromJson(val.toUtf8());
	QJsonObject resObj = resDoc.object();
	
	//Select results(items):
	QJsonValue itemsVal = resObj.value(QString("items"));
	QJsonArray itemsArr = itemsVal.toArray();

	//For each result:
	for (int i = 0; i < itemsArr.size(); i++)
	{
		QJsonValue val0 = itemsArr[i];
		QJsonObject obj0 = itemsArr[i].toObject();
		QJsonObject obj00 = obj0.value(QString("image")).toObject();
					
		// create custom temporary event loop on stack
		QEventLoop eventLoop;

		// "quit()" the event-loop, when the network request "finished()"
		QNetworkAccessManager mgr;
		QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

		// the HTTP request
		QNetworkRequest req(QUrl(obj00.value(QString("thumbnailLink")).toString()));
		req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");
		QNetworkReply *reply = mgr.get(req);
		eventLoop.exec(); // blocks stack until "finished()" has been called

		if (reply->error() == QNetworkReply::NoError) {			
			//Store the link to the image.
			imgLinks.append(obj0.value(QString("link")).toString());
			
			//HTTP request success
			QPixmap thumbImg;
			if (thumbImg.loadFromData(reply->readAll()))
			{
				//Add the images to the thumbnail view
				QIcon ButtonIcon(thumbImg);
				ui.listWidget->setIconSize(QSize(180, 180));
				ui.listWidget->addItem(new QListWidgetItem(ButtonIcon, ""));
			}		
			delete reply;
		}
		else {
			//Store the error to the image list.
			imgLinks.append(reply->errorString());

			//HTTP request failure
			//QPixmap thumbImg("C:/404.png");
			//Add the images to the thumbnail view
			//QIcon ButtonIcon(thumbImg);
			ui.listWidget->setIconSize(QSize(180, 180));
			//ui.listWidget->addItem(new QListWidgetItem(ButtonIcon, "ERROR"));
			//QMessageBox::information(0, "info", QString("Failed %1").arg(reply->errorString()));
			delete reply;
		}
	}
}

void Infomager::on_listWidget_itemClicked(QListWidgetItem * item)
{
	write_text_to_log_file("SelectImg\n");
	if (item->text() == "ERROR")
	{		
		int index = ui.listWidget->row(item);
		QMessageBox::information(0, "info", QString("Error: %1").arg(imgLinks[index]));
	}
	else
	{
		int index = ui.listWidget->row(item);
		// create custom temporary event loop on stack
		QEventLoop eventLoop;

		// "quit()" the event-loop, when the network request "finished()"
		QNetworkAccessManager mgr;
		QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

		// the HTTP request
		QNetworkRequest req(QUrl(imgLinks.at(index)));
		req.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36");
		QNetworkReply *reply = mgr.get(req);
		eventLoop.exec(); // blocks stack until "finished()" has been called

		if (reply->error() == QNetworkReply::NoError) 
		{
			//HTTP request success
			QPixmap tmpImg;
			if (tmpImg.loadFromData(reply->readAll()))
			{
				if (tmpImg.width() > ui.graphicsView_infomage->width() || tmpImg.height() > ui.graphicsView_infomage->height())
				{
					tmpImg = tmpImg.scaled(ui.graphicsView_infomage->width(), ui.graphicsView_infomage->height(), Qt::KeepAspectRatio);
				}				
				QImage tmp;
				tmp = tmpImg.toImage().convertToFormat(QImage::Format_RGB888).rgbSwapped();				
				filt.setOrigBackdrop(qimage_to_mat_ref(tmp, CV_8UC3));				
				qimage_to_mat_ref(tmp, CV_8UC3).copyTo(img);
				img.copyTo(filtimg);

				imgW = tmpImg.width();
				imgH = tmpImg.height();

				ui.spinBox_barChartWidth->setValue(imgW - imgW / 3);
				ui.spinBox_lineChartWidth->setValue(imgW - imgW / 3);
				ui.spinBox_barChartHeight->setValue(imgH - imgH / 2);
				ui.spinBox_lineChartHeight->setValue(imgH - imgH / 2);

				scene_infomage->setOrigBackdrop(img, tmpImg);
				scene_infomage->setSceneRect(0, 0, imgW, imgH);
				loaded_img = true;	

				if (chartType == Line)
				{
					if (dataLoaded & loaded_img)
					{
						linearReg();
					}
				}
				if (chartType == Pie & pieType == OverLay)
				{
					if (dataLoaded)
					{
						this->drawOverlayPieChart();
						findHoughCircle();
					}
				}
			}
			delete reply;			
		}
		else 
		{
			QMessageBox::information(0, "info", QString("Failed: %1").arg(reply->errorString()));
			delete reply;
		}
	}		
}

//Meme Saving
void Infomager::on_pushButton_save_clicked()
{
	write_text_to_log_file("SaveMeme\n");
	if (loaded_img && dataLoaded)
	{
		QString fileName = QFileDialog::getSaveFileName(this, "Save image", "C:/Users/Darius/Documents/Infomager/", "PNG (*.png);;JPEG (*.JPEG)");
		if (!fileName.isNull())
		{
			QImage image(scene_infomage->sceneRect().size().toSize(), QImage::Format_ARGB32);
			QPainter painter(&image);
			painter.setRenderHint(QPainter::Antialiasing);
			scene_infomage->render(&painter);
			image.save(fileName);
		}
	}
}

//Data Loading
void Infomager::on_loadButtonData_clicked()
{
	lineYMax = -INFINITY;
	lineYMin = INFINITY;
	//Get file name (CSV only)
	QString fname = QFileDialog::getOpenFileName(this, tr("Oen File"), "C:/Users/Darius/Documents/Infomager/", "CSV files (*.csv);");
	QFileInfo info1(fname);
	if (fname == NULL)
	{
		//No file selected
		write_text_to_log_file("OpenDataFileNone\n");
		QMessageBox::information(0, "info", "A file was not selected.");
	}
	else
	{
		write_text_to_log_file("OpenDataFile\n");
		ui.tableWidget->clear();
		ui.tableWidget->setRowCount(0);
		//Read CSV File
		t_title = info1.baseName().toStdString();
		CsvParser *csvparser = CsvParser_new(fname.toLatin1().data(), ",", 1);
		CsvRow *header;
		CsvRow *row;

		header = CsvParser_getHeader(csvparser);
		
		if (header == NULL) {
			QMessageBox::information(0, "info", CsvParser_getErrorMessage(csvparser));
		}		

		char **headerFields = CsvParser_getFields(header);
		if (CsvParser_getNumFields(header) > 2)
		{
			QMessageBox::information(0, "info", "Invalid Data. Data must have only two attributes");
		}
		else
		{
			t_attr.resize(CsvParser_getNumFields(header));
			ui.tableWidget->setColumnCount(CsvParser_getNumFields(header));
			QStringList headerLst;
			for (int i = 0; i < CsvParser_getNumFields(header); i++)
			{
				t_attr[i] = headerFields[i];
				headerLst.append(headerFields[i]);
			}

			ui.tableWidget->setHorizontalHeaderLabels(headerLst);
			CsvParser_destroy_row(header);
			
			int row_no = 0;
			
			while ((row = CsvParser_getRow(csvparser)))
			{
				char **rowFields = CsvParser_getFields(row);
				t_val.resize(row_no + 1);
				t_label.resize(row_no + 1);
				ui.tableWidget->insertRow(row_no);
				for (int i = 0; i < CsvParser_getNumFields(row); i++)
				{
					ui.tableWidget->setRowHeight(row_no, 20);
					ui.tableWidget->setItem(row_no, i, new QTableWidgetItem(QString(rowFields[i])));
				}
				t_label[row_no] = rowFields[0];
				t_val[row_no] = atof(rowFields[1]);				

				if (lineYMin > t_val[row_no])
				{
					lineYMin = t_val[row_no];
					barYMin = t_val[row_no];
				}
				if (lineYMax < t_val[row_no])
				{
					lineYMax = t_val[row_no];
					barYMax = t_val[row_no];
				}				
				row_no++;
				CsvParser_destroy_row(row);
			}
		}
		ui.doubleSpinBox_yMax->setValue(lineYMax);
		ui.doubleSpinBox_yMin->setValue(lineYMin);
		ui.doubleSpinBox_yMaxBarFill->setValue(barYMax);
		ui.doubleSpinBox_yMinBarFill->setValue(barYMin);
		ui.doubleSpinBox_yMaxBarOverlay->setValue(barYMax);
		ui.doubleSpinBox_yMinBarOverlay->setValue(barYMin);

		dataLoaded = true;
		ui.comboBoxBrewerNames->clear();
		if (t_val.size() < 20)
		{
			for (int i = 0; i < brewerNames.size(); i++)
			{
				for (int j = 0;j < brewer[i].size();j++)
				{
					if (brewer[i][j].size() == t_val.size())
					{
						ui.comboBoxBrewerNames->addItem(brewerNames[i]);
					}
				}
			}
			for (int i = 0;i < pieColorButtons.size();i++)
			{
				pieColorButtons[i]->setVisible(true);
			}
			for (int i = 0; i < t_val.size(); i++)
			{
				pieColorButtons[i]->setVisible(true);
				pieColorButtons[i]->setText(QString::number(i + 1));
				pieColorButtons[i]->setToolTip(QString::fromStdString(t_label[i]));
			}
		}
		//Draw Initial chart if Bar or Line is selected
		if (chartType == Bar)
		{
			this->drawBarChart();
		}
		else if(chartType == Line)
		{
			this->drawLineChart();
			if (loaded_img)
			{
				linearReg();
			}			
		}
		else if (chartType == Pie)
		{
			if (pieType == OverLay)
			{
				if (dataLoaded)
				{
					this->drawOverlayPieChart();
					findHoughCircle();
				}
			}
			else {
				this->drawPieChart();
			}
			
		}
		this->imageSearch();
	}
	//Create the chart title
	if (!chartTitle)
	{
		chartTitle = new TitleTextItem(QString::fromStdString(t_title));
		chartTitle->setZValue(1000);
		chartTitle->setFlag(QGraphicsItem::ItemIsSelectable, true);
		chartTitle->setFlag(QGraphicsItem::ItemIsMovable, true);
		chartTitle->setCursor(Qt::SizeAllCursor);
		chartTitle->setTextInteractionFlags(Qt::TextEditorInteraction);		
		scene_infomage->addItem(chartTitle);
		scene_infomage->addItem(chartTitle->getRect());		
	}
	else
	{
		chartTitle->setPlainText(QString::fromStdString(t_title));
	}	
}

void  Infomager::linearReg()
{
	//***************Linear Regression***************
	double xsum = 0;
	double ysum = 0;
	double x2sum = 0;
	double xysum = 0;
	int n = t_val.size();
	for (int i = 0; i<n; i++)
	{
		double x = chart->mapToPosition(QPointF(i, t_val[i]), lineSeries).x();
		double y = chart->mapToPosition(QPointF(i, t_val[i]), lineSeries).y();
		xsum = xsum + x;
		ysum = ysum + y;
		x2sum = x2sum + pow(x, 2);
		xysum = xysum + x * y;
	}
	double slope =  -(n*xysum - xsum*ysum) / (n*x2sum - xsum*xsum);
	

	//***************Hough Lines***************
	cv::Mat dst, color_dst;
	
	cv::Canny(img, dst, 28, 240, 3);
	cvtColor(dst, color_dst, CV_GRAY2BGR);
	
	std::vector<cv::Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100);
	if (lines.size() > 0)
	{
		double tmp = INFINITY;
		int idx = 0;
		for (int i = 0; i < lines.size(); i++)
		{
			double tmpSlp = cos(lines[i][1]) / sin(lines[i][1]);
			if (abs(tmpSlp - slope) < tmp)
			{
				tmp = abs(tmpSlp - slope);
				idx = i;
			}
		}

		float rho = lines[idx][0];
		float theta = lines[idx][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		double m = -a / b;
		double c = (y0 - m*x0);
		double xTop = -c / m;
		double yTop = 0;
		double xBot = (imgH - c) / m;
		double yBot = imgH;
		double xLeft = 0;
		double yLeft = c;
		double xRight = imgW;
		double yRight = m*imgW + c;

		std::vector<CvPoint> pts;
		if (xTop >= 0 && xTop <= imgW)
		{
			pts.push_back(cv::Point(xTop, yTop));
		}
		if (xBot >= 0 && xBot <= imgW)
		{
			pts.push_back(cv::Point(xBot, yBot));
		}
		if (yLeft >= 0 && yLeft <= imgH)
		{
			pts.push_back(cv::Point(xLeft, yLeft));
		}
		if (yRight >= 0 && yRight <= imgH)
		{
			pts.push_back(cv::Point(xRight, yRight));
		}

		double MidX = (pts[0].x + pts[1].x) / 2;
		double MidY = (pts[0].y + pts[1].y) / 2;

		//cv::line(color_dst, pts[0], pts[1], cv::Scalar(0, 255, 0), 3, 8);	
		//cv::namedWindow("Detected Lines", 1);
		//cv::imshow("Detected Lines", color_dst);

		chartX = MidX - (ui.spinBox_lineChartWidth->value() / 2);
		chartY = MidY - (ui.spinBox_lineChartHeight->value() / 2);

		chart->setPos(QPoint(chartX, chartY));
	}
}

//Image Filtering
void Infomager::on_comboBoxPiFiltImg_2_currentIndexChanged(const QString & text)
{
	if (loaded_img)
	{
		if (text == "None")
		{
			write_text_to_log_file("FilterNone\n");
			filt.none().copyTo(filtimg);
			scene_infomage->setCurrentBackdrop(filtimg);
			QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
			scene_infomage->refreshBackrop(filt_backdrop);
			if (maskSet && chartType == Pie)
			{
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
			}
			if (!barOverlay && chartType == Bar)
			{
				scene_infomage->setCurrentBackdrop(filtimg);
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
				scene_infomage->removePieLabels();
				for (int i = 0; i < barMasksSet.size(); i++)
				{
					if (barMasksSet[i])
					{
						this->fillBarChart(i);
					}
				}
				QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(filt_backdrop);
			}
		}
		else if (text == "Stylization")
		{
			write_text_to_log_file("FilterStylize\n");
			filt.styliz().copyTo(filtimg);
			scene_infomage->setCurrentBackdrop(filtimg);
			QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
			scene_infomage->refreshBackrop(filt_backdrop);
			if (maskSet && chartType == Pie)
			{				
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
			}
			if (!barOverlay && chartType == Bar)
			{
				scene_infomage->setCurrentBackdrop(filtimg);
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
				scene_infomage->removePieLabels();
				for (int i = 0; i < barMasksSet.size(); i++)
				{
					if (barMasksSet[i])
					{
						this->fillBarChart(i);
					}
				}
				QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(filt_backdrop);
			}
		}
		else if (text == "Grayscale")
		{
			write_text_to_log_file("FilterGray\n");
			filt.grayscale().copyTo(filtimg);
			scene_infomage->setCurrentBackdrop(filtimg);
			QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
			scene_infomage->refreshBackrop(filt_backdrop);
			if (maskSet && chartType == Pie)
			{
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
			}
			if (!barOverlay && chartType == Bar)
			{
				scene_infomage->setCurrentBackdrop(filtimg);
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
				scene_infomage->removePieLabels();
				for (int i = 0; i < barMasksSet.size(); i++)
				{
					if (barMasksSet[i])
					{
						this->fillBarChart(i);
					}
				}
				QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(filt_backdrop);
			}
		}
		else if (text == "Smooth")
		{
			write_text_to_log_file("FilterSmooth\n");
			filt.smooth().copyTo(filtimg);
			scene_infomage->setCurrentBackdrop(filtimg);
			QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
			scene_infomage->refreshBackrop(filt_backdrop);
			if (maskSet && chartType == Pie)
			{
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
			}
			if (!barOverlay && chartType == Bar)
			{
				scene_infomage->setCurrentBackdrop(filtimg);
				QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(pix_viz);
				scene_infomage->removePieLabels();
				for (int i = 0; i < barMasksSet.size(); i++)
				{
					if (barMasksSet[i])
					{
						this->fillBarChart(i);
					}
				}
				QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
				scene_infomage->refreshBackrop(filt_backdrop);
			}
		}		
	}
}

void Infomager::on_pushButton_clear_clicked()
{
	write_text_to_log_file("Clear\n");
	scene_infomage->clearBack();
}

//Text Editing
void Infomager::on_pushButton_fontColor_clicked()
{	
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontColorTitle\n");
		QColor color = QColorDialog::getColor(chartTitle->defaultTextColor(), this, "Text Color");
		if (color.isValid())
		{
			QString style = "background: rgb(%1, %2, %3);";
			style = style.arg(color.red()).arg(color.green()).arg(color.blue());
			ui.label_fontColor->setStyleSheet(style);
			chartTitle->setDefaultTextColor(color);
		}
	}
	else if (chart->isSelected())
	{
		write_text_to_log_file("FontColorChart\n");
		QColor color = QColorDialog::getColor(chart->axisX()->labelsColor(), this, "Text Color");
		if (color.isValid())
		{
			QString style = "background: rgb(%1, %2, %3);";
			style = style.arg(color.red()).arg(color.green()).arg(color.blue());
			ui.label_fontColor->setStyleSheet(style);
			chart->axisX()->setLabelsColor(color);
			chart->axisY()->setLabelsColor(color);
		}
	}
	else
	{
		write_text_to_log_file("FontColorOther\n");
		QList<QGraphicsItem *> items = scene_infomage->selectedItems();
		if (items.count() > 0)
		{
			QColor color = Qt::black;
			if (items[0]->type() == QGraphicsTextItem::Type)
			{
				QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[0];
				color = QColorDialog::getColor(tmp->defaultTextColor(), this, "Text Color");
			}			
			for (int i = 0; i < items.count(); ++i)
			{
				if (items[i]->type() == QGraphicsTextItem::Type)
				{
					QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[i];
					QString style = "background: rgb(%1, %2, %3);";
					style = style.arg(color.red()).arg(color.green()).arg(color.blue());
					ui.label_fontColor->setStyleSheet(style);
					tmp->setDefaultTextColor(color);
				}
			}
		}
	}
}

void Infomager::on_pushButton_fontFill_clicked()
{
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontFillTitle\n");
		QColor color = QColorDialog::getColor(chartTitle->getFillColor(), this, "Text Color");
		if (color.isValid())
		{
			QString style = "background: rgb(%1, %2, %3);";
			style = style.arg(color.red()).arg(color.green()).arg(color.blue());
			ui.label_fontFill->setStyleSheet(style);
			color.setAlpha(ui.horizontalSlider_textFillOpac->value());
			chartTitle->setFillColor(color);
		}
	}
}

void Infomager::on_pushButton_fontItalics_clicked()
{
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontItalicsTitle\n");
		if (textFont.italic())
		{
			textFont.setItalic(false);
		}
		else
		{
			textFont.setItalic(true);
		}
		chartTitle->updateFont(textFont);
	}
	else if (chart->isSelected())
	{
		write_text_to_log_file("FontItalicsChart\n");
		if (textFont.italic())
		{
			textFont.setItalic(false);
		}
		else
		{
			textFont.setItalic(true);
		}
		chart->axisX()->setLabelsFont(textFont);
		chart->axisY()->setLabelsFont(textFont);
	}
	else
	{
		write_text_to_log_file("FontItalicsOther\n");
		if (textFont.italic())
		{
			textFont.setItalic(false);
		}
		else
		{
			textFont.setItalic(true);
		}
		QList<QGraphicsItem *> items = scene_infomage->selectedItems();
		
		for (int i = 0; i<items.count(); ++i)
		{
			if (items[i]->type() == QGraphicsTextItem::Type)
			{
				QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[i];
				tmp->setFont(textFont);
			}
		}
	}
}

void Infomager::on_pushButton_fontBold_clicked()
{
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontBoldTitle\n");
		if (textFont.bold())
		{
			textFont.setBold(false);
		}
		else
		{
			textFont.setBold(true);
		}
		chartTitle->updateFont(textFont);
	}
	else if (chart->isSelected() & chartType != Pie & barOverlay)
	{
		write_text_to_log_file("FontBoldChart\n");
		if (textFont.bold())
		{
			textFont.setBold(false);
		}
		else
		{
			textFont.setBold(true);
		}
		chart->axisX()->setLabelsFont(textFont);
		chart->axisY()->setLabelsFont(textFont);
	}
	else
	{
		write_text_to_log_file("FontBoldOther\n");
		if (textFont.bold())
		{
			textFont.setBold(false);
		}
		else
		{
			textFont.setBold(true);
		}
		QList<QGraphicsItem *> items = scene_infomage->selectedItems();
		
		for (int i = 0; i<items.count(); ++i)
		{
			if (items[i]->type() == QGraphicsTextItem::Type)
			{
				QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[i];
				tmp->setFont(textFont);
			}
		}
	}
}

void Infomager::on_fontComboBox_fontFamily_currentFontChanged()
{	
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontFamilyTitle\n");
		textFont.setFamily(ui.fontComboBox_fontFamily->currentText());
		chartTitle->updateFont(textFont);
	}
	else if (chart->isSelected())
	{
		write_text_to_log_file("FontFamilyChart\n");
		textFont.setFamily(ui.fontComboBox_fontFamily->currentText());
		chart->axisX()->setLabelsFont(textFont);
		chart->axisY()->setLabelsFont(textFont);
	}
	else
	{
		write_text_to_log_file("FontFamilyOther\n");
		QList<QGraphicsItem *> items = scene_infomage->selectedItems();

		for (int i = 0; i<items.count(); ++i)
		{
			if (items[i]->type() == QGraphicsTextItem::Type)
			{
				QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[i];
				textFont.setFamily(ui.fontComboBox_fontFamily->currentText());
				tmp->setFont(textFont);
			}
		}
	}
}

void Infomager::on_spinBox_fontSize_valueChanged()
{
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontSizeTitle\n");
		textFont.setPointSize(ui.spinBox_fontSize->value());
		chartTitle->updateFont(textFont);
	}
	else if (chart->isSelected())
	{	
		write_text_to_log_file("FontSizeChart\n");
		textFont.setPointSize(ui.spinBox_fontSize->value());
		chart->axisX()->setLabelsFont(textFont);
		chart->axisY()->setLabelsFont(textFont);
	}
	else
	{	
		write_text_to_log_file("FontSizeOther\n");
		QList<QGraphicsItem *> items = scene_infomage->selectedItems();

		for (int i = 0; i<items.count(); ++i)
		{
			if (items[i]->type() == QGraphicsTextItem::Type)
			{
				QGraphicsTextItem* tmp = (QGraphicsTextItem*)items[i];
				textFont.setPointSize(ui.spinBox_fontSize->value());
				tmp->setFont(textFont);
			}
		}
	}
}

void Infomager::on_horizontalSlider_textFillOpac_valueChanged()
{
	if (chartTitle->isSelected())
	{
		write_text_to_log_file("FontBackroundOpacTitle\n");
		QColor color = chartTitle->getFillColor();
		if (color.isValid())
		{
			QString style = "background: rgb(%1, %2, %3);";
			style = style.arg(color.red()).arg(color.green()).arg(color.blue());
			ui.label_fontFill->setStyleSheet(style);
			color.setAlpha(ui.horizontalSlider_textFillOpac->value());
			chartTitle->setFillColor(color);
		}
	}	
}
 
//Mask Creation
void Infomager::on_pushButton_createMask_clicked()
{
	write_text_to_log_file("PopPieMask\n");
	segM->show();
	segM->activateWindow();
	emit this->sendBackDrop(img);
}

void Infomager::on_pushButton_barMask_clicked()
{
	write_text_to_log_file("PopBarMask\n");
	segM->show();
	segM->activateWindow();
	emit this->sendBackDrop(img);
}

void Infomager::recieveMask(cv::Mat msk)
{
	if (chartType == Pie)
	{
		cv::Mat bckdrp;
		msk.copyTo(bckdrp);
		msk.copyTo(mask);
		maskSet = true;
		cv::compare(bckdrp, cv::GC_PR_FGD, bckdrp, cv::CMP_EQ);
		cv::threshold(bckdrp, bckdrp, 100, 255, cv::THRESH_BINARY);
		cv::cvtColor(bckdrp, bckdrp, CV_GRAY2RGB);
		QPixmap pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(bckdrp, QImage::Format_RGB888).rgbSwapped());
		ui.labelimg->setPixmap(pix_backdrop);
		cv::compare(mask, cv::GC_PR_FGD, mask, cv::CMP_EQ);
		cv::threshold(mask, mask, 100, 255, cv::THRESH_BINARY);
		this->fillSingleObject();		
	}	
	if (chartType == Bar)
	{
		cv::Mat bckdrp;
		msk.copyTo(bckdrp);
		msk.copyTo(barMasks[barMaskIdx]);
		barMasksSet[barMaskIdx] = true;
		cv::compare(bckdrp, cv::GC_PR_FGD, bckdrp, cv::CMP_EQ);
		cv::threshold(bckdrp, bckdrp, 100, 255, cv::THRESH_BINARY);
		cv::cvtColor(bckdrp, bckdrp, CV_GRAY2RGB);
		QPixmap pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(bckdrp, QImage::Format_RGB888).rgbSwapped());
		ui.label_barMask->setPixmap(pix_backdrop);
		cv::compare(barMasks[barMaskIdx], cv::GC_PR_FGD, barMasks[barMaskIdx], cv::CMP_EQ);
		cv::threshold(barMasks[barMaskIdx], barMasks[barMaskIdx], 100, 255, cv::THRESH_BINARY);
		
		scene_infomage->setCurrentBackdrop(filtimg);
		QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
		scene_infomage->removePieLabels();
		barFillYMin = INFINITY;
		barFillYMax = -INFINITY;
		for (int i = 0; i < barMasksSet.size(); i++)
		{
			if (barMasksSet[i])
			{
				cv::Mat src2;
				int thresh = 100;
				int max_thresh = 255;
				// Convert image to gray and blur it

				cvtColor(barMasks[i], src2, CV_GRAY2RGB);

				cv::Mat threshold_output;
				std::vector<std::vector<cv::Point> > contours;
				std::vector<cv::Vec4i> hierarchy;

				// Detect edges using Threshold
				threshold(barMasks[i], threshold_output, thresh, 255, cv::THRESH_BINARY);
				// Find contours
				findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

				// Approximate contours to polygons + get bounding rects and circles
				std::vector<std::vector<cv::Point> > contours_poly(contours.size());
				std::vector<cv::Rect> boundRect(contours.size());

				int b = 0;
				double ar = 0.0;
				for (int i = 0; i < contours.size(); i++)
				{
					approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
					boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
					if (ar < contourArea(contours[i]))
					{
						ar = contourArea(contours[i]);
						b = i;
					}
				}

				if (boundRect[b].y < barFillYMin) {
					barFillYMin = boundRect[b].y;
				}
				if ((boundRect[b].y + boundRect[b].height) > barFillYMax) {
					barFillYMax = boundRect[b].y + boundRect[b].height;
				}
			}
		}
		for (int i = 0; i < barMasksSet.size(); i++)
		{
			if (barMasksSet[i])
			{
				this->fillBarChart(i);
			}
		}
		QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
	}
}

//Distortion Help
void Infomager::on_pushButton_distortionHelp_clicked()
{
	write_text_to_log_file("PopDistortionHelp\n");
	distH->show();
	distH->activateWindow();
	emit this->sendDistortedVals(t_label, t_val, angles);
}

//Technique Select
void Infomager::on_pushButton_FillStyle_clicked()
{
	write_text_to_log_file("FillSelect\n");
	chartType = Pie;
	
	ui.groupBox_fill->setVisible(true);
	ui.label_FillStyle->setVisible(true);
	ui.groupBox_overlay->setVisible(false);
	ui.label_OverlayStyle->setVisible(false);

	pieType = Rad;
	ui.label_pieRad->setVisible(true);
	ui.pushButton_pieRad->setVisible(true);
	ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieArc->setStyleSheet("background: rgb(65, 65, 65);");
	ui.pushButton_pieArc->setText(QString("Arc"));

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Area:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Angle:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);


	ui.label_err3->setVisible(true);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(true);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(true);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(true);


	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);
	
	scene_infomage->removePieLabels();
	if (chartAdded)
	{
		chartAdded = false;
		scene_infomage->removeItem(chart);
	}
	if (dataLoaded)
	{
		drawPieChart(); 
	}
}

void Infomager::on_pushButton_OverlayStyle_clicked()
{
	write_text_to_log_file("OverlaySelect\n");
	chartType = Line;

	ui.groupBox_fill->setVisible(false);
	ui.label_FillStyle->setVisible(false);
	ui.groupBox_overlay->setVisible(true);
	ui.label_OverlayStyle->setVisible(true);

	ui.label_lineOverlay->setVisible(true);
	ui.label_barOverlay->setVisible(false);
	ui.label_pieOverlay->setVisible(false);

	ui.groupBox_LineOverlay->setVisible(true);
	ui.groupBox_BarOverlay->setVisible(false);
	ui.groupBox_PieOverlay->setVisible(false);


	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Orientation:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Height:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);


	ui.label_err3->setVisible(false);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(false);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(false);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(false);


	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);


	scene_infomage->removePieLabels();
	if (loaded_img)
	{
		scene_infomage->setCurrentBackdrop(filtimg);
		QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
	}

	if (dataLoaded)
	{
		drawLineChart();
	}
}

//Fill Technique Select

void Infomager::on_pushButton_fillRad_clicked() {
	divType = Radial;
	ui.label_fillRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_fillVerti->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillHori->setStyleSheet("background: rgb(65, 65, 65);");

	pieType = Rad;
	ui.label_pieRad->setVisible(true);
	ui.pushButton_pieRad->setVisible(true);
	ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieArc->setStyleSheet("background: rgb(65, 65, 65);");
	ui.pushButton_pieArc->setText(QString("Arc"));

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Area:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Angle:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);


	ui.label_err3->setVisible(true);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(true);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(true);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(true);


	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);
	
	this->fillSingleObject();

}

void Infomager::on_pushButton_fillVerti_clicked() {
	divType = Vertical;
	ui.label_fillRad->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillVerti->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_fillHori->setStyleSheet("background: rgb(65, 65, 65);");

	pieType = Arc;
	ui.label_pieArc->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieRad->setVisible(false);	
	ui.pushButton_pieRad->setVisible(false);
	ui.pushButton_pieArc->setText(QString("Length"));

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Area:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Length:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);

	ui.label_err3->setVisible(false);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(false);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(false);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(false);

	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);

	this->fillSingleObject();
}

void Infomager::on_pushButton_fillHori_clicked() {
	divType = Horizontal;
	ui.label_fillRad->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillVerti->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillHori->setStyleSheet("background: rgb(240, 240, 240);");

	pieType = Arc;
	ui.label_pieArc->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieRad->setVisible(false);
	ui.pushButton_pieRad->setVisible(false);
	ui.pushButton_pieArc->setText(QString("Length"));

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Area:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Length:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);

	ui.label_err3->setVisible(false);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(false);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(false);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(false);

	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);

	this->fillSingleObject();
}

void Infomager::on_pushButton_fillBarHori_clicked() {
	barDirection = BarHori;
	ui.label_fillHori_2->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_fillVerti_2->setStyleSheet("background: rgb(65, 65, 65);");
}

void Infomager::on_pushButton_fillBarVerti_clicked() {
	barDirection = BarVerti;
	ui.label_fillHori_2->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_fillVerti_2->setStyleSheet("background: rgb(240, 240, 240);");
}

void Infomager::fillSingleObject() {
	if (divType == Radial) {		
		this->computePieAngles();
		QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
	}
	if (divType == Horizontal) {	

		this->computeHorizontal();
		QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillHorizontal(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
	}
	if (divType == Vertical) {

		this->computeVertical();
		QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillVertical(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
	}
	
}

void Infomager::on_pushButton_optimizeChart_clicked() {
	if (chartType == Pie && divType == Radial) {
		write_text_to_log_file("PieChartOptimize\n");
		ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
		ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
		pieType = Area;
		double c1 = computePieAngles();
		orig_area_angles = angles;
		pieType = Arc;
		double c2 = computePieAngles();
		orig_arc_angles = angles;
		pieType = Rad;
		double c3 = computePieAngles();
		orig_rad_angles = angles;
		max_cost = (c1 + c2 + c3) / 3;

		max_change = 0;
		for (int i = 0; i < orig_area_angles.size(); i++) {
			if (abs(orig_area_angles[i] - orig_rad_angles[i]) > max_change) {
				max_change = abs(orig_area_angles[i] - orig_rad_angles[i]);
			}
			if (abs(orig_area_angles[i] - orig_arc_angles[i]) > max_change) {
				max_change = abs(orig_area_angles[i] - orig_arc_angles[i]);
			}
			if (abs(orig_rad_angles[i] - orig_arc_angles[i]) > max_change) {
				max_change = abs(orig_rad_angles[i] - orig_arc_angles[i]);
			}
			if (abs(0.05*angles[i]) > max_change) {
				max_change = abs(0.05*angles[i]);
			}
		}

		optimize = true;
		optimizePie();
	}
	if (chartType == Pie && divType == Horizontal) {
		write_text_to_log_file("HorizontalChartOptimize\n");
		ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
		ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
		pieType = Area;
		//double c1 = computeHorizontal();
		orig_area_angles = angles;
		pieType = Arc;
		double c2 = computeHorizontal();
		orig_arc_angles = angles;
		max_cost = c2; //(c1 + c2) / 2;
		
		max_change = 0;
		for (int i = 0; i < orig_area_angles.size(); i++) {		
			if (abs(orig_area_angles[i] - orig_arc_angles[i]) > max_change) {
				max_change = abs(orig_area_angles[i] - orig_arc_angles[i]);
			}		
			if (abs(0.01*angles[i]) > max_change) {
				max_change = abs(0.01*angles[i]);
			}
		}

		optimize = true;
		optimizeHorizontal();
	}
	if (chartType == Pie && divType == Vertical) {
		write_text_to_log_file("VerticalChartOptimize\n");
		ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
		ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
		pieType = Arc;
		double c1 = computeVertical();
		orig_area_angles = angles;
		pieType = Area;
		double c2 = computeVertical();
		orig_arc_angles = angles;
		max_cost =  (c1 + c2) / 2;

		max_change = 0;
		for (int i = 0; i < orig_area_angles.size(); i++) {
			if (abs(orig_area_angles[i] - orig_arc_angles[i]) > max_change) {
				max_change = abs(orig_area_angles[i] - orig_arc_angles[i]);
			}
			if (abs(0.01*angles[i]) > max_change) {
				max_change = abs(0.01*angles[i]);
			}
		}

		optimize = true;
		optimizeVertical();
	}
	if (chartType == Line) {		
		optimzeAspectRatio(false);
	}
	if (chartType == Bar && barOverlay) {		
		optimzeAspectRatio(true);
	}
	if (chartType == Bar && !barOverlay) {		
		barType = BarArea;
		double c1 = 0;
		scene_infomage->setCurrentBackdrop(filtimg);
		QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
		scene_infomage->removePieLabels();
		for (int i = 0; i < barMasksSet.size(); i++)
		{
			if (barMasksSet[i])
			{
				c1 = this->fillBarChart(i);
			}
		}
		QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
		
		
		barType = BarHeight;
		double c2 = 0;
		scene_infomage->setCurrentBackdrop(filtimg);
		pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(pix_viz);
		scene_infomage->removePieLabels();
		for (int i = 0; i < barMasksSet.size(); i++)
		{
			if (barMasksSet[i])
			{
				c2 = this->fillBarChart(i);
			}
		}
		filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
		
		
		max_cost = (c1 + c2) / 2;
		optimize = true;
		barType = BarOptimize;		
		optimizeBar();
		barType = BarHeight;
	}
}

//Pie Chart
void Infomager::on_pushButton_pieFill_clicked()
{
	write_text_to_log_file("PieChartSelect\n");
	//Check if the number of items is too large for a pie chart
	if (t_val.size() < 20)
	{
		//Set the chart type to Pie
		chartType = Pie;

		//Show controls for Pie and hide others
		ui.label_pieFill->setVisible(true);		
		ui.label_barFill->setVisible(false);

		ui.groupBox_PieFill->setVisible(true);
		ui.groupBox_BarFill->setVisible(false);
		
		ui.label_err1->setVisible(true);
		ui.label_err1->setText(QString("Area:"));
		ui.label_errVal1->setText(QString("0%"));
		ui.label_errorPin1->setGeometry(0, 10, 4, 22);

		ui.label_err2->setVisible(true);		
		ui.label_err2->setText(QString("Angle:"));
		ui.label_errVal2->setText(QString("0%"));
		ui.label_errorPin2->setGeometry(0, 47, 4, 22);


		ui.label_err3->setVisible(true);
		ui.label_err3->setText(QString("Arc Length:"));
		ui.label_errVal3->setVisible(true);
		ui.label_errVal3->setText(QString("0%"));
		ui.label_errorPin3->setVisible(true);
		ui.label_errorPin3->setGeometry(0, 84, 4, 22);
		ui.label_errorBar3->setVisible(true);
		

		ui.label_err4->setVisible(true);
		ui.label_err4->setText(QString("Avg.:"));
		ui.label_errVal4->setText(QString("0%"));
		ui.label_errorPin4->setGeometry(0, 121, 4, 22);


		//Remove previous memes
		if (chartAdded)
		{
			chartAdded = false;
			scene_infomage->removeItem(chart);
		}

		//Draw original pie chart in reference tile
		drawPieChart();
	}
	else
	{
		QMessageBox::information(0, "Alert", QString("Too many items for a pie chart."));
	}
}

void Infomager::on_pushButton_pieRad_clicked()
{
	write_text_to_log_file("PieChartRadial\n");
	ui.label_pieRad->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieArc->setStyleSheet("background: rgb(65, 65, 65);");
	optimize = false;
	pieType = Rad;
	if (loaded_img)
	{
		this->fillSingleObject();
	}
}

void Infomager::on_pushButton_pieAr_clicked()
{
	write_text_to_log_file("PieChartArea\n");
	ui.label_pieAr->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieRad->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieArc->setStyleSheet("background: rgb(65, 65, 65);");
	optimize = false;
	pieType = Area;
	if (loaded_img)
	{
		this->fillSingleObject();
	}
}

void Infomager::on_pushButton_pieArc_clicked()
{
	write_text_to_log_file("PieChartArc\n");
	ui.label_pieArc->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_pieAr->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_pieRad->setStyleSheet("background: rgb(65, 65, 65);");
	optimize = false;
	pieType = Arc;
	if (loaded_img)
	{
		this->fillSingleObject();
	}
}

void Infomager::on_horizontalSlider_pieAng_sliderReleased()
{
	write_text_to_log_file("PieChartAngleChange\n");
	pieRot = ui.horizontalSlider_pieAng->value();
	this->fillSingleObject();
}

void Infomager::on_spinBox_pieLabelRad_valueChanged()
{
	write_text_to_log_file("PieChartLabelAngleChange\n");
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_spinBox_pieOriginOffX_valueChanged()
{
	if (loaded_img)
	{
		write_text_to_log_file("PieChartCenterXChange\n");
		this->fillSingleObject();
	}
}

void Infomager::on_spinBox_pieOriginOffY_valueChanged()
{
	if (loaded_img)
	{
		write_text_to_log_file("PieChartCenterYChange\n");
		this->fillSingleObject();
	}
}

void Infomager::on_comboBoxBrewerNames_currentIndexChanged(const QString & text)
{
	write_text_to_log_file("PieChartColorSetChange\n");
	if(QString::compare(text, "", Qt::CaseInsensitive))
	{
		std::vector<char*>::iterator it;
		it = std::find(brewerNames.begin(), brewerNames.end(), text.toStdString());
		int pos = std::distance(brewerNames.begin(), it);
	
		for (int i = 0; i < brewer[pos].size(); i++)
		{
			if (brewer[pos][i].size() == t_val.size())
			{
				pieColors.clear();
				for (int j = 0; j < brewer[pos][i].size();j++)
				{
					QColor color = brewer[pos][i][j];
					pieColors.push_back(color);
					QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
					style = style.arg(color.red()).arg(color.green()).arg(color.blue());
					pieColorButtons[j]->setStyleSheet(style);
				}
				break;
			}
		}
		if (loaded_img && maskSet)
		{
			QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
			scene_infomage->refreshBackrop(pix_viz);
		}
	}
}

void Infomager::on_pushButton_pieColor_1_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[0], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_1->setStyleSheet(style);
	pieColors[0] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_2_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[1], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_2->setStyleSheet(style);
	pieColors[1] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_3_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[2], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_3->setStyleSheet(style);
	pieColors[2] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_4_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[3], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_4->setStyleSheet(style);
	pieColors[3] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_5_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[4], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_5->setStyleSheet(style);
	pieColors[4] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_6_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[5], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_6->setStyleSheet(style);
	pieColors[5] = color;
	this->fillSingleObject();
}

void Infomager::on_pushButton_pieColor_7_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[6], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_7->setStyleSheet(style);
	pieColors[6] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_8_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[7], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_8->setStyleSheet(style);
	pieColors[7] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_9_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[8], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_9->setStyleSheet(style);
	pieColors[8] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_10_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[9], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_10->setStyleSheet(style);
	pieColors[9] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_11_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[10], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_11->setStyleSheet(style);
	pieColors[10] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_12_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[11], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_12->setStyleSheet(style);
	pieColors[11] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_13_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[12], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_13->setStyleSheet(style);
	pieColors[12] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_14_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[13], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_14->setStyleSheet(style);
	pieColors[13] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::on_pushButton_pieColor_15_clicked()
{
	write_text_to_log_file("PieChartColorIndvChange\n");
	QColor color = QColorDialog::getColor(pieColors[14], this, "Text Color");
	QString style = "border-style: none;background-color: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.pushButton_pieColor_15->setStyleSheet(style);
	pieColors[14] = color;
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

void Infomager::drawPieChart()
{
	if (chartPrwAdded)
	{
		chartPrwAdded = false;
		scene_preview->removeItem(chartpreview);
		chartpreview = 0;
	}

	QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

	for (int i = 0; i < t_val.size(); i++)
	{
		series->append(QString::fromStdString(t_label[i]), t_val[i]);
	}

	series->setLabelsVisible(true);
	series->setPieSize(0.6);

	chartpreview = new QtCharts::QChart();
	chartpreview->setPreferredSize(260, 180);
	chartpreview->addSeries(series);
	chartpreview->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
	chartpreview->createDefaultAxes();
	chartpreview->setMargins(QMargins(0, 0, 0, 0));
	chartpreview->legend()->hide();
	chartpreview->setBackgroundVisible(false);

	scene_preview->addItem(chartpreview);
	chartPrwAdded = true;
}

double Infomager::computePieAngles()
{
	cv::Mat src2;
	int thresh = 100;
	int max_thresh = 255;
	// Convert image to gray and blur it
	cvtColor(mask, src2, CV_GRAY2RGB);

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// Detect edges using Threshold
	threshold(mask, threshold_output, thresh, 255, cv::THRESH_BINARY);
	// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

	// Approximate contours to polygons + get bounding rects and circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<cv::Point2f>center(contours.size());
	std::vector<float>radius(contours.size());

	int bigIdx = 0; //Index of biggest contour
	double ar = 0.0; //Area of biggest contour
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
		if (ar < contourArea(contours[i]))
		{
			ar = contourArea(contours[i]);
			bigIdx = i;
		}
	}

	// Draw biggest contour 
	std::vector<cv::Point> bigContour = contours[bigIdx];
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	src2 = cv::Mat::zeros(src2.size(), CV_8UC3);
	drawContours(src2, contours, bigIdx, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

	std::vector<cv::Point> bighull;
	cv::convexHull(bigContour, bighull);

	//compute total of data values
	double total = 0;

	for (int j = 0; j < t_val.size(); j++)
	{
		total = total + t_val[j];
	}

	if (!optimize) {
		angles.clear();
	}
	areas.clear();
	areas.push_back(0);
	arcLength.clear();
	arcLength.push_back(0);
	pieCenter = center[bigIdx];
	pieCenter.x = pieCenter.x + ui.spinBox_pieOriginOffX->value();
	pieCenter.y = pieCenter.y + ui.spinBox_pieOriginOffY->value();

	//Compute Angles 
	if (pieType == Area & !optimize)
	{
		//Based on area
		double area = contourArea(contours[bigIdx]); //Total region area
		double offset = pieRot; // Start angle of first segment (offset from 0)
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3); // Temporary image to hold each segment of the area to compute the segment area
		for (int j = 0; j < t_val.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);  // Temporary image to hold each segment of the area to compute the segment area
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			double start = offset;
			double stop = start + 0.5;
			double area_tmp = 0.0;
			angles.push_back(offset);
			while (area_tmp < (area*t_val[j] / total))
			{
				drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				ellipse(drawing, pieCenter, cv::Size((int)radius[bigIdx] * 2, (int)radius[bigIdx] * 2), 0, start, stop, cv::Scalar(255, 255, 255), -2, 8, 0);
				drawing.copyTo(drawing_tmp, src2);
				cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
				threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
				findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
				stop = stop + 0.5;
				area_tmp = contourArea(contours[0]);
			}
			offset = stop - 0.5;
			areas.push_back(area_tmp);
		}
		angles.push_back(360 + pieRot); // Add 360 to the hardcoded offset
		drawing_tmp.release();
	}
	else if (pieType == Rad & !optimize)
	{
		//Based on Angles
		double offset = pieRot;
		if (!optimize) {
			//Compute Angles
			for (int j = 0; j < t_val.size(); j++)
			{
				angles.push_back(offset);
				offset = offset + (t_val[j] * 360 / total);
			}
			angles.push_back(360 + pieRot); // Add 360 to the hardcoded offset
		}


		//Compute Areas
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
		double area_tmp = 0.0;
		for (int j = 1; j < angles.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			ellipse(drawing, pieCenter, cv::Size((int)radius[bigIdx] * 2, (int)radius[bigIdx] * 2), 0, angles[j - 1], angles[j], cv::Scalar(255, 255, 255), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);
			cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
			threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
			findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
			area_tmp = contourArea(contours[0]);
			areas.push_back(area_tmp);
		}
		drawing_tmp.release();
	}
	else if (pieType == Arc & !optimize)
	{
		//Based on Arc Length
		std::vector<std::vector<cv::Point>> contourSegs;
		contourSegs.push_back(bighull);
		cv::Mat hull_mask = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
		drawContours(hull_mask, contourSegs, 0, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

		cv::Mat threshold_output_hull;
		std::vector<std::vector<cv::Point> > hull_contours;
		std::vector<cv::Vec4i> hull_hierarchy;

		cvtColor(hull_mask, hull_mask, CV_RGB2GRAY);
		// Detect edges using Threshold
		threshold(hull_mask, threshold_output_hull, thresh, 255, cv::THRESH_BINARY);
		// Find contours
		findContours(threshold_output_hull, hull_contours, hull_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

		int bigHullIdx = 0; //Index of biggest contour
		ar = 0.0; //Area of biggest contour
		for (int i = 0; i < hull_contours.size(); i++)
		{
			approxPolyDP(cv::Mat(hull_contours[i]), contours_poly[i], 3, true);
			minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
			if (ar < contourArea(hull_contours[i]))
			{
				ar = contourArea(hull_contours[i]);
				bighull = hull_contours[i];
			}
		}
		contourSegs.clear();

		double arcLen = cv::arcLength(bighull, false); //Total Arc Length
		char buffer[100];
		sprintf_s(buffer, "ArcLen: %f\n", arcLen);
		OutputDebugStringA(buffer);
		double offset = pieRot; // Start angle of first segment (offset from 0)
		for (int j = 0; j < t_val.size(); j++)
		{
			double start = offset;
			double stop = start + 0.5;
			double arc_tmp = 0.0;
			angles.push_back(offset);
			while (arc_tmp < (arcLen*t_val[j] / total))
			{
				//Compute arc length of  sector
				//Create triangle from pie center for current angle;
				std::vector<cv::Point> triangle;
				triangle.push_back(pieCenter);
				double x = pieCenter.x + radius[bigIdx] * 10 * cos((start)* PI / 180);
				double y = pieCenter.y + radius[bigIdx] * 10 * sin((start)* PI / 180);
				triangle.push_back(cv::Point(x, y));
				x = pieCenter.x + radius[bigIdx] * 10 * cos((stop)* PI / 180);
				y = pieCenter.y + radius[bigIdx] * 10 * sin((stop)* PI / 180);
				triangle.push_back(cv::Point(x, y));

				//Find the contour points inside the triangle
				std::vector<cv::Point> inPoints;
				std::vector<int> indices;
				bool reorder = false;

				for (int i = 0; i < bighull.size(); i++) {
					//if point inside trianlge add it to list
					if (cv::pointPolygonTest(triangle, bighull[i], false) > -1) {
						inPoints.push_back(bighull[i]);
						indices.push_back(i);
						if (i == 0) {
							//the set of points containing the first point may need to be reordered.
							reorder = true;
						}
					}
				}

				//Reorder points if neccessary
				if (inPoints.size() > 1) {
					if (reorder) {
						std::vector<cv::Point> inPointsReorder;
						std::vector<int> indicesReorder;
						for (int i = 1; i < inPoints.size(); i++) {
							if (indices[i] - indices[i - 1] > 1) {
								for (int j = 0; j < inPoints.size(); j++) {
									inPointsReorder.push_back(inPoints[(j + i) % inPoints.size()]);
									indicesReorder.push_back(indices[(j + i) % indices.size()]);
								}
							}
						}
						if (inPointsReorder.size() > 0) {
							inPointsReorder.push_back(bighull[(indicesReorder[indicesReorder.size() - 1] + 1) % bighull.size()]);
							arc_tmp = cv::arcLength(inPointsReorder, false);//New arc len
						}
						else {
							inPoints.push_back(bighull[(indices[indices.size() - 1] + 1) % bighull.size()]);
							arc_tmp = cv::arcLength(inPoints, false);//New arc len
						}
						std::vector<cv::Point>().swap(inPointsReorder);
						std::vector<int>().swap(indicesReorder);
					}
					else {
						inPoints.push_back(bighull[(indices[indices.size() - 1] + 1) % bighull.size()]);
						arc_tmp = cv::arcLength(inPoints, false);//New arc len
					}
				}

				stop = stop + 0.5;
			}
			char buffer2[100];
			sprintf_s(buffer2, "ArcLen: %f\n", arc_tmp);
			OutputDebugStringA(buffer2);
			offset = stop - 0.5;
			arcLength.push_back(arc_tmp);
		}
		angles.push_back(360 + pieRot); // Add 360 to the hardcoded offset		
		hull_mask.release();
		threshold_output_hull.release();

		//Compute Areas
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
		double area_tmp = 0.0;
		for (int j = 1; j < angles.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			ellipse(drawing, pieCenter, cv::Size((int)radius[bigIdx] * 2, (int)radius[bigIdx] * 2), 0, angles[j - 1], angles[j], cv::Scalar(255, 255, 255), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);
			cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
			threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
			findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
			area_tmp = contourArea(contours[0]);
			areas.push_back(area_tmp);
		}
		drawing_tmp.release();
	}


	//*********************************************************************************************
	//Generate hull mask
	if (pieType != Arc) {
		std::vector<std::vector<cv::Point>> contourSegs;
		contourSegs.push_back(bighull);
		cv::Mat hull_mask = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
		drawContours(hull_mask, contourSegs, 0, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

		cv::Mat threshold_output_hull;
		std::vector<std::vector<cv::Point> > hull_contours;
		std::vector<cv::Vec4i> hull_hierarchy;

		cvtColor(hull_mask, hull_mask, CV_RGB2GRAY);
		// Detect edges using Threshold
		threshold(hull_mask, threshold_output_hull, thresh, 255, cv::THRESH_BINARY);
		// Find contours
		findContours(threshold_output_hull, hull_contours, hull_hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

		int bigHullIdx = 0; //Index of biggest contour
		ar = 0.0; //Area of biggest contour
		for (int i = 0; i < hull_contours.size(); i++)
		{
			approxPolyDP(cv::Mat(hull_contours[i]), contours_poly[i], 3, true);
			minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
			if (ar < contourArea(hull_contours[i]))
			{
				ar = contourArea(hull_contours[i]);
				bighull = hull_contours[i];
			}
		}
		contourSegs.clear();

		for (int j = 1; j < angles.size(); j++) {
			//Create triangle from pie center for current angle;
			std::vector<cv::Point> triangle;
			triangle.push_back(pieCenter);
			double x = pieCenter.x + radius[bigIdx] * 10 * cos((angles[j - 1]) * PI / 180);
			double y = pieCenter.y + radius[bigIdx] * 10 * sin((angles[j - 1]) * PI / 180);
			triangle.push_back(cv::Point(x, y));
			x = pieCenter.x + radius[bigIdx] * 10 * cos((angles[j]) * PI / 180);
			y = pieCenter.y + radius[bigIdx] * 10 * sin((angles[j]) * PI / 180);
			triangle.push_back(cv::Point(x, y));

			//Find the contour points inside the triangle
			std::vector<cv::Point> inPoints;
			std::vector<int> indices;
			bool reorder = false;

			for (int i = 0; i < bighull.size(); i++) {
				//if point inside trianlge add it to list
				if (cv::pointPolygonTest(triangle, bighull[i], false) > -1) {
					inPoints.push_back(bighull[i]);
					indices.push_back(i);
					if (i == 0) {
						//the set of points containing the first point may need to be reordered.
						reorder = true;
					}
				}
			}
			if (inPoints.size() > 1) {
				if (reorder) {
					std::vector<cv::Point> inPointsReorder;
					std::vector<int> indicesReorder;
					for (int i = 1; i < inPoints.size(); i++) {
						if (indices[i] - indices[i - 1] > 1) {
							for (int j = 0; j < inPoints.size(); j++) {
								inPointsReorder.push_back(inPoints[(j + i) % inPoints.size()]);
								indicesReorder.push_back(indices[(j + i) % indices.size()]);
							}
						}
					}
					if (inPointsReorder.size() > 0) {
						inPointsReorder.push_back(bighull[(indicesReorder[indicesReorder.size() - 1] + 1) % bighull.size()]);
						contourSegs.push_back(inPointsReorder);
					}
					else {
						inPoints.push_back(bighull[(indices[indices.size() - 1] + 1) % bighull.size()]);
						contourSegs.push_back(inPoints);
					}
					std::vector<cv::Point>().swap(inPointsReorder);
					std::vector<int>().swap(indicesReorder);
				}
				else {
					inPoints.push_back(bighull[(indices[indices.size() - 1] + 1) % bighull.size()]);
					contourSegs.push_back(inPoints);
				}
			}
			std::vector<cv::Point>().swap(triangle);
			std::vector<cv::Point>().swap(inPoints);
			std::vector<int>().swap(indices);
		}

		arcLength.clear();
		arcLength.push_back(0);
		for (int i = 0; i < contourSegs.size(); i++)
		{
			arcLength.push_back(cv::arcLength(contourSegs[i], false));
		}
		std::vector<std::vector<cv::Point>>().swap(contourSegs);
		std::vector<std::vector<cv::Point>>().swap(hull_contours);
		std::vector<cv::Vec4i>().swap(hull_hierarchy);
		hull_mask.release();
		threshold_output_hull.release();
	}
	//********************************************Compute Errors*************************************************

	double totalAng = 0;
	double totalArea = 0;
	double totalArc = 0;
	for (int j = 1; j < angles.size(); j++) {
		totalAng = totalAng + (angles[j] - angles[j - 1]);
		totalArea = totalArea + areas[j];
		totalArc = totalArc + arcLength[j];
	}

	double errAng = 0;
	double errArea = 0;
	double errArc = 0;
	for (int j = 1; j < angles.size(); j++) {
		double d = t_val[j - 1] / total;
		errAng = errAng + abs((d - (angles[j] - angles[j - 1]) / totalAng) / d);
		errArea = errArea + abs((d - areas[j] / totalArea) / d);
		errArc = errArc + abs((d - arcLength[j] / totalArc) / d);
	}

	ui.label_errVal1->setText(QString::number(roundf((errArea / t_val.size()) * 100), 'g', 2).append("%"));
	ui.label_errVal2->setText(QString::number(roundf((errAng / t_val.size()) * 100), 'g', 2).append("%"));
	ui.label_errVal3->setText(QString::number(roundf((errArc / t_val.size()) * 100), 'g', 2).append("%"));

	int pinX = (int)120 * roundf((errArea / t_val.size()) * 100) / 100;
	ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);
	pinX = (int)120 * roundf((errAng / t_val.size()) * 100) / 100;
	ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);
	pinX = (int)120 * roundf((errArc / t_val.size()) * 100) / 100;
	ui.label_errorPin3->setGeometry(pinX, 84, 4, 22);
	   	  
	//Release memory used by temp images
	src2.release();
	threshold_output.release();
	drawing.release();

	//Release memory used by vectors
	std::vector<std::vector<cv::Point>>().swap(contours);
	std::vector<cv::Vec4i>().swap(hierarchy);
	std::vector<std::vector<cv::Point>>().swap(contours_poly);
	std::vector<cv::Point2f>().swap(center);
	std::vector<float>().swap(radius);
	std::vector<cv::Point>().swap(bigContour);
	std::vector<cv::Point>().swap(bighull);

	double avgError = (errArea + errAng + errArc) / (t_val.size());
	ui.label_errVal4->setText(QString::number(100 * avgError / 3, 'g', 2).append("%"));
	pinX = (int)120 * avgError / 3;
	ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);
	return avgError / 3;
}

cv::Mat Infomager::fillPie()
{
	cv::Mat image(filtimg.size(), CV_8UC3);
	filtimg.copyTo(image);

	//draw a piechart based on the angles computed
	cv::Mat drawing(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Point pt;
	pt.x = pieCenter.x;
	pt.y = pieCenter.y;
	for (int j = 0; j < t_val.size(); j++)
	{
		cv::Scalar color = cv::Scalar(pieColors[j].red(), pieColors[j].green(), pieColors[j].blue());
		double start = angles[j];
		double stop = angles[j + 1];
		ellipse(drawing, pt, cv::Size(filtimg.cols, filtimg.cols), 0, start, stop, color, -2, 8, 0);
		//cv::namedWindow("Detected Lines", 1);
		//cv::imshow("Detected Lines", drawing);
		//cv::waitKey();
	}



	//Use the mask to cut out the piechart
	cv::Mat foreground(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	drawing.copyTo(foreground, mask);

	//Merge the chart with the image in LAB color space
	cvtColor(image, image, CV_BGR2Lab);
	cvtColor(foreground, foreground, CV_RGB2Lab);

	//Merge the chart with the image in HSV color space
	for (int x = 0; x < mask.rows; x++)
	{
		for (int y = 0; y < mask.cols; y++)
		{
			cv::Vec3b intensity = foreground.at<cv::Vec3b>(x, y);
			if (intensity.val[0] != 0)
			{
				//For LAB
				image.at<cv::Vec3b>(x, y).val[1] = intensity.val[1];
				image.at<cv::Vec3b>(x, y).val[2] = intensity.val[2];

				//For HSV
				//image.at<cv::Vec3b>(x, y).val[0] = intensity.val[0];								
			}
		}
	}

	cvtColor(image, image, CV_Lab2BGR);

	// Labelling the chart
	scene_infomage->removePieLabels();

	QGraphicsTextItem *tmp;
	double txtAng = 0;
	for (int i = 0; i < t_val.size(); i++)
	{
		tmp = new QGraphicsTextItem(QString::fromStdString(t_label[i]) + ": " + QString("%1").arg(t_val[i]) + "%");
		tmp->setZValue(600);
		txtAng = (angles[i] + pieRot + angles[i + 1] + pieRot) / 2;
		txtAng = (txtAng * PI) / 180.0;
		int txtRad = ui.spinBox_pieLabelRad->value();;
		double x, y;
		if (txtAng >= 2 * PI)
		{
			txtAng = txtAng - floor(txtAng / PI)*PI;
		}
		if (txtAng > PI / 2 && txtAng < 3 * PI / 2)
		{
			x = pieCenter.x + txtRad * cos(txtAng) - tmp->boundingRect().width();
		}
		else
		{
			x = pieCenter.x + txtRad * cos(txtAng);
		}
		if (txtAng > 0 && txtAng < PI)
		{
			y = pieCenter.y + txtRad * sin(txtAng);
		}
		else
		{
			y = pieCenter.y + txtRad * sin(txtAng) - tmp->boundingRect().height();
		}
		tmp->setPos(QPointF(x, y));
		tmp->setFlag(QGraphicsItem::ItemIsSelectable, true);
		tmp->setFlag(QGraphicsItem::ItemIsMovable, true);
		scene_infomage->addItem(tmp);
	}
	return image;
}

void Infomager::generateNeighborAngles(std::vector<double> old_angles) {
	std::vector<double> newAngles;
	std::vector<double> randNos;
	newAngles.push_back(old_angles[0]);
	for (int i = 1; i < old_angles.size() - 1; i++) {
		double rNo = (((double)rand() / (RAND_MAX)) * max_change * 2) - max_change;
		newAngles.push_back(old_angles[i] + rNo);
	}

	newAngles.push_back(old_angles[old_angles.size() - 1]);
	double rRot = ((double)rand() / (RAND_MAX)) * 360;
	for (int i = 0; i < angles.size(); i++) {
		angles[i] = newAngles[i] + rRot;
	}
}

void Infomager::optimizePie() {
	double old_cost = computePieAngles();
	double T = 1.0;
	double T_min = 0.00001;
	double alpha = 0.9;
	std::vector<double> curr_angles = angles;
	std::vector<double> old_angles = angles;
	std::vector<double> best_angles = angles;
	double best_cost = old_cost;
	int itrCnt = 0;
	double totDiff = 0;
	double avgDiff = 1;
	while (T > T_min  & avgDiff > 0.0001) {
		int i = 1;
		int j = 1;
		while (i <= 200 & j <= 200) {
			generateNeighborAngles(old_angles);
			double new_cost = computePieAngles();
			double ap = acceptance_probability(old_cost, new_cost, T);
			double r = ((double)rand() / (RAND_MAX)) + 0.00001;

			if (new_cost < best_cost) {
				best_cost = new_cost;
				best_angles = angles;
			}

			itrCnt += 1;
			totDiff += abs(new_cost - old_cost);
			avgDiff = totDiff / itrCnt;

			if (ap > r) {
				char buffer[100];
				sprintf_s(buffer, "Accept.   Error Diff: %f   AP: %f\n", (new_cost - old_cost), ap);
				OutputDebugStringA(buffer);

				curr_angles = angles;
				old_cost = new_cost;
				i += 1;
				j = 1;				
			}
			else {
				char buffer[100];
				sprintf_s(buffer, "Reject.   Error Diff: %f\n", (new_cost - old_cost));
				OutputDebugStringA(buffer);
				angles = curr_angles;
				j += 1;
			}
		}
		T = T * alpha;
	}
	char buffer2[100];
	sprintf_s(buffer2, "Curr Cost: %f\n", old_cost);
	OutputDebugStringA(buffer2);
	sprintf_s(buffer2, "Best Cost: %f \n", best_cost);
	OutputDebugStringA(buffer2);

	angles = curr_angles;
	computePieAngles();

	QMessageBox::information(0, "info", QString("Optimize done"));
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillPie(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

//Horizontal Chart
double Infomager::computeHorizontal()
{
	cv::Mat src2;
	int thresh = 100;
	int max_thresh = 255;
	// Convert image to gray and blur it
	cvtColor(mask, src2, CV_GRAY2RGB);

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// Detect edges using Threshold
	threshold(mask, threshold_output, thresh, 255, cv::THRESH_BINARY);
	// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

	// Approximate contours to polygons + get bounding rects and circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<cv::Point2f>center(contours.size());
	std::vector<float>radius(contours.size());

	int bigIdx = 0; //Index of biggest contour
	double ar = 0.0; //Area of biggest contour
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
		if (ar < contourArea(contours[i]))
		{
			ar = contourArea(contours[i]);
			bigIdx = i;
		}
	}

	// Draw biggest contour 
	std::vector<cv::Point> bigContour = contours[bigIdx];
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	src2 = cv::Mat::zeros(src2.size(), CV_8UC3);
	drawContours(src2, contours, bigIdx, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

	std::vector<cv::Point> bighull;
	cv::convexHull(bigContour, bighull);

	//compute total of data values
	double total = 0;

	for (int j = 0; j < t_val.size(); j++)
	{
		total = total + t_val[j];
	}

	if (!optimize) {
		angles.clear();
	}
	areas.clear();
	areas.push_back(0);
	arcLength.clear();
	arcLength.push_back(0);
	pieCenter = center[bigIdx];
	pieCenter.x = pieCenter.x + ui.spinBox_pieOriginOffX->value();
	pieCenter.y = pieCenter.y + ui.spinBox_pieOriginOffY->value();

	//Compute Angles 
	if (pieType == Area & !optimize)
	{
		//Based on area
		double area = contourArea(contours[bigIdx]); //Total region area
		cv::Rect encRect = boundingRect(contours[bigIdx]);
		double offset = encRect.x; // Start angle of first segment (offset from 0)
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3); // Temporary image to hold each segment of the area to compute the segment area
		for (int j = 0; j < t_val.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);  // Temporary image to hold each segment of the area to compute the segment area
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			double start = offset;
			double stop = start + 1;
			double area_tmp = 0.0;
			angles.push_back(offset);
			while (area_tmp < (area*t_val[j] / total))
			{
				drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				cv::Point pt1(start, encRect.y);
				cv::Point pt2(stop, encRect.y + encRect.height);
				rectangle(drawing, pt1, pt2, cv::Scalar(255, 255, 255), -2, 8, 0);
				drawing.copyTo(drawing_tmp, src2);
				cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
				threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
				findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
				stop = stop + 1;
				area_tmp = contourArea(contours[0]);
			}
			offset = stop - 1;
			areas.push_back(area_tmp);
		}
		angles.push_back(encRect.x + encRect.width); // Add 360 to the hardcoded offset
		drawing_tmp.release();
	}
	else if (pieType == Arc & !optimize)
	{
		//Based on Axis Length
		cv::Rect encRect = boundingRect(contours[bigIdx]);
		double offset = encRect.x; // Start angle of first segment (offset from 0)
		angles.push_back(offset);
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3); // Temporary image to hold each segment of the area to compute the segment area
		for (int j = 0; j < t_val.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);  // Temporary image to hold each segment of the area to compute the segment area
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			double start = offset;
			double stop = start + 0.5;
			double area_tmp = 0.0;			
			double segWidth = encRect.width*t_val[j] / total;
			angles.push_back(start + segWidth);
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			cv::Point pt1(start, encRect.y);
			cv::Point pt2(start+ segWidth, encRect.y + encRect.height);
			rectangle(drawing, pt1, pt2, cv::Scalar(255, 255, 255), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);
			cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
			threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
			findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));			
			area_tmp = contourArea(contours[0]);			
			areas.push_back(area_tmp);
			offset = start + segWidth;
		}
		//angles.push_back(360 + pieRot); // Add 360 to the hardcoded offset
		drawing_tmp.release();
	}

	//********************************************Compute Errors*************************************************

	
	double totalArea = 0;
	double totalArc = 0;
	for (int j = 1; j < angles.size(); j++) {
		totalArea = totalArea + areas[j];
		double start = angles[j-1];
		double stop = angles[j];
		totalArc = totalArc + (stop - start);
	}

	
	double errArea = 0;
	double errArc = 0;
	for (int j = 1; j < angles.size(); j++) {
		double d = t_val[j - 1] / total;
		errArea = errArea + abs((d - areas[j] / totalArea) / d);
		double start = angles[j-1];
		double stop = angles[j];
		errArc = errArc + abs((d - (stop - start) / totalArc) / d);
	}

	ui.label_errVal1->setText(QString::number(roundf((errArea / t_val.size()) * 100), 'g', 2).append("%"));	
	ui.label_errVal2->setText(QString::number(roundf((errArc / t_val.size()) * 100), 'g', 2).append("%"));

	int pinX = (int)120 * roundf((errArea / t_val.size()) * 100) / 100;
	ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);
	pinX = (int)120 * roundf((errArc / t_val.size()) * 100) / 100;
	ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);

	//Release memory used by temp images
	src2.release();
	threshold_output.release();
	drawing.release();


	//Release memory used by vectors
	std::vector<std::vector<cv::Point>>().swap(contours);
	std::vector<cv::Vec4i>().swap(hierarchy);
	std::vector<std::vector<cv::Point>>().swap(contours_poly);
	std::vector<cv::Point2f>().swap(center);
	std::vector<float>().swap(radius);
	std::vector<cv::Point>().swap(bigContour);
	std::vector<cv::Point>().swap(bighull);

	//double avgError = (errArea + errAng) / (t_val.size());	
	double avgError = (errArea + errArc) / (t_val.size());
	ui.label_errVal4->setText(QString::number(100 * avgError / 2, 'g', 2).append("%"));
	pinX = (int)120 * avgError / 2;
	ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);
	return avgError / 2;
}

cv::Mat Infomager::fillHorizontal()
{
	cv::Mat image(filtimg.size(), CV_8UC3);
	filtimg.copyTo(image);

	//draw a rect based on the angles computed
	cv::Mat drawing(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Point pt;
	pt.x = pieCenter.x;
	pt.y = pieCenter.y;
	for (int j = 0; j < t_val.size(); j++)
	{
		cv::Scalar color = cv::Scalar(pieColors[j].red(), pieColors[j].green(), pieColors[j].blue());
		double start = angles[j];
		double stop = angles[j + 1];
		
		cv::Point pt1(start, 0);
		cv::Point pt2(stop, filtimg.rows);
		rectangle(drawing, pt1, pt2, color, -2, 8, 0);
		//cv::namedWindow("Detected Lines", 1);
		//cv::imshow("Detected Lines", drawing);
		//cv::waitKey();
	}



	//Use the mask to cut out the piechart
	cv::Mat foreground(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	drawing.copyTo(foreground, mask);

	//Merge the chart with the image in LAB color space
	cvtColor(image, image, CV_BGR2Lab);
	cvtColor(foreground, foreground, CV_RGB2Lab);

	//Merge the chart with the image in HSV color space

	for (int x = 0; x < mask.rows; x++)
	{
		for (int y = 0; y < mask.cols; y++)
		{
			cv::Vec3b intensity = foreground.at<cv::Vec3b>(x, y);
			if (intensity.val[0] != 0)
			{
				//For LAB
				image.at<cv::Vec3b>(x, y).val[1] = intensity.val[1];
				image.at<cv::Vec3b>(x, y).val[2] = intensity.val[2];							
			}
		}
	}

	cvtColor(image, image, CV_Lab2BGR);

	// Labelling the chart
	scene_infomage->removePieLabels();

	QGraphicsTextItem *tmp;
	double txtAng = 0;
	for (int i = 0; i < t_val.size(); i++)
	{
		tmp = new QGraphicsTextItem(QString::fromStdString(t_label[i]) + ": " + QString("%1").arg(t_val[i]) + "%");
		tmp->setZValue(600);
		txtAng = (angles[i] + pieRot + angles[i + 1] + pieRot) / 2;
		txtAng = (txtAng * PI) / 180.0;
		int txtRad = ui.spinBox_pieLabelRad->value();;
		double x, y;
		if (txtAng >= 2 * PI)
		{
			txtAng = txtAng - floor(txtAng / PI)*PI;
		}
		if (txtAng > PI / 2 && txtAng < 3 * PI / 2)
		{
			x = pieCenter.x + txtRad * cos(txtAng) - tmp->boundingRect().width();
		}
		else
		{
			x = pieCenter.x + txtRad * cos(txtAng);
		}
		if (txtAng > 0 && txtAng < PI)
		{
			y = pieCenter.y + txtRad * sin(txtAng);
		}
		else
		{
			y = pieCenter.y + txtRad * sin(txtAng) - tmp->boundingRect().height();
		}
		tmp->setPos(QPointF(x, y));
		tmp->setFlag(QGraphicsItem::ItemIsSelectable, true);
		tmp->setFlag(QGraphicsItem::ItemIsMovable, true);
		scene_infomage->addItem(tmp);
	}
	return image;
}

void Infomager::generateNeighborHorDiv(std::vector<double> old_angles) {
	std::vector<double> newAngles;
	std::vector<double> randNos;
	newAngles.push_back(old_angles[0]);
	for (int i = 1; i < old_angles.size() - 1; i++) {
		//double rNo = ((double)rand() / (RAND_MAX)) * 16 - 8;
		double rNo = (((double)rand() / (RAND_MAX)) * max_change * 2) - max_change;
		newAngles.push_back(old_angles[i] + rNo);
	}

	for (int i = 0; i < angles.size(); i++) {
		angles[i] = newAngles[i];
	}	
}

void Infomager::optimizeHorizontal() {
	double old_cost = computeHorizontal();
	double T = 1.0;
	double T_min = 0.00001;
	double alpha = 0.9;
	std::vector<double> curr_angles = angles;
	std::vector<double> old_angles = angles;
	std::vector<double> best_angles = angles;
	double best_cost = old_cost;
	int itrCnt = 0;
	double totDiff = 0;
	double avgDiff = 1;
	while (T > T_min  & avgDiff > 0.0001) {
		int i = 1;
		int j = 1;
		while (i <= 200 & j <= 200) {
			generateNeighborHorDiv(old_angles);
			double new_cost = computeHorizontal();
			double ap = acceptance_probability(old_cost, new_cost, T);
			double r = ((double)rand() / (RAND_MAX)) + 0.00001;
			
			if (new_cost < best_cost) {
				best_cost = new_cost;
				best_angles = angles;
			}

			itrCnt += 1;
			totDiff += abs(new_cost - old_cost);
			avgDiff = totDiff / itrCnt;
			if (ap > r) {
				char buffer[100];
				sprintf_s(buffer, "Accept.   Error Diff: %f   AP: %f\n", (new_cost - old_cost), ap);
				OutputDebugStringA(buffer);

				curr_angles = angles;
				old_cost = new_cost;
				i += 1;
				j = 1;
			}
			else {
				char buffer[100];
				sprintf_s(buffer, "Reject.   Error Diff: %f\n", (new_cost - old_cost));
				OutputDebugStringA(buffer);

				angles = curr_angles;
				j += 1;
			}
		}
		T = T * alpha;
	}
	
	char buffer2[100];
	sprintf_s(buffer2, "Curr Cost: %f\n", old_cost);
	OutputDebugStringA(buffer2);
	sprintf_s(buffer2, "Best Cost: %f \n", best_cost);
	OutputDebugStringA(buffer2);

	angles = best_angles;
	computeHorizontal();

	QMessageBox::information(0, "info", QString("Optimize done"));
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillHorizontal(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

//Vertical Chart
double Infomager::computeVertical()
{	
	cv::Mat src2;
	int thresh = 100;
	int max_thresh = 255;
	// Convert image to gray and blur it
	cvtColor(mask, src2, CV_GRAY2RGB);

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	// Detect edges using Threshold
	threshold(mask, threshold_output, thresh, 255, cv::THRESH_BINARY);
	// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cv::Point(0, 0));

	// Approximate contours to polygons + get bounding rects and circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<cv::Point2f>center(contours.size());
	std::vector<float>radius(contours.size());

	int bigIdx = 0; //Index of biggest contour
	double ar = 0.0; //Area of biggest contour
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((cv::Mat)contours_poly[i], center[i], radius[i]);
		if (ar < contourArea(contours[i]))
		{
			ar = contourArea(contours[i]);
			bigIdx = i;
		}
	}

	// Draw biggest contour 
	std::vector<cv::Point> bigContour = contours[bigIdx];
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	src2 = cv::Mat::zeros(src2.size(), CV_8UC3);
	drawContours(src2, contours, bigIdx, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

	std::vector<cv::Point> bighull;
	cv::convexHull(bigContour, bighull);

	//compute total of data values
	double total = 0;

	for (int j = 0; j < t_val.size(); j++)
	{
		total = total + t_val[j];
	}

	if (!optimize) {
		angles.clear();
	}
	areas.clear();
	areas.push_back(0);
	arcLength.clear();
	arcLength.push_back(0);
	pieCenter = center[bigIdx];
	pieCenter.x = pieCenter.x + ui.spinBox_pieOriginOffX->value();
	pieCenter.y = pieCenter.y + ui.spinBox_pieOriginOffY->value();

	//Compute Angles 
	if (pieType == Area & !optimize)
	{
		//Based on area
		double area = contourArea(contours[bigIdx]); //Total region area
		cv::Rect encRect = boundingRect(contours[bigIdx]);
		char buffer[100];
		sprintf_s(buffer, "Top: %f Bottom: %f\n", (double) encRect.y, (double) encRect.y + encRect.height);
		OutputDebugStringA(buffer);
		double offset = encRect.y; // Start angle of first segment (offset from 0)
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3); // Temporary image to hold each segment of the area to compute the segment area
		for (int j = 0; j < t_val.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);  // Temporary image to hold each segment of the area to compute the segment area
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			double start = offset;
			double stop = start + 1;
			double area_tmp = 0.0;
			angles.push_back(offset);
			while (area_tmp < (area*t_val[j] / total) & stop <= encRect.y + encRect.height)
			{
				drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);	
				cv::Point pt1(encRect.x, start);
				cv::Point pt2(encRect.x + encRect.width, stop);
				rectangle(drawing, pt1, pt2, cv::Scalar(255, 255, 255), -2, 8, 0);
				drawing.copyTo(drawing_tmp, src2);
				cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
				threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
				findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
				stop = stop + 1;
				area_tmp = contourArea(contours[0]);				
			}
			offset = stop - 1;
			areas.push_back(area_tmp);
			
		}
		char buffer3[100];
		sprintf_s(buffer3, "End Compute Area \n");
		OutputDebugStringA(buffer3);
		angles.push_back(encRect.y + encRect.height); // Add 360 to the hardcoded offset
		drawing_tmp.release();
	}
	else if (pieType == Arc & !optimize)
	{
		//Based on Axis Length
		cv::Rect encRect = boundingRect(contours[bigIdx]);
		double offset = encRect.y; // Start angle of first segment (offset from 0)
		angles.push_back(offset);
		cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3); // Temporary image to hold each segment of the area to compute the segment area
		for (int j = 0; j < t_val.size(); j++)
		{
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);  // Temporary image to hold each segment of the area to compute the segment area
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			double start = offset;			
			double area_tmp = 0.0;
			double segHeight = encRect.height*t_val[j] / total;
			angles.push_back(start + segHeight);
			drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
			cv::Point pt1(encRect.x, start);
			cv::Point pt2(encRect.x + encRect.width, start + segHeight);
			rectangle(drawing, pt1, pt2, cv::Scalar(255, 255, 255), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);
			cvtColor(drawing_tmp, drawing_tmp, CV_BGR2GRAY);
			threshold(drawing_tmp, threshold_output, thresh, 255, cv::THRESH_BINARY);
			findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
			area_tmp = contourArea(contours[0]);
			areas.push_back(area_tmp);
			offset = start + segHeight;
		}
		//angles.push_back(360 + pieRot); // Add 360 to the hardcoded offset
		drawing_tmp.release();
	}
	
	//********************************************Compute Errors*************************************************
	
	double totalArea = 0;
	double totalArc = 0;
	for (int j = 1; j < angles.size(); j++) {	
		totalArea = totalArea + areas[j];
		double start = angles[j-1];
		double stop = angles[j];
		totalArc = totalArc + (stop - start);
	}
	
	double errArea = 0;
	double errArc = 0;
	for (int j = 1; j < angles.size(); j++) {
		double d = t_val[j - 1] / total;	
		errArea = errArea + abs((d - areas[j] / totalArea) / d);
		double start = angles[j-1];
		double stop = angles[j];
		errArc = errArc + abs((d - (stop - start) / totalArc) / d);
	}


	ui.label_errVal1->setText(QString::number(roundf((errArea / t_val.size()) * 100), 'g', 2).append("%"));
	ui.label_errVal2->setText(QString::number(roundf((errArc / t_val.size()) * 100), 'g', 2).append("%"));

	int pinX = (int)120 * roundf((errArea / t_val.size()) * 100) / 100;
	ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);
	pinX = (int)120 * roundf((errArc / t_val.size()) * 100) / 100;
	ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);
	   

	//Release memory used by temp images
	src2.release();
	threshold_output.release();
	drawing.release();


	//Release memory used by vectors
	std::vector<std::vector<cv::Point>>().swap(contours);
	std::vector<cv::Vec4i>().swap(hierarchy);
	std::vector<std::vector<cv::Point>>().swap(contours_poly);
	std::vector<cv::Point2f>().swap(center);
	std::vector<float>().swap(radius);
	std::vector<cv::Point>().swap(bigContour);
	std::vector<cv::Point>().swap(bighull);

	double avgError = (errArea + errArc) / (t_val.size());
	ui.label_errVal4->setText(QString::number(100 * avgError / 2, 'g', 2).append("%"));
	pinX = (int)120 * avgError / 2;
	ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);
	return avgError / 2;
}

cv::Mat Infomager::fillVertical()
{
	char buffer2[100];
	sprintf_s(buffer2, "Fill Vertical\n");
	OutputDebugStringA(buffer2);
	cv::Mat image(filtimg.size(), CV_8UC3);
	filtimg.copyTo(image);

	//draw a piechart based on the angles computed
	cv::Mat drawing(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	cv::Point pt;
	pt.x = pieCenter.x;
	pt.y = pieCenter.y;
	for (int j = 0; j < t_val.size(); j++)
	{
		cv::Scalar color = cv::Scalar(pieColors[j].red(), pieColors[j].green(), pieColors[j].blue());
		double start = angles[j];
		double stop = angles[j + 1];

		cv::Point pt1(0, start);
		cv::Point pt2(filtimg.cols, stop);
		rectangle(drawing, pt1, pt2, color, -2, 8, 0);
		//cv::namedWindow("Detected Lines", 1);
		//cv::imshow("Detected Lines", drawing);
		//cv::waitKey();
	}



	//Use the mask to cut out the piechart
	cv::Mat foreground(filtimg.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	drawing.copyTo(foreground, mask);

	//Merge the chart with the image in LAB color space
	cvtColor(image, image, CV_BGR2Lab);
	cvtColor(foreground, foreground, CV_RGB2Lab);

	//Merge the chart with the image in HSV color space

	for (int x = 0; x < mask.rows; x++)
	{
		for (int y = 0; y < mask.cols; y++)
		{
			cv::Vec3b intensity = foreground.at<cv::Vec3b>(x, y);
			if (intensity.val[0] != 0)
			{
				//For LAB
				image.at<cv::Vec3b>(x, y).val[1] = intensity.val[1];
				image.at<cv::Vec3b>(x, y).val[2] = intensity.val[2];					
			}
		}
	}

	cvtColor(image, image, CV_Lab2BGR);

	// Labelling the chart
	scene_infomage->removePieLabels();

	QGraphicsTextItem *tmp;
	double txtAng = 0;
	for (int i = 0; i < t_val.size(); i++)
	{
		tmp = new QGraphicsTextItem(QString::fromStdString(t_label[i]) + ": " + QString("%1").arg(t_val[i]) + "%");
		tmp->setZValue(600);
		txtAng = (angles[i] + pieRot + angles[i + 1] + pieRot) / 2;
		txtAng = (txtAng * PI) / 180.0;
		int txtRad = ui.spinBox_pieLabelRad->value();;
		double x, y;
		if (txtAng >= 2 * PI)
		{
			txtAng = txtAng - floor(txtAng / PI)*PI;
		}
		if (txtAng > PI / 2 && txtAng < 3 * PI / 2)
		{
			x = pieCenter.x + txtRad * cos(txtAng) - tmp->boundingRect().width();
		}
		else
		{
			x = pieCenter.x + txtRad * cos(txtAng);
		}
		if (txtAng > 0 && txtAng < PI)
		{
			y = pieCenter.y + txtRad * sin(txtAng);
		}
		else
		{
			y = pieCenter.y + txtRad * sin(txtAng) - tmp->boundingRect().height();
		}
		tmp->setPos(QPointF(x, y));
		tmp->setFlag(QGraphicsItem::ItemIsSelectable, true);
		tmp->setFlag(QGraphicsItem::ItemIsMovable, true);
		scene_infomage->addItem(tmp);
	}
	return image;
}

void Infomager::generateNeighborVerDiv(std::vector<double> old_angles) {
	std::vector<double> newAngles;
	std::vector<double> randNos;
	newAngles.push_back(old_angles[0]);
	for (int i = 1; i < old_angles.size() - 1; i++) {
		//double rNo = ((double)rand() / (RAND_MAX)) * 16 - 8;
		double rNo = (((double)rand() / (RAND_MAX)) * max_change * 2) - max_change;
		newAngles.push_back(old_angles[i] + rNo);
	}

	for (int i = 0; i < angles.size(); i++) {
		angles[i] = newAngles[i];
	}
}

void Infomager::optimizeVertical() {
	double old_cost = computeVertical();
	double T = 1.0;
	double T_min = 0.00001;
	double alpha = 0.9;
	std::vector<double> curr_angles = angles;
	std::vector<double> old_angles = angles;
	std::vector<double> best_angles = angles;
	double best_cost = old_cost;
	int itrCnt = 0;
	double totDiff = 0;
	double avgDiff = 1;
	while (T > T_min  & avgDiff > 0.0001) {
		int i = 1;
		int j = 1;
		while (i <= 200 & j <= 200) {
			generateNeighborVerDiv(old_angles);
			double new_cost = computeVertical();
			double ap = acceptance_probability(old_cost, new_cost, T);
			double r = ((double)rand() / (RAND_MAX)) + 0.00001;

			if (new_cost < best_cost) {
				best_cost = new_cost;
				best_angles = angles;
			}

			itrCnt += 1;
			totDiff += abs(new_cost - old_cost);
			avgDiff = totDiff / itrCnt;
			if (ap > r) {
				char buffer[100];
				sprintf_s(buffer, "Accept.   Error Diff: %f   AP: %f\n", (new_cost - old_cost), ap);
				OutputDebugStringA(buffer);

				curr_angles = angles;
				old_cost = new_cost;
				i += 1;
				j = 1;
			}
			else {
				char buffer[100];
				sprintf_s(buffer, "Reject.   Error Diff: %f\n", (new_cost - old_cost));
				OutputDebugStringA(buffer);

				angles = curr_angles;
				j += 1;
			}
		}
		T = T * alpha;
	}

	char buffer2[100];
	sprintf_s(buffer2, "Curr Cost: %f\n", old_cost);
	OutputDebugStringA(buffer2);
	sprintf_s(buffer2, "Best Cost: %f \n", best_cost);
	OutputDebugStringA(buffer2);

	angles = best_angles;
	computeVertical();

	QMessageBox::information(0, "info", QString("Optimize done"));
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(this->fillVertical(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
}

//Compute Acceptance Probability
double Infomager::acceptance_probability(double old_cost, double new_cost, double T) {

	if (abs(new_cost - max_cost) > 0.1) {
		return 0;
	}

	if ((new_cost - old_cost) <= 0) {
		return 1.1;
	}

	return exp(-(new_cost - old_cost) / T);
}


//Pie Chart Overlay

void Infomager::on_pushButton_pieOverlay_clicked()
{
	write_text_to_log_file("PieChartSelect\n");
	chartType = Pie;
	pieType = OverLay;

	ui.label_lineOverlay->setVisible(false);
	ui.label_barOverlay->setVisible(false);
	ui.label_pieOverlay->setVisible(true);

	ui.groupBox_LineOverlay->setVisible(false);
	ui.groupBox_BarOverlay->setVisible(false);
	ui.groupBox_PieOverlay->setVisible(true);

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Angle:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Area:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);


	ui.label_err3->setVisible(true);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(false);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(false);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(false);


	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);

	scene_infomage->removePieLabels();
	if (loaded_img)
	{
		scene_infomage->setCurrentBackdrop(filtimg);
		QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
	}

	if (dataLoaded)
	{
		drawOverlayPieChart();
		drawPieChart();
	}
}

void Infomager::drawOverlayPieChart() {
	if (chartAdded)
	{
		chartAdded = false;
		scene_infomage->removeItem(chart);
	}	
		
	//QT Charts Code
	QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

	for (int i = 0; i < t_val.size(); i++)
	{
		series->append(QString::fromStdString(t_label[i]), t_val[i]);
	}

	series->setLabelsVisible(true);
	series->setPieSize(0.7);
	series->setPieStartAngle(ui.horizontalSlider_pieChartRot->value());
	series->setPieEndAngle(360+ui.horizontalSlider_pieChartRot->value());

	chart = new QtCharts::QChart();
	chart->setPreferredSize(260, 180);
	chart->addSeries(series);
	chart->setAnimationOptions(QtCharts::QChart::NoAnimation);
	chart->createDefaultAxes();
	chart->setMargins(QMargins(0, 0, 0, 0));
	chart->legend()->hide();
	chart->setBackgroundVisible(false);
	chart->setFont(textFont);
	chart->setFlag(QGraphicsItem::ItemIsMovable, true);
	chart->setFlag(QGraphicsItem::ItemIsSelectable, true);
	chart->setZValue(999);
	chart->setPos(QPoint(chartX, chartY));
	scene_infomage->addItem(chart);

	scene_infomage->addItem(chart);
	chartAdded = true;	

	findHoughCircle();
}

void Infomager::findHoughCircle() {

	//***************Hough Lines***************
	cv::Mat dst, color_dst;

	cv::Canny(img, dst, 28, 240, 3);
	cvtColor(dst, color_dst, CV_GRAY2BGR);

	std::vector<cv::Vec3f> circles;
	HoughCircles(dst, circles, CV_HOUGH_GRADIENT, 1, dst.rows / 10, 150, 80, 0, 0);

	if (circles.size() > 0)
	{
		//Circles are sorted according to the vote in the accumulator in descending order so pick the first.
		int radius = cvRound(circles[0][2]);
		if (radius*1.5 > dst.cols / 2) {
			radius = dst.cols / 1.5;
		}
		if (radius*1.5 > dst.rows / 2) {
			radius = dst.rows / 1.5;
		}

		ui.spinBox_pieChartRadius->setValue(radius * 1.5);

		chart->setPreferredSize(radius*3, radius*3);
        chartX = circles[0][0] - radius*1.5;
		chartY = circles[0][1] - radius*1.5;
		chart->setPos(QPoint(chartX, chartY));

		

		
	}
	else {
		if (dst.cols > dst.rows) {
			ui.spinBox_pieChartRadius->setValue(dst.rows / 25);
			chart->setPreferredSize(dst.rows / 2, dst.rows / 2);

			chartX = dst.cols / 2 - dst.rows / 4;
			chartY = dst.rows / 2 - dst.rows / 4;

			chart->setPos(QPoint(chartX, chartY));
		}
		else {
			ui.spinBox_pieChartRadius->setValue(dst.cols / 2);
			chart->setPreferredSize(dst.cols / 2, dst.cols / 2);

			chartX = dst.cols / 2 - dst.cols / 4;
			chartY = dst.rows / 2 - dst.cols / 4;

			chart->setPos(QPoint(chartX, chartY));
		}	
	}
}

void Infomager::on_spinBox_pieChartRadius_valueChanged()
{
	if (chartAdded)
	{		
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(ui.spinBox_pieChartRadius->value(), ui.spinBox_pieChartRadius->value());
		scene_infomage->addItem(chart);	
	}
}

void Infomager::on_horizontalSlider_pieChartRot_valueChanged()
{
	if (chartAdded)
	{
		scene_infomage->removeItem(chart);
		//QT Charts Code
		QtCharts::QPieSeries *series = new QtCharts::QPieSeries();

		for (int i = 0; i < t_val.size(); i++)
		{
			series->append(QString::fromStdString(t_label[i]), t_val[i]);
		}

		series->setLabelsVisible(true);
		series->setPieSize(0.7);
		series->setPieStartAngle(ui.horizontalSlider_pieChartRot->value());
		series->setPieEndAngle(360 + ui.horizontalSlider_pieChartRot->value());

		chart->removeAllSeries();
		chart->addSeries(series);
		chart->setPreferredSize(ui.spinBox_pieChartRadius->value(), ui.spinBox_pieChartRadius->value());
		scene_infomage->addItem(chart);
	}
}

//***************Average slope***************
void  Infomager::avgSlope()
{
	if (chartType == Line) {

		//Hieght and Area errors
		double x1 = chart->mapToPosition(QPointF(0, t_val[0]), lineSeries).x();
		double x2 = chart->mapToPosition(QPointF(1, t_val[1]), lineSeries).x();
		double width = x2 - x1;
		double maxH = chart->mapToPosition(QPointF(1, ui.doubleSpinBox_yMin->value()), lineSeries).y();


		double total = 0;
		double totalArea = 0;
		double totalHt = 0;		
		std::vector<double> barHts;
		for (int j = 0; j < t_val.size(); j++)
		{
			total = total + t_val[j];
			double barHt = chart->mapToPosition(QPointF(j, t_val[j]), lineSeries).y();
			barHts.push_back(maxH - barHt);			
			totalHt = totalHt + barHts[j];
		}

		double errHt = 0;
		
		for (int j = 0; j < barHts.size(); j++) {
			double d = t_val[j] / total;		
			errHt = errHt + abs((d - barHts[j] / totalHt) / d);
		}

		ui.label_errVal2->setText(QString::number((floorf(errHt * 100) / 100), 'g', 2).append("%"));

		//***************Average slope***************
		int n = t_val.size();
		double avg_slope = 0.0;
		double median_slope = 0.0;
		double avg_orient = 0.0;
		double xMax = -INFINITY;
		double yMax = -INFINITY;
		double xMin = INFINITY;
		double yMin = INFINITY;
		std::vector<double> slopes;
		double aspectRatio = (double)ui.spinBox_lineChartWidth->value() / ui.spinBox_lineChartHeight->value();
		int n_slopes = 0;
		for (int i = 0; i < n - 1; i++)
		{
			double x1 = chart->mapToPosition(QPointF(i, t_val[i]), lineSeries).x();
			double y1 = chart->mapToPosition(QPointF(i, t_val[i]), lineSeries).y();
			double x2 = chart->mapToPosition(QPointF(i + 1, t_val[i + 1]), lineSeries).x();
			double y2 = chart->mapToPosition(QPointF(i + 1, t_val[i + 1]), lineSeries).y();

			if ((y2 - y1) != 0) {
				slopes.push_back(abs((y2 - y1) / (x2 - x1)));
				double orient = atan(slopes[i] / aspectRatio) * 180 / PI;
				avg_orient = avg_orient + abs(orient);
				n_slopes++;
			}

		}
		avg_orient = avg_orient / n_slopes;
		std::sort(slopes.begin(), slopes.end());
		size_t size = slopes.size();

		if (size % 2 == 0)
		{
			median_slope = (slopes[size / 2 - 1] + slopes[size / 2]) / 2;
		}
		else
		{
			median_slope = slopes[size / 2];
		}

		double OrientErr = abs((45 - avg_orient) / 45);
		ui.label_errVal1->setText(QString::number(roundf(100 * OrientErr), 'g', 2).append("%"));
		int pinX = (int)120 * OrientErr;
		ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);

		double avgError = 0.0;
		avgError += (floorf(errHt * 100) / 100) / 100;
		pinX = (int)120 * avgError;
		ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);
		avgError += OrientErr;
		ui.label_errVal4->setText(QString::number(100 * avgError / 2, 'g', 2).append("%"));
		pinX = (int)120 * avgError / 2;
		ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);
	}
}

void Infomager::optimzeAspectRatio(bool isBar) {
	double Rx = 1.0;
	double Ry = 1.0;
	double aspectRatio = 1.0;

	Rx = t_val.size(); 
	auto biggest = std::max_element(std::begin(t_val), std::end(t_val));
	auto smallest = std::min_element(std::begin(t_val), std::end(t_val));
	Ry = (double) *biggest - (double)*smallest;

	std::vector<double> slopes;	
	for (int i = 1; i < t_val.size(); i++)
	{
		double x1 = i-1;
		double y1 = t_val[i - 1];
		double x2 = i;
		double y2 = t_val[i];

		if ((y2 - y1) != 0) {
			slopes.push_back(abs((y2 - y1) / (x2 - x1)));	
		}
	}

	std::sort(slopes.begin(), slopes.end());	
	size_t size = slopes.size();

	double median_slope = 0.0;
	if (size % 2 == 0)
	{
		median_slope = (slopes[size / 2 - 1] + slopes[size / 2]) / 2;
	}
	else
	{
		median_slope = slopes[size / 2];
	}

	aspectRatio = abs(median_slope) * Rx / Ry;
	
	double width = (double) ui.spinBox_lineChartWidth->value();
	double height = (double) ui.spinBox_lineChartHeight->value();
	if (isBar) {
		width = (double)ui.spinBox_barChartWidth->value();
		height = (double)ui.spinBox_barChartHeight->value();
	}
	
	double newWidth = aspectRatio * height;
	double newHeight = width / aspectRatio;

	if (newHeight < imgH)
	{
		ui.spinBox_lineChartWidth->setValue(width);
		ui.spinBox_lineChartHeight->setValue(newHeight);
		ui.spinBox_barChartWidth->setValue(width);
		ui.spinBox_barChartHeight->setValue(newHeight);
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(width, newHeight);
		scene_infomage->addItem(chart);
	}
	else if (newWidth < imgW) {
		ui.spinBox_lineChartWidth->setValue(newWidth);
		ui.spinBox_lineChartHeight->setValue(height);
		ui.spinBox_barChartWidth->setValue(newWidth);
		ui.spinBox_barChartHeight->setValue(height);
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(newWidth, height);
		scene_infomage->addItem(chart);
	}
}

//Line Chart
void Infomager::on_pushButton_lineOverlay_clicked()
{
	write_text_to_log_file("LineChartSelect\n");
	chartType = Line;
	
	ui.label_lineOverlay->setVisible(true);
	ui.label_barOverlay->setVisible(false);
	ui.label_pieOverlay->setVisible(false);
	
	ui.groupBox_LineOverlay->setVisible(true);
	ui.groupBox_BarOverlay->setVisible(false);
	ui.groupBox_PieOverlay->setVisible(false);

	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Orientation:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Height:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);


	ui.label_err3->setVisible(false);
	ui.label_err3->setText(QString("Arc Length:"));
	ui.label_errVal3->setVisible(false);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(false);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(false);


	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);

	scene_infomage->removePieLabels();
	if (loaded_img)
	{
		scene_infomage->setCurrentBackdrop(filtimg);
		QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(filtimg.clone(), QImage::Format_RGB888).rgbSwapped());
		scene_infomage->refreshBackrop(filt_backdrop);
	}

	if (dataLoaded)
	{
		drawLineChart();
	}	
}

void Infomager::drawLineChart()
{
	if (chartAdded)
	{
		chartAdded = false;
		scene_infomage->removeItem(chart);		
	}
	if (chartPrwAdded)
	{
		chartPrwAdded = false;
		scene_preview->removeItem(chartpreview);
		chartpreview = 0;
	}
	//QT Charts Code
	chart = new QtCharts::QChart();
	if (lineSpline)
	{
		lineSeries = new QtCharts::QSplineSeries();
		lineGlowSeries = new QtCharts::QSplineSeries();
	}
	else
	{
		lineSeries = new QtCharts::QLineSeries();
		lineGlowSeries = new QtCharts::QLineSeries();
	}
	xaxis = new QtCharts::QCategoryAxis();
	QtCharts::QLineSeries *seriesPrvw = new QtCharts::QLineSeries();
	QtCharts::QCategoryAxis *xaxisPrvw = new QtCharts::QCategoryAxis();
	
	for (int i = 0; i < t_val.size(); i++)
	{
		lineSeries->append(i,t_val[i]);
		lineGlowSeries->append(i, t_val[i]);
		seriesPrvw->append(i, t_val[i]);
	}
	for (int i = 0; i < t_val.size(); i=i+xStep)
	{
		xaxis->append(QString::fromStdString(t_label[i]), i);
		xaxisPrvw->append(QString::fromStdString(t_label[i]), i);
	}	
	
	QPen pen(lineColor);
	pen.setWidth(lineThick);
	lineSeries->setPen(pen);
	QPen penG(lineGlowColor);
	penG.setWidth(lineGlowThick);
	lineGlowSeries->setPen(penG);
	chart->addSeries(lineGlowSeries);
	chart->addSeries(lineSeries);
	chart->setPreferredSize(ui.spinBox_lineChartWidth->value(), ui.spinBox_lineChartHeight->value());
	chart->setAnimationOptions(QtCharts::QChart::NoAnimation);
	chart->createDefaultAxes();	
	xaxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
	chart->removeAxis(chart->axisX());
	chart->setAxisX(xaxis, lineSeries);
	chart->axisX()->setGridLineVisible(false);
	chart->axisY()->setGridLineVisible(false);
	chart->axisY()->setMin(ui.doubleSpinBox_yMin->value());
	chart->axisY()->setMax(ui.doubleSpinBox_yMax->value());
			
	chart->setMargins(QMargins(0, 0, 0, 0));
	chart->legend()->hide();
	chart->setBackgroundVisible(false);
	chart->setFont(textFont);	
	chart->setFlag(QGraphicsItem::ItemIsMovable, true);
	chart->setFlag(QGraphicsItem::ItemIsSelectable, true);
	chart->setZValue(999);	
	chart->setPos(QPoint(chartX, chartY));
	scene_infomage->addItem(chart);
	chartAdded = true;	

	chartpreview = new QtCharts::QChart();
	chartpreview->setPreferredSize(260, 160);
	chartpreview->addSeries(seriesPrvw);
	chartpreview->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
	chartpreview->createDefaultAxes();
	xaxisPrvw->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
	chartpreview->removeAxis(chart->axisX());
	chartpreview->setAxisX(xaxisPrvw, seriesPrvw);
	chartpreview->setMargins(QMargins(0, 0, 0, 0));
	chartpreview->legend()->hide();
	chartpreview->setBackgroundVisible(false);
	scene_preview->addItem(chartpreview);	
	chartPrwAdded = true;
}

void Infomager::on_spinBox_lineChartHeight_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("LineChartHeightChng\n");
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(ui.spinBox_lineChartWidth->value(), ui.spinBox_lineChartHeight->value());
		scene_infomage->addItem(chart);
		avgSlope();
	}
}

void Infomager::on_spinBox_lineChartWidth_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("LineChartWidthChng\n");
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(ui.spinBox_lineChartWidth->value(), ui.spinBox_lineChartHeight->value());
		scene_infomage->addItem(chart);
		avgSlope();
	}
}

void Infomager::on_pushButton_lineColor_clicked()
{
	write_text_to_log_file("LineChartLineColorChng\n");
	lineColor = QColorDialog::getColor(lineColor, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(lineColor.red()).arg(lineColor.green()).arg(lineColor.blue());
	ui.label_lineColor->setStyleSheet(style);	
	QPen pen(lineColor);
	pen.setWidth(lineThick);
	lineSeries->setPen(pen);
}

void Infomager::on_horizontalSlider_lineThick_valueChanged()
{
	write_text_to_log_file("LineChartLineThickChng\n");
	lineThick = ui.horizontalSlider_lineThick->value();
	chartX = chart->pos().x();
	chartY = chart->pos().y();
	drawLineChart();
}

void Infomager::on_pushButton_lineGlowColor_clicked()
{
	write_text_to_log_file("LineChartGlowColorChng\n");
	lineGlowColor = QColorDialog::getColor(lineGlowColor, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(lineGlowColor.red()).arg(lineGlowColor.green()).arg(lineGlowColor.blue());
	ui.label_lineGlowColor->setStyleSheet(style);
	lineGlowColor.setAlpha(150);
	QPen pen(lineGlowColor);
	pen.setWidth(lineGlowThick);
	lineGlowSeries->setPen(pen);
}

void Infomager::on_horizontalSlider_lineGlowThick_valueChanged()
{
	write_text_to_log_file("LineChartGlowThickChng\n");
	lineGlowThick = ui.horizontalSlider_lineGlowThick->value();
	chartX = chart->pos().x();
	chartY = chart->pos().y();
	drawLineChart();
}

void Infomager::on_horizontalSlider_xTicks_valueChanged()
{
	write_text_to_log_file("LineChartXTicksChng\n");
	xStep = ui.horizontalSlider_xTicks->value();
	drawLineChart();
	avgSlope();
}

void Infomager::on_doubleSpinBox_yMin_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("LineChartYMinChng\n");
		chartX = chart->pos().x();
		chartY = chart->pos().y();
		drawLineChart();
		avgSlope();
	}
}

void Infomager::on_doubleSpinBox_yMax_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("LineChartYMaxChng\n");
		chartX = chart->pos().x();
		chartY = chart->pos().y();
		drawLineChart();
		avgSlope();
	}
}

void Infomager::on_pushButton_lineXColor_clicked()
{
	write_text_to_log_file("LineChartXAxisColorChng\n");
	QColor color = QColorDialog::getColor(Qt::white, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.label_lineXColor->setStyleSheet(style);
	chart->axisX()->setLinePenColor(color);
}

void Infomager::on_pushButton_lineYColor_clicked()
{
	write_text_to_log_file("LineChartYAxisColorChng\n");
	QColor color = QColorDialog::getColor(Qt::white, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.label_lineYColor->setStyleSheet(style);	
	chart->axisY()->setLinePenColor(color);
}

void Infomager::on_pushButton_lineLine_clicked()
{
	write_text_to_log_file("LineChartToLineChng\n");
	lineSpline = false;
	ui.label_lineLine->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_lineSpline->setStyleSheet("background: rgb(65, 65, 65);");
	chartX = chart->pos().x();
	chartY = chart->pos().y();
	drawLineChart();
}

void Infomager::on_pushButton_lineSpline_clicked()
{
	write_text_to_log_file("LineChartToSplineChng\n");
	lineSpline = true;
	ui.label_lineSpline->setStyleSheet("background: rgb(240, 240, 240);");
	ui.label_lineLine->setStyleSheet("background: rgb(65, 65, 65);");
	chartX = chart->pos().x();
	chartY = chart->pos().y();
	drawLineChart();
}

//bar Chart

void Infomager::on_pushButton_barFill_clicked()
{
	if (t_val.size() < 7)
	{
		write_text_to_log_file("BarChartFillSelect\n");
		chartType = Bar;
		barOverlay = false;
		ui.label_pieFill->setVisible(false);
		ui.label_barFill->setVisible(true);

		ui.groupBox_PieFill->setVisible(false);
		ui.groupBox_BarFill->setVisible(true);

		ui.label_err1->setVisible(true);
		ui.label_err1->setText(QString("Area:"));
		ui.label_errVal1->setText(QString("0%"));
		ui.label_errorPin1->setGeometry(0, 10, 4, 22);

		ui.label_err2->setVisible(true);
		ui.label_err2->setText(QString("Length:"));		
		ui.label_errVal2->setText(QString("0%"));
		ui.label_errorPin2->setGeometry(0, 47, 4, 22);

		ui.label_err3->setVisible(false);
		ui.label_err3->setText(QString("Arc Length:"));
		ui.label_errVal3->setVisible(false);
		ui.label_errVal3->setText(QString("0%"));
		ui.label_errorPin3->setVisible(false);
		ui.label_errorPin3->setGeometry(0, 84, 4, 22);
		ui.label_errorBar3->setVisible(false);

		ui.label_err4->setVisible(true);
		ui.label_err4->setText(QString("Avg.:"));
		ui.label_errVal4->setText(QString("0%"));
		ui.label_errorPin4->setGeometry(0, 121, 4, 22);




		if (chartAdded)
		{
			chartAdded = false;
			scene_infomage->removeItem(chart);
		}
		if (dataLoaded)
		{
			barMaskIdx = 0;
			ui.label_barCurrLabel->setText(QString::fromStdString(t_label[barMaskIdx]));
			if (loaded_img)
			{
				barMasks.clear();
				for (int i = 0; i < t_label.size(); i++)
				{
					barMasks.push_back(img.clone());
					barMaskAreas.push_back(-1.0);
					barMaskHeights.push_back(-1.0);
					barMasksSet.push_back(false);
					barMasks[i] = cv::Scalar(0, 0, 0);
					cv::cvtColor(barMasks[i], barMasks[i], CV_RGB2GRAY);
				}
			}
		}
	}
	else
	{
		write_text_to_log_file("BarChartFillSelectDenied\n");
		QMessageBox::information(0, "Alert", QString("Too many items for a fill style bar chart."));
	}
}

void Infomager::on_pushButton_barOverlay_clicked()
{
	write_text_to_log_file("BarOverlayChartSelect\n");
	chartType = Bar;
	barOverlay = true;
	ui.label_lineOverlay->setVisible(false);
	ui.label_barOverlay->setVisible(true);
	ui.label_pieOverlay->setVisible(false);

	ui.groupBox_LineOverlay->setVisible(false);
	ui.groupBox_BarOverlay->setVisible(true);
	ui.groupBox_PieOverlay->setVisible(false);
	
	ui.label_err1->setVisible(true);
	ui.label_err1->setText(QString("Orientation:"));
	ui.label_errVal1->setText(QString("0%"));
	ui.label_errorPin1->setGeometry(0, 10, 4, 22);

	ui.label_err2->setVisible(true);
	ui.label_err2->setText(QString("Height:"));
	ui.label_errVal2->setText(QString("0%"));
	ui.label_errorPin2->setGeometry(0, 47, 4, 22);

	ui.label_err3->setVisible(true);
	ui.label_err3->setText(QString("Area:"));
	ui.label_errVal3->setVisible(true);
	ui.label_errVal3->setText(QString("0%"));
	ui.label_errorPin3->setVisible(true);
	ui.label_errorPin3->setGeometry(0, 84, 4, 22);
	ui.label_errorBar3->setVisible(true);

	ui.label_err4->setVisible(true);
	ui.label_err4->setText(QString("Avg.:"));
	ui.label_errVal4->setText(QString("0%"));
	ui.label_errorPin4->setGeometry(0, 121, 4, 22);

	scene_infomage->removePieLabels();

	if (dataLoaded)
	{
		drawBarChart();
	}
}

double Infomager::barError()
{
	if (chartType == Bar && barOverlay)
	{
		//Hieght and Area errors
		double x1 = chart->mapToPosition(QPointF(0, t_val[0]), barSeries).x();
		double x2 = chart->mapToPosition(QPointF(1, t_val[1]), barSeries).x();
		double width = x2 - x1;
		double maxH =  chart->mapToPosition(QPointF(1, barAxisY->min()), barSeries).y();		
		
		double total = 0;
		double totalArea = 0;
		double totalHt = 0;
		std::vector<double> barAreas;
		std::vector<double> barHts;
		for (int j = 0; j < t_val.size(); j++)
		{
			total = total + t_val[j];
			double barHt = chart->mapToPosition(QPointF(j, t_val[j]), barSeries).y();
			double tmpArea = (width * 0.5) * (maxH - barHt);
			barAreas.push_back(tmpArea);
			barHts.push_back(maxH - barHt);
			totalArea = totalArea + tmpArea;
			totalHt = totalHt + barHts[j];
		}

		double errHt = 0;
		double errArea = 0;
		for (int j = 0; j < barAreas.size(); j++) {
			double d = t_val[j] / total;			
			errArea = errArea + abs((d - barAreas[j] / totalArea) / d);
			errHt = errHt + abs((d - barHts[j] / totalHt) / d);
		}
	

		//45 degree banking
		int n = t_val.size();
		double avg_slope = 0.0;
		double median_slope = 0.0;
		double avg_orient = 0.0;
		double xMax = -INFINITY;
		double yMax = -INFINITY;
		double xMin = INFINITY;
		double yMin = INFINITY;
		std::vector<double> slopes;
		double aspectRatio = (double)ui.spinBox_barChartWidth->value() / ui.spinBox_barChartHeight->value();
		int n_slopes = 0;
		for (int i = 0; i < n - 1; i++)
		{
			double x1 = chart->mapToPosition(QPointF(i, t_val[i]), barSeries).x();
			double y1 = chart->mapToPosition(QPointF(i, t_val[i]), barSeries).y();
			double x2 = chart->mapToPosition(QPointF(i + 1, t_val[i + 1]), barSeries).x();
			double y2 = chart->mapToPosition(QPointF(i + 1, t_val[i + 1]), barSeries).y();

			if ((y2 - y1) != 0) {
				slopes.push_back(abs((y2 - y1) / (x2 - x1)));
				double orient = atan(slopes[i] / aspectRatio) * 180 / PI;
				avg_orient = avg_orient + abs(orient);
				n_slopes++;
			}

		}
		avg_orient = avg_orient / n_slopes;
		std::sort(slopes.begin(), slopes.end());
		size_t size = slopes.size();

		if (size % 2 == 0)
		{
			median_slope = (slopes[size / 2 - 1] + slopes[size / 2]) / 2;
		}
		else
		{
			median_slope = slopes[size / 2];
		}

		double OrientErr = abs((45 - avg_orient) / 45);
		ui.label_errVal1->setText(QString::number(roundf(100 * OrientErr), 'g', 2).append("%"));
		int pinX = (int) 120 * OrientErr;
		ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);

		
		ui.label_errVal2->setText(QString::number((floorf(errHt * 100) / 100), 'g', 2).append("%"));
		ui.label_errVal3->setText(QString::number((floorf(errArea * 100) / 100), 'g', 2).append("%"));

		
		pinX = (int)120 * (floorf(errHt * 100) / 100) / 100;
		ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);
		pinX = (int)120 * (floorf(errArea * 100) / 100) / 100;
		ui.label_errorPin3->setGeometry(pinX, 84, 4, 22);

		double avgError = 0.0;
		avgError += (floorf(errHt * 100) / 100) / 100;
		avgError += (floorf(errArea * 100) / 100) / 100;
		avgError += OrientErr;			
		ui.label_errVal4->setText(QString::number(100 * avgError/3, 'g', 2).append("%"));
		pinX = (int)120 * avgError/3;
		ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);

	}
	if (chartType == Bar && !barOverlay) {
		double total = 0;
		double totalArea = 0;
		double totalHt = 0;
		for (int j = 0; j < t_val.size(); j++)
		{
			if (barMaskAreas[j] > 0 & barMaskHeights[j] > 0)
			{
				total = total + t_val[j];
				totalArea = totalArea + barMaskAreas[j];
				totalHt = totalHt + barMaskHeights[j];
			}
		}

		double errHt = 0;
		double errArea = 0;
		for (int j = 0; j < barMaskAreas.size(); j++)
		{
			if (barMaskAreas[j] > 0 & barMaskHeights[j] > 0)
			{
				double d = t_val[j] / total;
				errArea = errArea + abs((d - barMaskAreas[j] / totalArea) / d);
				errHt = errHt + abs((d - barMaskHeights[j] / totalHt) / d);
			}
		}

		ui.label_errVal1->setText(QString::number(errArea * 100, 'g', 2).append("%"));
		ui.label_errVal2->setText(QString::number(errHt * 100, 'g', 2).append("%"));
		
		int pinX = (int)120 * errArea;
		ui.label_errorPin1->setGeometry(pinX, 10, 4, 22);
		pinX = (int)120 * errHt;
		ui.label_errorPin2->setGeometry(pinX, 47, 4, 22);
		

		double avgError = (errArea + errHt) / 2;
		ui.label_errVal4->setText(QString::number(100 * avgError, 'g', 2).append("%"));
		pinX = (int)120 * avgError;
		ui.label_errorPin4->setGeometry(pinX, 121, 4, 22);
		return avgError;
	}	
	return 0;
}

void Infomager::drawBarChart()
{
	if (chartAdded)
	{
		chartAdded = false;
		scene_infomage->removeItem(chart);		
	}
	if (chartPrwAdded)
	{
		chartPrwAdded = false;
		scene_preview->removeItem(chartpreview);
		chartpreview = 0;
	}
	//QT Charts Code
	chart = new QtCharts::QChart();
	barSeries = new QtCharts::QBarSeries();
	barSet = new QtCharts::QBarSet("Bar");
	QStringList categories;
	QtCharts::QBarSet *barSetPrw = new QtCharts::QBarSet("Bar2");
	QStringList categoriesPrw;

	for (int i = 0; i < t_val.size(); i++)
	{
		categories.append(QString::fromStdString(t_label[i]));
		barSet->append(t_val[i]);
		categoriesPrw.append(QString::fromStdString(t_label[i]));
		barSetPrw->append(t_val[i]);
	}	
	
	barSet->setColor(barColor);
	barSet->setBorderColor(barGlowColor);
	barSeries->append(barSet);
	
	chart->setPreferredSize(ui.spinBox_barChartWidth->value(), ui.spinBox_barChartHeight->value());
	chart->addSeries(barSeries);
	chart->setAnimationOptions(QtCharts::QChart::NoAnimation);
	QtCharts::QBarCategoryAxis *axis = new QtCharts::QBarCategoryAxis();	
	barAxisY = new QtCharts::QValueAxis();
	axis->append(categories);
	axis->setGridLineVisible(false);
	chart->createDefaultAxes();
	chart->setAxisX(axis, barSeries);
	chart->setAxisY(barAxisY, barSeries);
	chart->axisX()->setLabelsAngle(45);
	chart->axisY()->setGridLineVisible(false);	
	chart->setMargins(QMargins(0, 0, 0, 0));
	chart->legend()->setVisible(false);	
	chart->setBackgroundVisible(false);
	chart->setFont(textFont);
	chart->setFlag(QGraphicsItem::ItemIsMovable, true);
	chart->setFlag(QGraphicsItem::ItemIsSelectable, true);
	chart->setZValue(999);	
	chart->setPos(QPoint(chartX, chartY));
	chart->axisY()->setMin(ui.doubleSpinBox_yMinBarOverlay->value());
	chart->axisY()->setMax(ui.doubleSpinBox_yMaxBarOverlay->value());
	scene_infomage->addItem(chart);
	chartAdded = true;	
	
	chartpreview = new QtCharts::QChart();
	QtCharts::QBarSeries *seriesPrvw = new QtCharts::QBarSeries();
	seriesPrvw->append(barSetPrw);
	chartpreview->setPreferredSize(260, 160);
	chartpreview->addSeries(seriesPrvw);
	chartpreview->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
	QtCharts::QBarCategoryAxis *axisPrw = new QtCharts::QBarCategoryAxis();	
	axisPrw->append(categoriesPrw);
	axisPrw->setGridLineVisible(false);
	chartpreview->createDefaultAxes();
	chartpreview->setAxisX(axisPrw, seriesPrvw);
//	chartpreview->axisX()->setLabelsAngle(45);
	chartpreview->axisY()->setGridLineVisible(false);
	chartpreview->setMargins(QMargins(0, 0, 0, 0));
	chartpreview->legend()->setVisible(true);
	chartpreview->setBackgroundVisible(false);
	scene_preview->addItem(chartpreview);
	chartPrwAdded = true;
	barError();
}

void Infomager::on_spinBox_barChartHeight_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("BarChartHeightChng\n");
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(ui.spinBox_barChartWidth->value(), ui.spinBox_barChartHeight->value());
		scene_infomage->addItem(chart);
		barError();
	}
}

void Infomager::on_spinBox_barChartWidth_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("BarChartWidthChng\n");
		scene_infomage->removeItem(chart);
		chart->setPreferredSize(ui.spinBox_barChartWidth->value(), ui.spinBox_barChartHeight->value());
		scene_infomage->addItem(chart);
		barError();
	}
}

void Infomager::on_pushButton_barColor_clicked()
{
	write_text_to_log_file("BarChartBarColorChng\n");
	barColor = QColorDialog::getColor(barColor, this, "Bar Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(barColor.red()).arg(barColor.green()).arg(barColor.blue());
	ui.label_barColor->setStyleSheet(style);	
	barSet->setColor(barColor);
}

void Infomager::on_horizontalSlider_barWidth_valueChanged()
{
	write_text_to_log_file("BarChartBarWidthChng\n");
	barThick = ui.horizontalSlider_barWidth->value();
	barSeries->setBarWidth((double) barThick/100);
	barError();
}

void Infomager::on_pushButton_barGlowColor_clicked()
{
	write_text_to_log_file("BarChartGlowColorChng\n");
	barGlowColor = QColorDialog::getColor(barGlowColor, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(barGlowColor.red()).arg(barGlowColor.green()).arg(barGlowColor.blue());
	ui.label_barGlowColor->setStyleSheet(style);	
	QPen pen(barGlowColor);
	pen.setWidth(barGlowThick);
	barSet->setPen(pen);
}

void Infomager::on_horizontalSlider_barGlowWidth_valueChanged()
{
	write_text_to_log_file("BarChartGlowWidthChng\n");
	barGlowThick= ui.horizontalSlider_barGlowWidth->value();
	QPen pen(barGlowColor);
	pen.setWidth(barGlowThick);
	barSet->setPen(pen);
}

void Infomager::on_pushButton_barXColor_clicked()
{
	write_text_to_log_file("BarChartXAxisColorChng\n");
	QColor color = QColorDialog::getColor(Qt::white, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.label_barXColor->setStyleSheet(style);
	chart->axisX()->setLinePenColor(color);
}

void Infomager::on_pushButton_barYColor_clicked()
{
	write_text_to_log_file("BarChartYAxisColorChng\n");
	QColor color = QColorDialog::getColor(Qt::white, this, "Line Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(color.red()).arg(color.green()).arg(color.blue());
	ui.label_barYColor->setStyleSheet(style);
	chart->axisY()->setLinePenColor(color);
}

void Infomager::on_doubleSpinBox_yMinBarOverlay_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("BarChartOverlayYMinChng\n");
		chartX = chart->pos().x();
		chartY = chart->pos().y();
		drawBarChart();
	}
}

void Infomager::on_doubleSpinBox_yMaxBarOverlay_valueChanged()
{
	if (chartAdded)
	{
		write_text_to_log_file("BarChartOverlayYMaxChng\n");
		chartX = chart->pos().x();
		chartY = chart->pos().y();
		drawBarChart();
	}
}

void Infomager::on_pushButton_barMaskPrev_clicked()
{
	if (dataLoaded)
	{
		write_text_to_log_file("BarChartFillPrevMask\n");
		if (barMaskIdx > 0)
		{
			barMaskIdx = barMaskIdx - 1;
			ui.label_barCurrLabel->setText(QString::fromStdString(t_label[barMaskIdx]));
			if (loaded_img)
			{
				cv::Mat bckdrp;
				barMasks[barMaskIdx].copyTo(bckdrp);
				cv::cvtColor(bckdrp, bckdrp, CV_GRAY2RGB);
				QPixmap pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(bckdrp, QImage::Format_RGB888).rgbSwapped());
				ui.label_barMask->setPixmap(pix_backdrop);
			}
		}		
	}
}

void Infomager::on_pushButton_barMaskNext_clicked()
{
	if (dataLoaded)
	{
		write_text_to_log_file("BarChartFillNextMask\n");
		if (barMaskIdx < t_label.size()-1)
		{
			barMaskIdx = barMaskIdx + 1;
			ui.label_barCurrLabel->setText(QString::fromStdString(t_label[barMaskIdx]));
			if (loaded_img)
			{
				cv::Mat bckdrp;
				barMasks[barMaskIdx].copyTo(bckdrp);
				cv::cvtColor(bckdrp, bckdrp, CV_GRAY2RGB);
				QPixmap pix_backdrop = QPixmap::fromImage(mat_to_qimage_ref(bckdrp, QImage::Format_RGB888).rgbSwapped());
				ui.label_barMask->setPixmap(pix_backdrop);
			}
		}
	}
}

double Infomager::fillBarChart(int idx)
{	
	cv::Mat src2;
	int thresh = 100;
	int max_thresh = 255;
	// Convert image to gray and blur it
	cvtColor(barMasks[idx], src2, CV_GRAY2RGB);

	cv::Mat threshold_output;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	// Detect edges using Threshold
	threshold(barMasks[idx], threshold_output, thresh, 255, cv::THRESH_BINARY);
	// Find contours
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	// Approximate contours to polygons + get bounding rects and circles
	std::vector<std::vector<cv::Point> > contours_poly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());

	int b = 0;
	double ar = 0.0;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));		
		if (ar < contourArea(contours[i]))
		{
			ar = contourArea(contours[i]);
			b = i;
		}
	}

	// Draw biggest contour 	
	src2 = cv::Mat::zeros(src2.size(), CV_8UC3);
	drawContours(src2, contours, b, cv::Scalar(255, 255, 255), -2, 8, hierarchy, 0, cv::Point());

	double total = 0;

	for (int j = 0; j<t_val.size(); j++)
	{
		total = total + t_val[j];
	}

	double t_valMax = 1;

	for (int j = 0; j < t_val.size(); j++)
	{
		if (t_val[j] > t_valMax) {
			t_valMax = t_val[j];
		}
	}

	double offset = 0.0;
	cv::Mat drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	cv::Mat drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
	cv::Point labelPos(boundRect[b].x, 10);

	//Compute fill amount
	if (barDirection == BarVerti) {
		if (barType == BarHeight) {
			//Based on height
			double yMin = ui.doubleSpinBox_yMinBarFill->value();
			double yMax = ui.doubleSpinBox_yMaxBarFill->value();

			int top = boundRect[b].y + boundRect[b].height - ((t_val[idx] / t_valMax) * boundRect[b].height);
			if (!barPercentage) {
				top = barFillYMax - (((t_val[idx] - yMin) / (yMax - yMin)) * (barFillYMax - barFillYMin));
			}
			cv::Point p1(boundRect[b].x, top);
			cv::Point p2(boundRect[b].x + boundRect[b].width, boundRect[b].y + boundRect[b].height);
			labelPos = p1;
			barMaskHeights[idx] = (boundRect[b].y + boundRect[b].height) - top;

			rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);

			src2 = cv::Mat::zeros(src2.size(), CV_8UC1);
			cvtColor(drawing_tmp, src2, CV_BGR2GRAY);

			cv::Mat threshold_output2;
			std::vector<std::vector<cv::Point> > contours2;
			std::vector<cv::Vec4i> hierarchy2;

			// Detect edges using Threshold
			threshold(src2, threshold_output2, thresh, 255, cv::THRESH_BINARY);
			// Find contours
			findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

			barMaskAreas[idx] = -1.0;
			for (int i = 0; i < contours2.size(); i++)
			{
				if (barMaskAreas[idx] < contourArea(contours2[i]))
				{
					barMaskAreas[idx] = contourArea(contours2[i]);
				}
			}
		}
		else if (barType == BarArea) {
			//Based on Area
			double mux = 0.005;
			int top = boundRect[b].y + boundRect[b].height - (mux * boundRect[b].height);
			double ht = 1;
			double area = contourArea(contours[b]);
			double cutOff = (t_val[idx] / t_valMax);
			double yMin = ui.doubleSpinBox_yMinBarFill->value();
			double yMax = ui.doubleSpinBox_yMaxBarFill->value();
			if (!barPercentage) {
				cutOff = ((t_val[idx] - yMin) / (yMax - yMin));
			}
			barMaskAreas[idx] = -1.0;
			while ((barMaskAreas[idx] / area) < cutOff && top >= 0) {
				drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				cv::Point p1(boundRect[b].x, top);
				cv::Point p2(boundRect[b].x + boundRect[b].width, boundRect[b].y + boundRect[b].height);
				labelPos = p1;
				barMaskHeights[idx] = (boundRect[b].y + boundRect[b].height) - top;
				rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
				drawing.copyTo(drawing_tmp, src2);

				//Based on Area		

				cv::Mat src3 = cv::Mat::zeros(src2.size(), CV_8UC1);
				cvtColor(drawing_tmp, src3, CV_BGR2GRAY);

				cv::Mat threshold_output2;
				std::vector<std::vector<cv::Point> > contours2;
				std::vector<cv::Vec4i> hierarchy2;

				// Detect edges using Threshold
				threshold(src3, threshold_output2, thresh, 255, cv::THRESH_BINARY);
				// Find contours
				findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

				barMaskAreas[idx] = -1.0;
				for (int i = 0; i < contours2.size(); i++)
				{
					if (barMaskAreas[idx] < contourArea(contours2[i]))
					{
						barMaskAreas[idx] = contourArea(contours2[i]);
					}
				}
				mux = mux + 0.005;
				top = boundRect[b].y + boundRect[b].height - (mux * boundRect[b].height);
			}

		}
		else if (barType == BarOptimize) {
			//Based on mask height

			int top = boundRect[b].y + boundRect[b].height - barMaskHeights[idx];

			cv::Point p1(boundRect[b].x, top);
			cv::Point p2(boundRect[b].x + boundRect[b].width, boundRect[b].y + boundRect[b].height);
			labelPos = p1;

			rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);

			src2 = cv::Mat::zeros(src2.size(), CV_8UC1);
			cvtColor(drawing_tmp, src2, CV_BGR2GRAY);

			cv::Mat threshold_output2;
			std::vector<std::vector<cv::Point> > contours2;
			std::vector<cv::Vec4i> hierarchy2;

			// Detect edges using Threshold
			threshold(src2, threshold_output2, thresh, 255, cv::THRESH_BINARY);
			// Find contours
			findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

			barMaskAreas[idx] = -1.0;
			for (int i = 0; i < contours2.size(); i++)
			{
				if (barMaskAreas[idx] < contourArea(contours2[i]))
				{
					barMaskAreas[idx] = contourArea(contours2[i]);
				}
			}
		}
	}
	else {
		if (barType == BarHeight) {
			//Based on height

			int right = boundRect[b].x + ((t_val[idx] / t_valMax) * boundRect[b].width);
		
			cv::Point p1(boundRect[b].x, boundRect[b].y);
			cv::Point p2(right, boundRect[b].y + boundRect[b].height);
			labelPos = p1;
			barMaskHeights[idx] = (t_val[idx] / t_valMax) * boundRect[b].width;

			rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);

			src2 = cv::Mat::zeros(src2.size(), CV_8UC1);
			cvtColor(drawing_tmp, src2, CV_BGR2GRAY);

			cv::Mat threshold_output2;
			std::vector<std::vector<cv::Point> > contours2;
			std::vector<cv::Vec4i> hierarchy2;

			// Detect edges using Threshold
			threshold(src2, threshold_output2, thresh, 255, cv::THRESH_BINARY);
			// Find contours
			findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

			barMaskAreas[idx] = -1.0;
			for (int i = 0; i < contours2.size(); i++)
			{
				if (barMaskAreas[idx] < contourArea(contours2[i]))
				{
					barMaskAreas[idx] = contourArea(contours2[i]);
				}
			}
		}
		else if (barType == BarArea) {
			//Based on Area
			double mux = 0.005;
			int right = boundRect[b].x + (mux * boundRect[b].width);
			double ht = 1;
			double area = contourArea(contours[b]);
			double cutOff = (t_val[idx] / t_valMax);
		
			barMaskAreas[idx] = -1.0;
			while ((barMaskAreas[idx] / area) < cutOff && right >= 0) {
				drawing_tmp = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				drawing = cv::Mat::zeros(threshold_output.size(), CV_8UC3);
				cv::Point p1(boundRect[b].x, boundRect[b].y);
				cv::Point p2(right, boundRect[b].y + boundRect[b].height);
				labelPos = p1;
				barMaskHeights[idx] = right - boundRect[b].x;
				rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
				drawing.copyTo(drawing_tmp, src2);

				//Based on Area		

				cv::Mat src3 = cv::Mat::zeros(src2.size(), CV_8UC1);
				cvtColor(drawing_tmp, src3, CV_BGR2GRAY);

				cv::Mat threshold_output2;
				std::vector<std::vector<cv::Point> > contours2;
				std::vector<cv::Vec4i> hierarchy2;

				// Detect edges using Threshold
				threshold(src3, threshold_output2, thresh, 255, cv::THRESH_BINARY);
				// Find contours
				findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

				barMaskAreas[idx] = -1.0;
				for (int i = 0; i < contours2.size(); i++)
				{
					if (barMaskAreas[idx] < contourArea(contours2[i]))
					{
						barMaskAreas[idx] = contourArea(contours2[i]);
					}
				}
				mux = mux + 0.005;
				right = boundRect[b].x + (mux * boundRect[b].width);
			}

		}
		else if (barType == BarOptimize) {
			//Based on mask height

			int right = boundRect[b].x + barMaskHeights[idx];

			cv::Point p1(boundRect[b].x, boundRect[b].y);
			cv::Point p2(right, boundRect[b].y + boundRect[b].height);
			labelPos = p1;

			rectangle(drawing, p1, p2, cv::Scalar(barColor.blue(), barColor.green(), barColor.red()), -2, 8, 0);
			drawing.copyTo(drawing_tmp, src2);

			src2 = cv::Mat::zeros(src2.size(), CV_8UC1);
			cvtColor(drawing_tmp, src2, CV_BGR2GRAY);

			cv::Mat threshold_output2;
			std::vector<std::vector<cv::Point> > contours2;
			std::vector<cv::Vec4i> hierarchy2;

			// Detect edges using Threshold
			threshold(src2, threshold_output2, thresh, 255, cv::THRESH_BINARY);
			// Find contours
			findContours(threshold_output2, contours2, hierarchy2, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

			barMaskAreas[idx] = -1.0;
			for (int i = 0; i < contours2.size(); i++)
			{
				if (barMaskAreas[idx] < contourArea(contours2[i]))
				{
					barMaskAreas[idx] = contourArea(contours2[i]);
				}
			}
		}
	}
	//Get Background Image
	cv::Mat image(filtimg.size(), CV_8UC3);
	scene_infomage->getCurrentBackdrop().copyTo(image);

	//Merge the chart with the image in LAB color space
	cvtColor(image, image, CV_BGR2Lab);
	cvtColor(drawing_tmp, drawing_tmp, CV_BGR2Lab);	

	for (int x = 0; x < drawing_tmp.rows; x++)
	{
		for (int y = 0; y < drawing_tmp.cols; y++)
		{
			cv::Vec3b intensity = drawing_tmp.at<cv::Vec3b>(x, y);
			if (intensity.val[0] != 0)
			{
				image.at<cv::Vec3b>(x, y).val[0] = image.at<cv::Vec3b>(x, y).val[0]+10;
				image.at<cv::Vec3b>(x, y).val[1] = intensity.val[1];
				image.at<cv::Vec3b>(x, y).val[2] = intensity.val[2];
			}
		}
	}

	

	cvtColor(image, image, CV_Lab2BGR);
	scene_infomage->setCurrentBackdrop(image);

	QGraphicsTextItem *tmp = new QGraphicsTextItem(QString::fromStdString(t_label[idx]) + ": " + QString("%1").arg(t_val[idx]));
	tmp->setZValue(600);
	int yOff = ui.spinBox_labelOffBar->value();
	tmp->setPos(QPointF(labelPos.x, labelPos.y+yOff));
	tmp->setFlag(QGraphicsItem::ItemIsSelectable, true);
	tmp->setFlag(QGraphicsItem::ItemIsMovable, true);
	scene_infomage->addItem(tmp);	
	return barError();
}

void Infomager::on_pushButton_barMaskColor_clicked()
{
	write_text_to_log_file("BarChartFillColorChng\n");
	barColor = QColorDialog::getColor(barColor, this, "Bar Color");
	QString style = "background: rgb(%1, %2, %3);";
	style = style.arg(barColor.red()).arg(barColor.green()).arg(barColor.blue());
	ui.label_barMaskColor->setStyleSheet(style);
	barSet->setColor(barColor);
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::on_spinBox_labelOffBar_valueChanged()
{
	write_text_to_log_file("BarChartLabelOffset\n");
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::on_doubleSpinBox_yMinBarFill_valueChanged()
{
	write_text_to_log_file("BarChartyMinBarFill\n");
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::on_doubleSpinBox_yMaxBarFill_valueChanged()
{
	write_text_to_log_file("BarChartyMaxBarFill\n");
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::on_pushButton_barLength_clicked()
{
	write_text_to_log_file("BarChartHeightBar\n");
	barType = BarHeight;
	ui.label_barArea->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_barLength->setStyleSheet("background: rgb(240, 240, 240);");
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::on_pushButton_barArea_clicked()
{
	write_text_to_log_file("BarChartAreaBar\n");
	barType = BarArea;
	ui.label_barLength->setStyleSheet("background: rgb(65, 65, 65);");
	ui.label_barArea->setStyleSheet("background: rgb(240, 240, 240);");
	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

void Infomager::generateNeighborHt() {
	std::vector<double> newHts;
	std::vector<double> randNos;

	double maxHt = 0;

	for (int j = 0; j < barMaskHeights.size(); j++)
	{
		if (barMaskHeights[j] > maxHt) {
			maxHt = barMaskHeights[j];
		}
	}

	double newMaxHt = 0;

	for (int i = 0; i < barMaskHeights.size(); i++) {
		double rNo = ((double)rand() / (RAND_MAX)) * (maxHt * 15/100);
		newHts.push_back(barMaskHeights[i] + rNo);
		if (newHts[i] > newMaxHt) {
			newMaxHt = newHts[i];
		}
	}
	
	double rmin = 0;
	double rmax = newMaxHt;
	double tmin = 0;
	double tmax = maxHt;

	for (int i = 0; i < newHts.size(); i++) {
		newHts[i] = tmin + (newHts[i] - rmin) * (tmax - tmin) / (rmax - rmin);
	}	
	
	for (int i = 0; i < barMaskHeights.size(); i++) {
		barMaskHeights[i] = newHts[i];
	}
}

void Infomager::optimizeBar() {
	double old_cost = 0.0;
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			old_cost = this->fillBarChart(i);
		}
	}	
	double T = 1.0;
	double T_min = 0.00001;
	double alpha = 0.9;
	std::vector<double> curr_hts = barMaskHeights;		
	std::vector<double> best_hts = barMaskHeights;
	double best_cost = old_cost;
	int itrCnt = 0;
	double totDiff = 0;
	double avgDiff = 1;
	while (T > T_min  & avgDiff > 0.0001) {
		int i = 1;
		int j = 1;	
		while (i <= 100 & j <= 50) {
			generateNeighborHt();
			double new_cost = 0;
			scene_infomage->removePieLabels();
			for (int i = 0; i < barMasksSet.size(); i++)
			{
				if (barMasksSet[i])
				{
					new_cost = this->fillBarChart(i);
				}
			}
			double ap = acceptance_probability(old_cost, new_cost, T);
			double r = ((double)rand() / (RAND_MAX)) + 0.00001;
			
			if (new_cost < best_cost) {
				best_cost = new_cost;
				best_hts = barMaskHeights;
			}			

			if (ap > r) {

				itrCnt += 1;
				totDiff += abs(new_cost - old_cost);
				avgDiff = totDiff / itrCnt;

				char buffer[100];
				sprintf_s(buffer, "Accept.   Error Diff: %f   AP: %f\n", avgDiff, ap);
				OutputDebugStringA(buffer);				

				curr_hts = barMaskHeights;
				old_cost = new_cost;
				i += 1;
				j = 1;
			}
			else {
				char buffer[100];
				sprintf_s(buffer, "Reject.   Error Diff: %f\n", avgDiff);
				OutputDebugStringA(buffer);

				barMaskHeights = curr_hts;
				j += 1;
			}
		}
		T = T*alpha;
	}

	char buffer2[100];
	sprintf_s(buffer2, "Curr Cost: %f\n", old_cost);
	OutputDebugStringA(buffer2);
	sprintf_s(buffer2, "Best Cost: %f \n", best_cost);
	OutputDebugStringA(buffer2);

	barMaskHeights = best_hts;

	QMessageBox::information(0, "info", QString("Optimize done"));

	scene_infomage->setCurrentBackdrop(filtimg);
	QPixmap pix_viz = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(pix_viz);
	scene_infomage->removePieLabels();
	for (int i = 0; i < barMasksSet.size(); i++)
	{
		if (barMasksSet[i])
		{
			this->fillBarChart(i);
		}
	}
	QPixmap filt_backdrop = QPixmap::fromImage(mat_to_qimage_ref(scene_infomage->getCurrentBackdrop(), QImage::Format_RGB888).rgbSwapped());
	scene_infomage->refreshBackrop(filt_backdrop);
}

//Window Handling
void Infomager::on_pushButton_close_clicked()
{
	write_text_to_log_file("InfomagerClose\n");
	QApplication::quit();
	exit(0);
}

void Infomager::mousePressEvent(QMouseEvent *event) 
{
	write_text_to_log_file("MousePress\n");
	segM->hide();
	m_nMouseClick_X_Coordinate = event->x();
	m_nMouseClick_Y_Coordinate = event->y();	
}

void Infomager::mouseMoveEvent(QMouseEvent *event) 
{
/*	if (m_nMouseClick_Y_Coordinate < 30)
	{
		int moveX = event->globalX() - m_nMouseClick_X_Coordinate;
		int moveY = event->globalY() - m_nMouseClick_Y_Coordinate;
		if (moveX < 0 & moveY < 0)
		{
			move(0, 0);
		}
		else if(moveX < 0)
		{
			move(0, moveY);
		}
		else if(moveY < 0)
		{
			move(moveX, 0);
		}
		else
		{
			move(moveX, moveY);
		}
	}*/
}

//File Logging
void Infomager::write_text_to_log_file(const std::string &text)
{
	std::ofstream log_file("infomager_log_file_DC.txt", std::ios_base::out | std::ios_base::app);
	log_file << text;
}