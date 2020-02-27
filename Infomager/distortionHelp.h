#ifndef DISTORTIONHELP_H
#define DISTORTIONHELP_H

#include "ui_distortionHelp.h"
#include <QtWidgets/qdialog.h>
#include <qmessagebox.h>
#include <QFontDialog>
#include <QColorDialog>
#include <QGraphicsScene>
#include <QGraphicsView>


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


class distortionHelp : public QDialog
{
	Q_OBJECT

public:
	distortionHelp(QWidget *parent);
	~distortionHelp();

private:
	Ui::distortionHelp ui;
	QGraphicsScene* scene_preview;
	QtCharts::QChart *chartpreview;
	bool chartPrwAdded;

public slots:
	void onValsLoaded(std::vector<std::string> t_label, std::vector<double> t_val, std::vector<double> errors);

private slots:
	void on_pushButton_close_clicked();

};

#endif // SEGMASK_H
