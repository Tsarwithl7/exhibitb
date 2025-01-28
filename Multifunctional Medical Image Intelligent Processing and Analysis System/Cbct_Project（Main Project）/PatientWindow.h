#pragma once
#include <QMainWindow>
#include <QtWidgets>
#include <QtOpenGL>

#include "RenderWidget.h"
#include "FrontWidget.h"
#include "SideWidget.h"
#include "PDFExport.h"

class PatientWindow :
    public QMainWindow
{
	Q_OBJECT

public:
	PatientWindow(QWidget* parent = nullptr, const char* name = nullptr, const char* cbctPath = nullptr, bool cbct = false, const char* quDuanPath = nullptr, bool quDuan = false, const char* touCePath = nullptr, bool touCe = false);
	~PatientWindow();

private:
	QTabWidget* tabWidget = new QTabWidget(NULL);
	RenderWidget* renderwidget;
	FrontWidget* frontwidget;
	SideWidget* sidewidget;
	PDFExport* analyzewidget;

	QPalette pal;

	QLabel* labelModel;
	QComboBox* comboBox;

};

