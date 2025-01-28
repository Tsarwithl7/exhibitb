#include "PatientWindow.h"
/*
 *
 * 程序的病人窗口类
 * 调用其他Widget实现功能
 * 不同widget之间的信号与槽通信在该类中调用
 * 作者：吴国正
 *
 */

PatientWindow::PatientWindow(QWidget* parent, const char* name, const char* cbctPath, bool cbct, const char* quDuanPath, bool quDuan, const char* touCePath, bool touCe)
	: QMainWindow(parent)
{
	//背景色
	pal.setColor(QPalette::Window, Qt::white);
	setAutoFillBackground(true);
	setPalette(pal);
	if (cbct) {
		renderwidget = new RenderWidget(NULL);
		renderwidget->readNii(cbctPath);
		tabWidget->addTab(renderwidget, "CBCT显示");
	}
	if (quDuan) {
		frontwidget = new FrontWidget(NULL);
		frontwidget->open(quDuanPath);
		tabWidget->addTab(frontwidget, "曲断操作");
	}
	if (touCe) {
		sidewidget = new SideWidget(NULL);
		sidewidget->open(touCePath);
		tabWidget->addTab(sidewidget, "头侧操作");
	}
	if (cbct && quDuan && touCe) {
		analyzewidget = new PDFExport(NULL);
		analyzewidget->open(quDuanPath, touCePath);
		tabWidget->addTab(analyzewidget, "数据分析");
	}

	setCentralWidget(tabWidget);

	setWindowTitle(tr(name));
	setWindowModality(Qt::WindowModal);
	setMinimumSize(1366, 768);
	//this->resize(QSize(1920, 1080));
	//showMaximized();
}

PatientWindow::~PatientWindow()
{
	delete tabWidget;
	delete renderwidget;
	delete frontwidget;
	delete sidewidget;
	delete analyzewidget;
}
