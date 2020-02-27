#ifndef INFOMAGER_H
#define INFOMAGER_H

#include "ui_infomager.h"
#include "sceneinfomage.h"
#include "imgFilter.h"
#include "segMask.h"
#include "distortionHelp.h"
#include "titleTextItem.h"
#include "load_img.h"

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>
#include <QFontDialog>
#include <QColorDialog>

#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>

#include <qfiledialog.h>
#include <qfile.h>
#include <qtextstream.h>

#include <QNetworkAccessManager>
#include <QtNetwork>
#include <QUrl>

#include <csvparser.h>
#include <colorBrewer.h>
#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <fstream>
#include <string>

class Infomager : public QMainWindow
{
	Q_OBJECT

public:
	Infomager(QWidget *parent = 0);
	~Infomager();
	
	enum ChartType { None, Pie, Line, Bar };	
	enum PieType { Rad, Area, OverLay, Arc };
	enum DivType { Radial, Horizontal, Vertical };
	enum BarType { BarArea, BarHeight };
	
private:
	Ui::InfomagerClass ui;
	segMask *segM;
	distortionHelp *distH;

	//View Objects
	SceneInfomage* scene_infomage;	
	QtCharts::QChart *chart;
	QGraphicsScene* scene_preview;
	QtCharts::QChart *chartpreview;
	

	//Background Image
	cv::Mat img;	
	cv::Mat mask;
	cv::Mat filtimg;
	imgFilter filt;
	bool loaded_img;	
	bool maskSet;
	int imgH, imgW;
	QStringList imgLinks;
	///Functions
	void imageSearch();

	//Data
	std::string t_title = "NO TITLE";
	std::vector<std::string> t_attr;
	std::vector<std::string> t_label;
	std::vector<double> t_val;
	std::vector<std::vector<int>> t_label_clr;
	bool dataLoaded;
	///Functions
	void linearReg();
	void avgSlope();
	void barError();
	
	//Title
	TitleTextItem * chartTitle;
	QGraphicsRectItem * titleRect;
	QFont textFont;
	
	//Chart
	ChartType chartType;
	double chartX, chartY;
	bool chartAdded = false;
	bool chartPrwAdded = false;

	//Pie Chart
	std::vector<double> angles;
	std::vector<double> orig_rad_angles;
	std::vector<double> orig_area_angles;
	std::vector<double> orig_arc_angles;
	std::vector<double> areas;
	std::vector<double> arcLength;
	cv::Point pieCenter;
	std::vector<cv::Vec4i> pie_hierarchy;
	std::vector<cv::Point> pie_bighull;
	int pieRot;
	PieType pieType;
	DivType divType;
	QList<QColor> pieColors;
	QList<QPushButton* > pieColorButtons;
	bool optimize = false;
	double max_cost;
	///Functions
	void drawPieChart();
	double computePieAngles();
	cv::Mat fillPie();
	void optimizePie();
	double computePieError();
	void generateNeighborAngles();
	double acceptance_probability(double old_cost, double new_cost, double T);
	
	void fillSingleObject();
	double computeHorizontal();
	cv::Mat fillHorizontal();
	double computeVertical();
	cv::Mat fillVertical();


	//Line Chart
	QtCharts::QLineSeries *lineSeries;
	QtCharts::QLineSeries *lineGlowSeries;
	QtCharts::QCategoryAxis *xaxis;
	bool lineSpline;
	int lineThick;
	QColor lineColor;
	int lineGlowThick;
	QColor lineGlowColor;
	int xStep;
	double lineYMin, lineYMax;
	///Functions
	void drawLineChart();
	
	//Bar Chart
	QtCharts::QBarSeries *barSeries;
	QtCharts::QBarSet *barSet;
	QtCharts::QValueAxis *barAxisY;
	int barThick;
	QColor barColor;
	int barGlowThick;
	QColor barGlowColor;
	bool barOverlay;
	bool barPercentage;
	BarType barType;
	int barMaskIdx;
	double barYMin, barYMax;
	double barFillYMin, barFillYMax;
	std::vector<cv::Mat> barMasks;
	std::vector<double> barMaskAreas;
	std::vector<double> barMaskHeights;
	std::vector<bool> barMasksSet;
	///Functions
	void drawBarChart();
	void fillBarChart(int idx);
	void optimizeBar();
	void optimzeAspectRatio(bool isBar);

	//Window Handling
	int m_nMouseClick_X_Coordinate;
	int m_nMouseClick_Y_Coordinate;
	///Functions
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	
public slots:
	void recieveMask(cv::Mat bckdrp);

private slots:
	//User Data and Image Input Controls
	void on_loadButtonImg_clicked();
	void on_loadButtonData_clicked();
	void on_listWidget_itemClicked(QListWidgetItem * item);
	void on_pushButton_save_clicked();

