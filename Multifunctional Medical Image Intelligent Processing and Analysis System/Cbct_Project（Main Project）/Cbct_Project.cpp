/*
 *
 * 程序的主窗口类
 * 调用其他Widget实现功能
 * 不同widget之间的信号与槽通信在该类中调用
 * 作者：吴国正
 *
 */
#include "Cbct_Project.h"
#include "SQLServerConnection.cpp"

Cbct_Project::Cbct_Project(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

	//背景色
	pal.setColor(QPalette::Window, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);

	//dockwidget = new QDockWidget(NULL);
	//dockWidget->setObjectName("功能界面");
	//dockwidget->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetMovable);
	//dockwidget->setTitleBarWidget(new QWidget(NULL));
	//dockwidget->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea);
	//dockwidget->setAllowedAreas(Qt::DockWidgetArea::RightDockWidgetArea);
	//dockWidget->setGeometry(QRect(0, 0, 400, 300));
	//dockWidget->setAcceptDrops(false);

	stackedwidget = new QStackedWidget(NULL);
	//stackedWidget->setObjectName("选择界面");
	//stackedWidget->setGeometry(QRect(0, 0, 300, 200));


	//主界面
	//welcomewidget = new WelcomeWidget(NULL);
	stackedwidget->addWidget(welcomewidget);
	
	//数据管理界面
	stackedwidget->addWidget(tablewidget);

	//默认显示主界面
	stackedwidget->setCurrentIndex(0);
	setCentralWidget(stackedwidget);

	createActions();
	createMenus();
	createToolBars();

	setWindowTitle(tr("牙科正畸医学影像智能处理与分析系统"));
	setMinimumSize(1280, 720);
	//this->resize(QSize(1366, 768));
	//showMaximized();

}

Cbct_Project::~Cbct_Project()
{

}

void Cbct_Project::createActions()//创建打开，保存等功能并绑定槽函数
{
	exitAct = new QAction(QIcon("images/close.png"), tr("退出"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("退出程序"));
	connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	aboutAct = new QAction(tr("关于"), this);
	aboutAct->setStatusTip(tr("显示帮助信息"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	labelModel = new QLabel(this);
	labelModel->setText("界面：");
	QFont font("Segoe UI", 14);
	labelModel->setFont(font);

	comboBox = new QComboBox(this);
	comboBox->addItem(QString("主界面"));
	comboBox->addItem(QString("数据管理"));
	comboBox->setObjectName("切换不同功能");
	comboBox->setCurrentIndex(0);
	comboBox->setAutoFillBackground(true);
	//QFont font("Segoe UI", 14);
	comboBox->setFont(font);
	comboBox->setStyleSheet("QComboBox { border: 1px solid #E0E0E0; background-color: #FFFFFF; padding: 2px; }"
		"QComboBox::drop-down { border: none; }"
		"QComboBox::down-arrow { image: url(images/arrow.png); }"
		"QComboBox QAbstractItemView { border: none; background-color: #FFFFFF; }"
		"QComboBox QAbstractItemView::item { padding: 1px; }");
	connect(comboBox, &QComboBox::currentIndexChanged, stackedwidget, &QStackedWidget::setCurrentIndex);
}
void Cbct_Project::createMenus()//创建上方的菜单
{
	/*
	helpMenu = menuBar()->addMenu(tr("帮助"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(exitAct);
	helpMenu->setAutoFillBackground(true);
	helpMenu->setPalette(pal);
	*/
}
void Cbct_Project::createToolBars()//创建工具栏
{
	toolToolBar = addToolBar(tr("工具栏"));
	toolToolBar->addWidget(labelModel);
	toolToolBar->addWidget(comboBox);
	toolToolBar->setMinimumHeight(40);
}
void Cbct_Project::closeEvent(QCloseEvent* event)
{

}

void Cbct_Project::about()
{
	QMessageBox::about(this, tr("关于"),
		tr("山东大学项目实训\t\t2023.6"
			"\n面向精准正畸的多功能医学影像智能处理与分析系统"));
}