#include "distortionHelp.h"

distortionHelp::distortionHelp(QWidget *parent) : QDialog(parent)
{
	ui.setupUi(this);
	chartPrwAdded = false;
	scene_preview = new QGraphicsScene(this);
	ui.graphicsView_chartDistortion->setScene(scene_preview);
}

distortionHelp::~distortionHelp()
{
	
}

void distortionHelp::onValsLoaded(std::vector<std::string> t_label, std::vector<double> t_val, std::vector<double> errors)
{
	ui.graphicsView_chartDistortion->setScene(scene_preview);

	if (chartPrwAdded)
	{
		chartPrwAdded = false;
		scene_preview->removeItem(chartpreview);
		chartpreview = 0;
	}

	QtCharts::QPieSeries *series1 = new QtCharts::QPieSeries();
	QtCharts::QPieSeries *series2 = new QtCharts::QPieSeries();

	for (int i = 0; i < t_label.size(); i++)
	{
		series1->append(QString::fromStdString(t_label[i]), errors[i+1]- errors[i]);
	}

	for (int i = 0; i < t_val.size(); i++)
	{	
		series2->append(QString::fromStdString(t_label[i]), t_val[i]);
	}

	series1->setLabelsVisible(true);
	series1->setPieSize(0.8);
	series1->setHoleSize(0.55);

	series2->setLabelsVisible(false);
	series2->setPieSize(0.55);
	series2->setHoleSize(0.3);


	chartpreview = new QtCharts::QChart();
	chartpreview->setPreferredSize(460, 460);
	chartpreview->addSeries(series1);
	chartpreview->addSeries(series2);
	chartpreview->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
	chartpreview->createDefaultAxes();
	chartpreview->setMargins(QMargins(0, 0, 0, 0));
	chartpreview->legend()->hide();
	chartpreview->setBackgroundVisible(false);


	scene_preview->addItem(chartpreview);
	//chartPrwAdded = true;
}

void distortionHelp::on_pushButton_close_clicked()
{
	this->hide();
}