	//Text Controls
	void on_pushButton_fontColor_clicked();
	void on_pushButton_fontFill_clicked();
	void on_pushButton_fontItalics_clicked();
	void on_pushButton_fontBold_clicked();
	void on_fontComboBox_fontFamily_currentFontChanged();
	void on_spinBox_fontSize_valueChanged();
	void on_horizontalSlider_textFillOpac_valueChanged();
	
	//Background Controls
	void on_comboBoxPiFiltImg_2_currentIndexChanged(const QString & text);
	void on_pushButton_clear_clicked();
	
	//Pie Chart Controls
	void on_pushButton_pieFill_clicked();
	void on_pushButton_createMask_clicked();
	void on_pushButton_pieRad_clicked();
	void on_pushButton_pieAr_clicked();
	void on_pushButton_pieArc_clicked();
	void on_pushButton_optimizeChart_clicked();
	void on_horizontalSlider_pieAng_sliderReleased();
	void on_spinBox_pieLabelRad_valueChanged();
	void on_spinBox_pieOriginOffX_valueChanged();
	void on_spinBox_pieOriginOffY_valueChanged();
	void on_comboBoxBrewerNames_currentIndexChanged(const QString & text);
	void on_pushButton_pieColor_1_clicked();
	void on_pushButton_pieColor_2_clicked();
	void on_pushButton_pieColor_3_clicked();
	void on_pushButton_pieColor_4_clicked();
	void on_pushButton_pieColor_5_clicked();
	void on_pushButton_pieColor_6_clicked();
	void on_pushButton_pieColor_7_clicked();
	void on_pushButton_pieColor_8_clicked();
	void on_pushButton_pieColor_9_clicked();
	void on_pushButton_pieColor_10_clicked();
	void on_pushButton_pieColor_11_clicked();
	void on_pushButton_pieColor_12_clicked();
	void on_pushButton_pieColor_13_clicked();
	void on_pushButton_pieColor_14_clicked();
	void on_pushButton_pieColor_15_clicked();


	//Technique Select
	void on_pushButton_FillStyle_clicked();
	void on_pushButton_OverlayStyle_clicked();

	void on_pushButton_fillRad_clicked();
	void on_pushButton_fillVerti_clicked();
	void on_pushButton_fillHori_clicked();	


	//Line Chart Controls
	void on_pushButton_lineOverlay_clicked();
	void on_spinBox_lineChartHeight_valueChanged();
	void on_spinBox_lineChartWidth_valueChanged();
	void on_pushButton_lineColor_clicked();
	void on_horizontalSlider_lineThick_valueChanged();
	void on_pushButton_lineGlowColor_clicked();
	void on_horizontalSlider_lineGlowThick_valueChanged();
	void on_pushButton_lineXColor_clicked();
	void on_horizontalSlider_xTicks_valueChanged();
	void on_pushButton_lineYColor_clicked();
	void on_doubleSpinBox_yMin_valueChanged();
	void on_doubleSpinBox_yMax_valueChanged();
	void on_pushButton_lineLine_clicked();
	void on_pushButton_lineSpline_clicked();

	//Bar Chart Controls
	void on_pushButton_barOverlay_clicked();
	void on_pushButton_barFill_clicked();
	void on_spinBox_barChartHeight_valueChanged();
	void on_spinBox_barChartWidth_valueChanged();
	void on_pushButton_barColor_clicked();
	void on_horizontalSlider_barWidth_valueChanged();
	void on_pushButton_barGlowColor_clicked();
	void on_horizontalSlider_barGlowWidth_valueChanged();
	void on_pushButton_barXColor_clicked();
	void on_pushButton_barYColor_clicked();
	void on_doubleSpinBox_yMinBarOverlay_valueChanged();
	void on_doubleSpinBox_yMaxBarOverlay_valueChanged();
	void on_pushButton_barMaskPrev_clicked();
	void on_pushButton_barMaskNext_clicked();
	void on_pushButton_barMask_clicked();

	void on_pushButton_barMaskColor_clicked();
	void on_spinBox_labelOffBar_valueChanged();
	void on_doubleSpinBox_yMinBarFill_valueChanged();
	void on_doubleSpinBox_yMaxBarFill_valueChanged();
	//void on_pushButton_barValue_clicked();
	//void on_pushButton_barPercentage_clicked();
	void on_pushButton_barArea_clicked();
	void on_pushButton_barLength_clicked();
	
	//Window Control
	void on_pushButton_close_clicked();

	//Distortion Help
	void on_pushButton_distortionHelp_clicked();

	//Logging
	void write_text_to_log_file(const std::string &text);

signals:
	void sendBackDrop(cv::Mat bckdrp);
	void sendDistortedVals(std::vector<std::string> t_label, std::vector<double> t_val, std::vector<double> errors);
};
#endif // Infomager_H