#pragma once
#include <QMainWindow>
//#include "ui_Cbct_Project.h"
#include <QtWidgets>
#include <QtOpenGL>
#include "WelcomeWidget.h"
#include "TableWidget.h"
#include "RenderWidget.h"
#include "FrontWidget.h"
#include "SideWidget.h"
#include "PDFExport.h"

class Cbct_Project : public QMainWindow
{
    Q_OBJECT

public:
    Cbct_Project(QWidget *parent = nullptr);
    ~Cbct_Project();

private:
    //Ui::CBCTwidget ui;

private:
	void closeEvent(QCloseEvent* event);
	void createActions();
	void createMenus();
	void createToolBars();

private slots:// 槽函数
	void about();

private:
	//QDockWidget* dockwidget = new QDockWidget(NULL);
	QStackedWidget* stackedwidget = new QStackedWidget(NULL);

	WelcomeWidget* welcomewidget = new WelcomeWidget(NULL);
	TableWidget* tablewidget = new TableWidget(NULL);

	QPalette pal;
	
	QMenu* fileMenu;
	QMenu* toolMenu;
	QMenu* viewMenu;
	QMenu* helpMenu;

	QLabel* labelModel;
	QComboBox* comboBox;

	QToolBar* fileToolBar;
	QToolBar* toolToolBar;

	QAction* newAct;
	QAction* openAct;
	QAction* openFolderAct;
	QAction* saveAct;
	QAction* saveAsAct;
	QAction* exitAct;
	QAction* aboutAct;

	QString lastDir;//上一次路径
	char* p;//要传的路径
};
