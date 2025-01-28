#include "PDFExport.h"

using namespace std;

const double PI = 3.141592653589793238462643383279502884;

struct Point2D {
	double x;
	double y;
};

struct Point3D {
	double x;
	double y;
	double z;
};

struct Line {
	Point3D point1, point2;
};

struct Vector2D {
	double x;
	double y;
};

struct Vector3D {
	double x;
	double y;
	double z;
};

Vector2D calculateVector(const Point2D& p1, const Point2D& p2) {
	Vector2D vec;
	vec.x = p2.x - p1.x;
	vec.y = p2.y - p1.y;
	return vec;
}

Point3D crossProduct(const Point3D& vector1, const Point3D& vector2) {
	Point3D result;
	result.x = vector1.y * vector2.z - vector1.z * vector2.y;
	result.y = vector1.z * vector2.x - vector1.x * vector2.z;
	result.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return result;
}

Vector3D calculateVector(const Point3D& p1, const Point3D& p2) {
	Vector3D vec;
	vec.x = p2.x - p1.x;
	vec.y = p2.y - p1.y;
	vec.z = p2.z - p1.z;
	return vec;
}

double angle(Vector2D va, Vector2D vb) {
	double dotProduct = va.x * vb.x + va.y * vb.y;
	double magnitudeProduct = std::sqrt(va.x * va.x + va.y * va.y) * std::sqrt(vb.x * vb.x + vb.y * vb.y);
	double cosValue = dotProduct / magnitudeProduct;
	return std::acos(cosValue) * 180.0 / PI;
}

double angle(Vector3D va, Vector3D vb) {
	double dotProduct = va.x * vb.x + va.y * vb.y + va.z * vb.z;
	double magnitudeProduct = std::sqrt(va.x * va.x + va.y * va.y + va.z * va.z) * std::sqrt(vb.x * vb.x + vb.y * vb.y + vb.z * vb.z);
	double cosValue = dotProduct / magnitudeProduct;
	return std::acos(cosValue) * 180.0 / PI;
}

double calculateAngleYZ(const Vector3D& va, const Vector3D& vb) {
	double dotProduct = va.y * vb.y + va.z * vb.z;
	double magVa = std::sqrt(va.y * va.y + va.z * va.z);
	double magVb = std::sqrt(vb.y * vb.y + vb.z * vb.z);

	double cosValue = dotProduct / (magVa * magVb);
	return std::acos(cosValue) * 180.0 / PI;
}

double distance(const Point2D& p1, const Point2D& p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return sqrt(dx * dx + dy * dy);
}

double distance(const Point3D& p1, const Point3D& p2) {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	double dz = p2.z - p1.z;
	return sqrt(dx * dx + dy * dy + dz * dz);
}

double vectorMagnitude(const Point3D& vector) {
	return std::sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}

double distanceToLine(const Point3D& point, const Line& line) {
	Point3D vector1, vector2, crossProductResult;
	vector1.x = point.x - line.point1.x;
	vector1.y = point.y - line.point1.y;
	vector1.z = point.z - line.point1.z;

	vector2.x = line.point2.x - line.point1.x;
	vector2.y = line.point2.y - line.point1.y;
	vector2.z = line.point2.z - line.point1.z;

	crossProductResult = crossProduct(vector1, vector2);

	double distance = vectorMagnitude(crossProductResult) / vectorMagnitude(vector2);
	return distance;
}

double toRadians(double degrees) {
	return degrees * PI / 180.0;
}

//按照9像素=1mm进行单位换算
double calculateDistanceToLine(const Point2D& p, const Point2D& p1, const Point2D& p2) {
	double numerator = fabs((p2.y - p1.y) * p.x - (p2.x - p1.x) * p.y + p2.x * p1.y - p2.y * p1.x);
	double denominator = sqrt((p2.y - p1.y) * (p2.y - p1.y) + (p2.x - p1.x) * (p2.x - p1.x));
	return numerator / denominator / 9;
}

double calculateDistanceToLine(const Point3D& p, const Point3D& p1, const Point3D& p2) {
	double numerator = fabs((p2.y - p1.y) * p.x - (p2.x - p1.x) * p.y + p2.x * p1.y - p2.y * p1.x);
	double denominator = sqrt((p2.y - p1.y) * (p2.y - p1.y) + (p2.x - p1.x) * (p2.x - p1.x));
	return numerator / denominator;
}

double distanceToLineYZ(const Point3D& point, const Line& line) {
	double yDiff = line.point2.y - line.point1.y;
	double zDiff = line.point2.z - line.point1.z;

	double eps = 0;

	if (line.point2.y - line.point1.y == 0) {
		eps = 1e-9;
	}


	// 计算直线的斜率
	double slope = zDiff / (yDiff + eps);

	// 计算直线上对应于点 Y 坐标的 Z 坐标
	double zIntercept = line.point1.z - slope * line.point1.y;

	// 计算点到直线在 YZ 分量上的距离
	double yDiffPoint = point.y - line.point1.y;
	double zDiffPoint = point.z - (slope * point.y + zIntercept);
	double distance = std::sqrt(yDiffPoint * yDiffPoint + zDiffPoint * zDiffPoint);

	//cout << "距离是：" << distance << endl;

	return distance;
}

double calculateSNA(const Point2D& s, const Point2D& n, const Point2D& a) {
	double snLength = distance(s, n);
	double naLength = distance(n, a);
	double saLength = distance(s, a);

	double cosSNA = (snLength * snLength + naLength * naLength - saLength * saLength) * 1.0 / 2 / snLength / naLength;

	/* double sinSNA = snLength / saLength;
	 double angleSNA = asin(sinSNA) / PI * 180.0;*/
	double angleSNA = acos(cosSNA) * 180.0 / PI;
	return angleSNA;
}

double calculateSNB(const Point2D& s, const Point2D& n, const Point2D& b) {
	double snLength = distance(s, n);
	double nbLength = distance(n, b);
	double sbLength = distance(s, b);

	double cosSNB = (snLength * snLength + nbLength * nbLength - sbLength * sbLength) * 1.0 / 2 / snLength / nbLength;

	/* double sinSNA = snLength / saLength;
	 double angleSNA = asin(sinSNA) / PI * 180.0;*/
	double angleSNB = acos(cosSNB) * 180.0 / PI;
	return angleSNB;
}

double calculateANB(const Point2D& a, const Point2D& n, const Point2D& b) {
	double naLength = distance(n, a);
	double nbLength = distance(n, b);
	double cosANB = (naLength * naLength + nbLength * nbLength - 1) / (2 * naLength * nbLength);
	//double angleANB = acos(cosANB) * 180.0 / PI;
	double angleANB = acos(cosANB);
	return angleANB;
}

double calculateFH_NPo(const Point2D& po, const Point2D& oR, const Point2D& N, const Point2D& pog) {
	Vector2D va = calculateVector(po, oR);
	Vector2D vb = calculateVector(N, pog);

	return angle(va, vb);
}

double calculateFH_MP(const Point2D& po, const Point2D& oR, const Point2D& T1, const Point2D& Me) {
	Vector2D va = calculateVector(po, oR);
	Vector2D vb = calculateVector(T1, Me);

	return angle(va, vb);
}

double calculateNA_APo(const Point2D& n, const Point2D& a, const Point2D& pog) {

	Vector2D va = calculateVector(n, a);
	Vector2D vb = calculateVector(a, pog);
	return angle(va, vb);
};

double calculateSGn_FH(const Point2D& s, const Point2D& Gn, const Point2D& po, const Point2D& oR) {
	Vector2D va = calculateVector(po, oR);
	Vector2D vb = calculateVector(s, Gn);

	return angle(va, vb);
}

double calculateSN_MP(const Point2D& s, const Point2D& n, const Point2D& T1, const Point2D& Me) {
	Vector2D va = calculateVector(s, n);
	Vector2D vb = calculateVector(T1, Me);

	return angle(va, vb);
}

double calculateU1_NA(const Point2D& p, const Point2D& q, const Point2D& n, const Point2D& a) {
	Vector2D va = calculateVector(p, q);
	Vector2D vb = calculateVector(n, a);

	return angle(va, vb);
}

double calculateU1_NB(const Point2D& r, const Point2D& s, const Point2D& n, const Point2D& b) {
	Vector2D va = calculateVector(r, s);
	Vector2D vb = calculateVector(n, b);

	return angle(va, vb);
}

double calculateU1_SN(const Point2D& r, const Point2D& s, const Point2D& n, const Point2D& b) {
	Vector2D va = calculateVector(r, s);
	Vector2D vb = calculateVector(n, b);

	return angle(va, vb);
}

double calculatePo_NB(const Point2D& pog, const Point2D& n, const Point2D& b) {
	return calculateDistanceToLine(pog, n, b);
};

struct Tooth {
	std::vector<Point3D> voxels;
	Point3D center;
	double length;
	double width;
};


std::vector<Tooth>findExtremeTeeth1(const Tooth* teeth, int size) {

	std::vector<Tooth>importantteeth;

	//leftUpperIncisorIdx左切牙 leftMolarIdx左磨牙 rightUpperIncisorIdx 右切牙 rightMolarIdx右磨牙

	int leftIncisorIdx = 0;
	int rightIncisorIdx = 0;
	int leftMolarIdx = 0;
	int rightMolarIdx = size - 1;

	//td::cout << "牙齿个数是：" << size << std::endl;

	// 寻找 Y 值最大的两颗牙齿（左上切牙和右上切牙）
	for (int i = 1; i < size; ++i) {
		if (teeth[i].center.y > teeth[leftIncisorIdx].center.y) {
			leftIncisorIdx = i;
		}
		else if (teeth[i].center.y > teeth[rightIncisorIdx].center.y) {
			rightIncisorIdx = i;
		}

	}
	importantteeth.push_back(teeth[leftIncisorIdx]);
	importantteeth.push_back(teeth[rightIncisorIdx]);



	// 寻找 Y 值最小的两颗牙齿（左右磨牙）
	for (int i = 1; i < size; ++i) {
		if (teeth[i].center.y < teeth[leftMolarIdx].center.y) {
			leftMolarIdx = i;
		}
		else if (teeth[i].center.y < teeth[rightMolarIdx].center.y) {
			rightMolarIdx = i;
		}
	}
	importantteeth.push_back(teeth[leftMolarIdx]);
	importantteeth.push_back(teeth[rightMolarIdx]);

	return importantteeth;
}

//计算牙弓宽度
float calculateArchWidth(const Tooth& molar1, const Tooth& molar2) {
	float distance = std::sqrt(std::pow((molar1.center.x - molar2.center.x), 2) + std::pow((molar1.center.y - molar2.center.y), 2));
	return distance;
}

//计算牙弓长度
float calculateArchLength(const Tooth& molar1, const Tooth& incisor1, const Tooth& molar2, const Tooth& incisor2) {
	float distance1 = std::abs(molar1.center.y - incisor1.center.y);
	float distance2 = std::abs(molar2.center.y - incisor2.center.y);
	float distance = (distance1 + distance2) / 2.0;
	return distance;
}

double calculate_middle(std::vector<Tooth>& upperteeth, std::vector<Tooth>& lowerteeth) {
	//std::cout << "已经进入测量方法！" << std::endl;

	double upindex = upperteeth[0].center.x + upperteeth[1].center.x;

	double downindex = lowerteeth[0].center.x + lowerteeth[1].center.x;

	return (upindex - downindex);

}


double evaluateCrowdingLevel(const Tooth* upperTeeth, int arclength, int size_upper) {

	//std::cout << "已进入crowdinglevel方法！" << std::endl;

	double totalLength = 0.0;

	for (int i = 0; i < size_upper; ++i) {
		totalLength += upperTeeth[i].length;

	}

	double crowdingRatio = arclength / totalLength;

	//std::cout << "crowdingRatio是：" << crowdingRatio << std::endl;
	return crowdingRatio;

}


// 计算前牙Bolton指数
double calculateAnteriorBoltonIndex(std::vector<Tooth>& upperteeth, std::vector<Tooth>& lowerteeth) {
	double upperWidthSum = (upperteeth[0].width + upperteeth[1].width) / 2.0;
	double lowerWidthSum = (lowerteeth[0].width + lowerteeth[1].width) / 2.0;
	double anteriorBoltonIndex = upperWidthSum / lowerWidthSum;
	//std::cout << "前牙Bolton指数为：" << anteriorBoltonIndex << std::endl;
	return anteriorBoltonIndex;
}


// 计算全牙Bolton指数
double calculateOverallBoltonIndex(const Tooth* upperTeeth, const Tooth* lowerTeeth, int size_upper, int size_lower) {
	double upperWidthSum = 0.0; // 上颌全牙总宽度
	double lowerWidthSum = 0.0; // 下颌全牙总宽度

	for (int i = 0; i < size_upper; ++i) {
		upperWidthSum += upperTeeth[i].width;
	}

	for (int i = 0; i < size_lower; ++i) {
		lowerWidthSum += lowerTeeth[i].width;
	}


	double overallBoltonIndex = upperWidthSum / lowerWidthSum;

	//std::cout << "全牙Bolton指数为：" << overallBoltonIndex << std::endl;
	return overallBoltonIndex;
}

PDFExport::PDFExport(QWidget* parent)
	: QMainWindow(parent)
{
	setWindowTitle("PDF 导出");
	resize(1366, 768);

	// 创建左侧的 DockWidget
	QDockWidget* dockWidget = new QDockWidget("操作栏", this);
	addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

	// 创建左侧 DockWidget 内的垂直布局
	QVBoxLayout* dockLayout = new QVBoxLayout();
	QWidget* dockWidgetContents = new QWidget(dockWidget);
	dockWidgetContents->setStyleSheet("background-color: white;");
	dockWidgetContents->setLayout(dockLayout);
	dockWidget->setWidget(dockWidgetContents);

	// 创建左侧 DockWidget 内的第一个水平布局和按钮1
	QHBoxLayout* buttonLayout1 = new QHBoxLayout();
	dockLayout->addLayout(buttonLayout1);
	QPushButton* button1 = new QPushButton("导出PDF");
	button1->setFixedSize(130, 50);
	button1->setStyleSheet("QPushButton {"
		"border-radius: 25px;"
		"border: 2px solid #000000;"
		"}");
	buttonLayout1->addWidget(button1, 0, Qt::AlignCenter); // 居中对齐

	// 创建左侧 DockWidget 内的第二个水平布局和按钮2
	dockLayout->addLayout(buttonLayout1);
	QPushButton* button2 = new QPushButton("打印PDF");
	button2->setFixedSize(130, 50);
	button2->setStyleSheet("QPushButton {"
		"border-radius: 25px;"
		"border: 2px solid #000000;"
		"}");
	buttonLayout1->addWidget(button2, 0, Qt::AlignCenter); // 居中对齐

	// 创建左侧 DockWidget 内的第三个水平布局和按钮3
	dockLayout->addLayout(buttonLayout1);
	QPushButton* button3 = new QPushButton("启动分析");
	button3->setFixedSize(130, 50);
	button3->setStyleSheet("QPushButton {"
		"border-radius: 25px;"
		"border: 2px solid #000000;"
		"}");
	buttonLayout1->addWidget(button3, 0, Qt::AlignCenter); // 居中对齐

	// 创建左侧 DockWidget 内的第四个水平布局和按钮4
	dockLayout->addLayout(buttonLayout1);
	QPushButton* button4 = new QPushButton("切换页面");
	button4->setFixedSize(130, 50);
	button4->setStyleSheet("QPushButton {"
		"border-radius: 25px;"
		"border: 2px solid #000000;"
		"}");
	buttonLayout1->addWidget(button4, 0, Qt::AlignCenter); // 居中对齐

	// 创建右侧的 DockWidget
	rightDockWidget = new QDockWidget("数据分析", this);
	addDockWidget(Qt::RightDockWidgetArea, rightDockWidget);

	// 创建右侧 DockWidget 内的 QStackedWidget
	stackedWidget = new QStackedWidget(rightDockWidget);
	rightDockWidget->setWidget(stackedWidget);
	// 创建第一页的布局
	QHBoxLayout* page1Layout = new QHBoxLayout();
	page1Widget = new QWidget(stackedWidget);
	page1Widget->setStyleSheet("background-color: white;");
	page1Widget->setLayout(page1Layout);

	// 创建第一页的垂直布局，用于放置标签和表格
	QVBoxLayout* labelVBoxLayout = new QVBoxLayout();
	page1Layout->addLayout(labelVBoxLayout);

	// 创建右侧 DockWidget 内的图片显示组件
	QLabel* label10 = new QLabel("头侧分析报告");
	QFont font = label10->font();
	font.setPointSize(24);
	font.setBold(true);
	label10->setAlignment(Qt::AlignLeft);
	label10->setFont(font);
	labelVBoxLayout->addWidget(label10);

	QLabel* nameLabel = new QLabel("患者：徐敏峰");
	nameLabel->setVisible(false);
	labelVBoxLayout->addWidget(nameLabel);

	// 创建右侧 DockWidget 内的表格
	tableWidget = new QTableWidget();
	tableWidget->setRowCount(0);
	tableWidget->setColumnCount(4);
	QStringList headers;
	headers << "测量项目" << "参考值" << "测量值" << "标准偏差";
	tableWidget->setHorizontalHeaderLabels(headers);
	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->verticalHeader()->setVisible(false);
	labelVBoxLayout->addWidget(tableWidget);
	tableWidget->setColumnWidth(0, 220);
	tableWidget->setColumnWidth(1, 220);
	tableWidget->setColumnWidth(2, 220);
	tableWidget->setColumnWidth(3, 220);

	// 创建第一页的垂直布局，用于放置图片标签
	QVBoxLayout* imageVBoxLayout = new QVBoxLayout();
	imageLabel1 = new QLabel();
	imageVBoxLayout->addWidget(imageLabel1);
	imageLabel1->setFixedWidth(400);

	// 添加第一页的内容到 stackedWidget
	page1Layout->addLayout(imageVBoxLayout);
	// 添加第一页的内容到 stackedWidget
	stackedWidget->addWidget(page1Widget);

	// 创建第二页的布局
	QHBoxLayout* page2Layout = new QHBoxLayout();
	page2Widget = new QWidget(stackedWidget);
	page2Widget->setStyleSheet("background-color: white;");
	page2Widget->setLayout(page2Layout);

	// 创建第二页的第一列布局，用于放置标签和表格
	QVBoxLayout* column1Layout = new QVBoxLayout();
	QLabel* label11 = new QLabel("曲断分析报告");
	QFont font2 = label11->font();
	font2.setPointSize(24);
	font2.setBold(true);
	label11->setAlignment(Qt::AlignLeft);
	label11->setFont(font2);
	column1Layout->addWidget(label11);
	column1Layout->addWidget(nameLabel);

	// 创建右侧 DockWidget 内的表格
	tableWidget2 = new QTableWidget();
	tableWidget2->setRowCount(0);
	tableWidget2->setColumnCount(2);
	QStringList headers2;
	headers2 << "测量项目" << "测量值";
	tableWidget2->setHorizontalHeaderLabels(headers2);
	tableWidget2->horizontalHeader()->setStretchLastSection(true);
	tableWidget2->verticalHeader()->setVisible(false);
	column1Layout->addWidget(tableWidget2);
	tableWidget2->setColumnWidth(0, 220);
	tableWidget2->setColumnWidth(1, 220);

	// 创建第二页的第二列布局，用于放置图片标签
	QVBoxLayout* column2Layout = new QVBoxLayout();
	imageLabel2 = new QLabel();
	column2Layout->addWidget(imageLabel2);
	imageLabel2->setFixedWidth(800);

	// 添加第二页的布局到页面布局
	page2Layout->addLayout(column1Layout);
	page2Layout->addLayout(column2Layout);

	// 添加第二页的内容到 stackedWidget
	stackedWidget->addWidget(page2Widget);

	// 创建主窗口内垂直布局
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QWidget* mainWidget = new QWidget(this);
	mainWidget->setStyleSheet("background-color: white;");
	mainWidget->setLayout(mainLayout);

	setCentralWidget(mainWidget);

	// 设置右侧 DockWidget 大小比例
	mainLayout->addWidget(dockWidget);
	mainLayout->addWidget(rightDockWidget);
	mainLayout->setStretchFactor(dockWidget, 1);
	mainLayout->setStretchFactor(rightDockWidget, 4);

	// 连接按钮的点击事件
	connect(button1, &QPushButton::clicked, this, &PDFExport::onExportClicked);
	connect(button2, &QPushButton::clicked, this, &PDFExport::onPrintClicked);
	connect(button3, &QPushButton::clicked, this, &PDFExport::onOpenImage1Clicked);
	connect(button4, &QPushButton::clicked, this, &PDFExport::onPageSwitchClicked);
}

PDFExport::~PDFExport()
{
}

void PDFExport::open(const char* quDuanPath, const char* touCePath)
{
	quDuan = QString::fromUtf8(quDuanPath);
	touCe = QString::fromUtf8(touCePath);

	QImage image(touCe);
	QPixmap pixmap = QPixmap::fromImage(image);
	QPixmap scaledPixmap = pixmap.scaledToWidth(400);
	imageLabel1->setPixmap(scaledPixmap);

	QImage image2(quDuan);
	QPixmap pixmap2 = QPixmap::fromImage(image2);
	QPixmap scaledPixmap2 = pixmap2.scaledToWidth(850);
	imageLabel2->setPixmap(scaledPixmap2);
}

void PDFExport::exportToPDF(QDockWidget* dockWidget, const QString& filePath)
{
	QPrinter printer(QPrinter::PrinterResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setPageSize(QPageSize(QSizeF(1370, 610), QPageSize::Millimeter));
	printer.setOutputFileName(filePath);

	QPainter painter;
	if (!painter.begin(&printer)) {
		// 处理绘图错误
		return;
	}
	painter.setRenderHint(QPainter::Antialiasing, true);

	// 缩放绘制内容
	painter.scale(3, 3);

	// 绘制第一页
	painter.save();
	painter.translate(page1Widget->geometry().topLeft());
	page1Widget->render(&painter);
	painter.restore();

	// 绘制第二页
	printer.newPage();
	painter.save();
	painter.translate(page2Widget->geometry().topLeft());
	page2Widget->render(&painter);
	painter.restore();

	painter.end();
}

//printer.setPageSize(QPageSize(QSizeF(1370, 610), QPageSize::Millimeter));
void PDFExport::previewPDF(QWidget* widget)
{
	QPrinter printer;
	QPrintPreviewDialog preview(&printer, this);
	connect(&preview, &QPrintPreviewDialog::paintRequested, [widget, &printer](QPrinter* printer) {
		widget->render(printer);
		});
	preview.exec();
}

void PDFExport::onExportClicked()
{
	QString filePath = QFileDialog::getSaveFileName(this, "导出 PDF", "", "PDF 文件 (*.pdf)");
	if (!filePath.isEmpty())
	{
		exportToPDF(rightDockWidget, filePath);
		QMessageBox::information(this, "导出成功", "PDF 文件已成功导出！");
	}
}

void PDFExport::onPrintClicked()
{
	QPrinter printer;
	QPrintDialog dialog(&printer, this);
	if (dialog.exec() == QDialog::Accepted)
	{
		exportToPDF(rightDockWidget, printer.outputFileName());
		QMessageBox::information(this, "打印成功", "PDF 文件已成功打印！");
	}
}

//与服务器的交互暂时被注释掉了
void PDFExport::onOpenImage1Clicked()
{

	if (stackedWidget->currentIndex() == 0) {
		QString imagePath = touCe;
		if (!imagePath.isEmpty())
		{
			QImage image(imagePath);
			if (!image.isNull()){
				pixmap = QPixmap::fromImage(image);
				QPixmap scaledPixmap = pixmap.scaledToWidth(400);
				imageLabel1->setPixmap(scaledPixmap);
				//result = QString::fromStdString(Message::runPythonFunction(5, 0, imagePath.toUtf8().constData(), ""));
				QString result = "{\"point\": [649, 794, 1296, 698, 1173, 1003, 427, 989, 1302, 1385, 1290, 1767, 1287, 1911, 1245, 1992, 1282, 1968, 575, 1682, 1339, 1561, 1356, 1577, 1473, 1502, 1470, 1663, 1455, 1363, 1413, 1927, 826, 1273, 1333, 1310, 470, 1144, 208, 1232, 430, 1198, 533, 984, 476, 1048, 510, 1575, 564, 1714, 652, 1500, 1330, 1644, 1279, 1641, 1242, 1885, 803, 970, 829, 1246, 1353, 1492, 1302, 1532, 1250, 1393, 1361, 1543, 1042, 1417, 1034, 1532, 1071, 1510, 968, 1478, 1250, 1754, 1011, 1636, 1068, 1556, 1088, 1580, 988, 1551, 1461, 869, 1276, 2077, 1367, 2040, 1401, 1735, 1461, 1591, 1396, 1583, 1393, 1564, 1455, 1524, 1464, 1446, 1584, 1321, 1595, 1256, 1512, 1021, 1336, 805, 1407, 615, 627, 1117, 641, 1965, 459, 1914, 510, 1559, 601, 1604, 664, 1347, 567, 1347, 592, 1270]}";
				QTimer* timer = new QTimer(this);
				timer->setInterval(10); // 设置间隔为 1 秒（根据需要进行调整）
				connect(timer, &QTimer::timeout, this, [=]() {
					if (!result.isEmpty()) {
						QString jsonString = result.mid(result.indexOf("{")); // 提取JSON字符串部分

						QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
						QJsonObject jsonObject = jsonDocument.object();
						QJsonValue jsonValue = jsonObject.value("point");
						QJsonArray jsonArray = jsonValue.toArray();
						Point2D s, n, a, b, po, oR, pog, T1, Me, Gn, p, q, r, s1;
						s.x = jsonArray[0].toDouble();
						s.y = jsonArray[1].toDouble();
						n.x = jsonArray[2].toDouble();
						n.y = jsonArray[3].toDouble();
						oR.x = jsonArray[4].toDouble();
						oR.y = jsonArray[5].toDouble();
						po.x = jsonArray[6].toDouble();
						po.y = jsonArray[7].toDouble();
						a.x = jsonArray[8].toDouble();
						a.y = jsonArray[9].toDouble();
						b.x = jsonArray[10].toDouble();
						b.y = jsonArray[11].toDouble();
						pog.x = jsonArray[12].toDouble();
						pog.y = jsonArray[13].toDouble();
						Me.x = jsonArray[14].toDouble();
						Me.y = jsonArray[15].toDouble();
						Gn.x = jsonArray[16].toDouble();
						Gn.y = jsonArray[17].toDouble();
						r.x = jsonArray[20].toDouble();
						r.y = jsonArray[21].toDouble();
						q.x = jsonArray[22].toDouble();
						q.y = jsonArray[23].toDouble();
						T1.x = jsonArray[48].toDouble();
						T1.y = jsonArray[49].toDouble();
						p.x = jsonArray[66].toDouble();
						p.y = jsonArray[67].toDouble();
						s1.x = jsonArray[78].toDouble();
						s1.y = jsonArray[79].toDouble();

						double sna = round(calculateSNA(s, n, a) * 10) / 10;
						double snb = round(calculateSNB(s, n, b) * 10) / 10;
						double FH_NPo = round(calculateFH_NPo(po, oR, n, pog) * 10) / 10;
						double NA_APo = round(calculateNA_APo(n, a, pog) * 10) / 10;
						double FH_MP = round(calculateFH_MP(po, oR, T1, Me) * 10) / 10;
						double SGn_FH = round(calculateSGn_FH(s, Gn, po, oR) * 10) / 10;
						double SN_MP = round(calculateSN_MP(s, n, T1, Me) * 10) / 10;
						double Po_NB = round(calculateDistanceToLine(pog, n, b) * 10) / 10;
						double U1_NA = round(calculateU1_NA(p, q, n, a) * 10) / 10;
						double U1_NAmm = round(calculateDistanceToLine(q, n, a) * 10) / 10;
						double L1_NB = round(calculateU1_NB(r, s1, n, b) * 10) / 10;
						double L1_NBmm = round(calculateDistanceToLine(r, n, b) * 10) / 10;
						double U1_SN = round(calculateU1_SN(s1, n, p, q) * 10) / 10;

						tableWidget->setRowCount(13);

						tableWidget->setItem(0, 0, new QTableWidgetItem("SNA"));
						tableWidget->setItem(0, 1, new QTableWidgetItem("82.8°(±4)"));
						tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(sna) + "°"));
						tableWidget->setItem(0, 3, new QTableWidgetItem(QString::number(sna - 82.8)));

						tableWidget->setItem(1, 0, new QTableWidgetItem("SNB"));
						tableWidget->setItem(1, 1, new QTableWidgetItem("80.1°(±3.9)"));
						tableWidget->setItem(1, 2, new QTableWidgetItem(QString::number(snb) + "°"));
						tableWidget->setItem(1, 3, new QTableWidgetItem(QString::number(snb - 80.1)));

						tableWidget->setItem(2, 0, new QTableWidgetItem("FH_NPo"));
						tableWidget->setItem(2, 1, new QTableWidgetItem("85.4°(±3.7)"));
						tableWidget->setItem(2, 2, new QTableWidgetItem(QString::number(FH_NPo) + "°"));
						tableWidget->setItem(2, 3, new QTableWidgetItem(QString::number(FH_NPo - 85.4)));

						tableWidget->setItem(3, 0, new QTableWidgetItem("NA_APo"));
						tableWidget->setItem(3, 1, new QTableWidgetItem("6°(±4.4)"));
						tableWidget->setItem(3, 2, new QTableWidgetItem(QString::number(NA_APo) + "°"));
						tableWidget->setItem(3, 3, new QTableWidgetItem(QString::number(NA_APo - 6)));

						tableWidget->setItem(4, 0, new QTableWidgetItem("FH_MP"));
						tableWidget->setItem(4, 1, new QTableWidgetItem("31.1°(±5.6)"));
						tableWidget->setItem(4, 2, new QTableWidgetItem(QString::number(FH_MP) + "°"));
						tableWidget->setItem(4, 3, new QTableWidgetItem(QString::number(FH_MP - 31.1)));

						tableWidget->setItem(5, 0, new QTableWidgetItem("SGn_FH"));
						tableWidget->setItem(5, 1, new QTableWidgetItem("66.3°(±7.1)"));
						tableWidget->setItem(5, 2, new QTableWidgetItem(QString::number(SGn_FH) + "°"));
						tableWidget->setItem(5, 3, new QTableWidgetItem(QString::number(SGn_FH - 66.3)));

						tableWidget->setItem(6, 0, new QTableWidgetItem("SN_MP"));
						tableWidget->setItem(6, 1, new QTableWidgetItem("32.5°(±5.2)"));
						tableWidget->setItem(6, 2, new QTableWidgetItem(QString::number(SN_MP) + "°"));
						tableWidget->setItem(6, 3, new QTableWidgetItem(QString::number(SN_MP - 32.5)));

						tableWidget->setItem(7, 0, new QTableWidgetItem("Po_NB"));
						tableWidget->setItem(7, 1, new QTableWidgetItem("1mm (±1.5)"));
						tableWidget->setItem(7, 2, new QTableWidgetItem(QString::number(Po_NB) + "mm"));
						tableWidget->setItem(7, 3, new QTableWidgetItem(QString::number(Po_NB - 1)));

						tableWidget->setItem(8, 0, new QTableWidgetItem("U1_NA"));
						tableWidget->setItem(8, 1, new QTableWidgetItem("22.8°(±5.7)"));
						tableWidget->setItem(8, 2, new QTableWidgetItem(QString::number(U1_NA) + "°"));
						tableWidget->setItem(8, 3, new QTableWidgetItem(QString::number(U1_NA - 22.8)));

						tableWidget->setItem(9, 0, new QTableWidgetItem("dU1_NA"));
						tableWidget->setItem(9, 1, new QTableWidgetItem("5.1mm(±2.4)"));
						tableWidget->setItem(9, 2, new QTableWidgetItem(QString::number(U1_NAmm) + "mm"));
						tableWidget->setItem(9, 3, new QTableWidgetItem(QString::number(U1_NAmm - 5.1)));

						tableWidget->setItem(10, 0, new QTableWidgetItem("L1_NB"));
						tableWidget->setItem(10, 1, new QTableWidgetItem("30.3°(±5.8)"));
						tableWidget->setItem(10, 2, new QTableWidgetItem(QString::number(L1_NB) + "°"));
						tableWidget->setItem(10, 3, new QTableWidgetItem(QString::number(L1_NB - 30.3)));

						tableWidget->setItem(11, 0, new QTableWidgetItem("dL1_NB"));
						tableWidget->setItem(11, 1, new QTableWidgetItem("6.7mm(±2.1)"));
						tableWidget->setItem(11, 2, new QTableWidgetItem(QString::number(L1_NBmm) + "mm"));
						tableWidget->setItem(11, 3, new QTableWidgetItem(QString::number(L1_NBmm - 6.7)));

						tableWidget->setItem(12, 0, new QTableWidgetItem("U1_SN"));
						tableWidget->setItem(12, 1, new QTableWidgetItem("105.7°(±6.3)"));
						tableWidget->setItem(12, 2, new QTableWidgetItem(QString::number(U1_SN) + "°"));
						tableWidget->setItem(12, 3, new QTableWidgetItem(QString::number(U1_SN - 105.7)));

					}

					timer->stop();
					delete timer;
					});

				timer->start();
			}
		}
	}
	else if (stackedWidget->currentIndex() == 1) {
		QString imagePath = quDuan;
		if (!imagePath.isEmpty())
		{
			QImage image(imagePath);
			if (!image.isNull())
			{
				pixmap = QPixmap::fromImage(image);
				QPixmap scaledPixmap = pixmap.scaledToWidth(850);
				imageLabel2->setPixmap(scaledPixmap);

				const char* filename = "D:/Data/teeth.nii.gz";

				// 创建 NIFTI 图片读取器
				vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
				reader->SetFileName(filename);
				reader->Update();

				std::vector<Point3D> tooth_voxels;
				// 获取图像数据
				vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();

				// 获取图像尺寸
				int dimensions[3];
				imageData->GetDimensions(dimensions);
				int nx = dimensions[0];  // 宽度
				int ny = dimensions[1];  // 高度
				int nz = dimensions[2];  // 深度

				double range[2];
				imageData->GetScalarRange(range);

				int maxRange = range[1];

				Tooth* upperTeeth;
				Tooth* lowerTeeth;
				Tooth* teeth = new Tooth[maxRange];

				int upper = 0, lower = 0;
				double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;
				double length = 0.0, width = 0.0;

				Tooth tooth;

				// 访问体素值
				float* data = static_cast<float*>(imageData->GetScalarPointer());
				int i = 0;
				for (int i = 1; i <= maxRange; i++) {
					tooth_voxels.clear();

					// 遍历所有体素
					for (int z = 0; z < nz; ++z) {
						for (int y = 0; y < ny; ++y) {
							for (int x = 0; x < nx; ++x) {
								// 计算当前体素的索引
								int index = x + y * nx + z * nx * ny;

								// 获取体素值
								float voxel_value = data[index];
								if (voxel_value == i) {
									tooth = Tooth();
									double voxel_coordinates[3];
									imageData->GetOrigin(voxel_coordinates);
									double voxel_spacing[3];
									imageData->GetSpacing(voxel_spacing);
									double real_x = voxel_coordinates[0] + x * voxel_spacing[0];
									double real_y = voxel_coordinates[1] + y * voxel_spacing[1];
									double real_z = voxel_coordinates[2] + z * voxel_spacing[2];
									tooth_voxels.push_back({ real_x, real_y, real_z });
								}

							}
						}
					}

					length = 0;
					width = 0;
					sum_x = 0;
					sum_y = 0;
					sum_z = 0;

					for (const auto& voxel : tooth_voxels) {
						sum_x += voxel.x;
						sum_y += voxel.y;
						sum_z += voxel.z;
					}
					Point3D center;
					center.x = sum_x / tooth_voxels.size();
					center.y = sum_y / tooth_voxels.size();
					center.z = sum_z / tooth_voxels.size();

					tooth.center.x = center.x;
					tooth.center.y = center.y;
					tooth.center.z = center.z;

					// 计算长宽数据
					double min_x = tooth_voxels[0].x;
					double max_x = tooth_voxels[0].x;
					double min_y = tooth_voxels[0].y;
					double max_y = tooth_voxels[0].y;
					double min_z = tooth_voxels[0].z;
					double max_z = tooth_voxels[0].z;

					for (const auto& voxel : tooth_voxels) {
						min_x = std::min(min_x, voxel.x);
						max_x = std::max(max_x, voxel.x);
						min_y = std::min(min_y, voxel.y);
						max_y = std::max(max_y, voxel.y);
						min_z = std::min(min_z, voxel.z);
						max_z = std::max(max_z, voxel.z);
					}

					length = max_z - min_z;
					width = std::max(max_x - min_x, max_y - min_y);

					tooth.length = length;
					tooth.width = width;

					if (center.z <= 70) {
						lower++;
					}
					else { upper++; }

					teeth[i - 1] = tooth;
				}

				upperTeeth = new Tooth[upper];
				lowerTeeth = new Tooth[lower];

				int lowerIndex = 0, upperIndex = 0;

				for (int i = 0; i < maxRange; i++) {
					tooth = teeth[i];
					if (tooth.center.z < 70) {
						lowerTeeth[lowerIndex] = tooth;
						lowerIndex++;
					}
					else {
						upperTeeth[upperIndex] = tooth;
						upperIndex++;
					}
				}

				tableWidget2->setRowCount(5 + maxRange);

				tableWidget2->setItem(0, 0, new QTableWidgetItem("牙弓长度"));
				std::vector<Tooth> importantteethup = findExtremeTeeth1(upperTeeth, upper);
				std::vector<Tooth> importantteethdown = findExtremeTeeth1(lowerTeeth, lower);
				length = calculateArchLength(importantteethup[0], importantteethup[2], importantteethup[3], importantteethup[1]);
				tableWidget2->setItem(0, 1, new QTableWidgetItem(QString::number(length, 'f', 1) + "mm"));
				tableWidget2->setItem(1, 0, new QTableWidgetItem("牙弓宽度"));
				width = calculateArchWidth(importantteethup[2], importantteethup[3]);
				tableWidget2->setItem(1, 1, new QTableWidgetItem(QString::number(width, 'f', 1) + "mm"));
				tableWidget2->setItem(2, 0, new QTableWidgetItem("拥挤度"));
				double crowding = evaluateCrowdingLevel(upperTeeth, length, upper);
				if (crowding < 0.02) {
					//std::cout << "牙齿排列稀疏" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列稀疏"));
				}
				else if (crowding < 0.04) {
					//std::cout << "牙齿排列中等" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列中等"));
				}
				else {
					//std::cout << "牙齿排列拥挤" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列拥挤"));
				}
				tableWidget2->setItem(3, 0, new QTableWidgetItem("Bolton指数"));
				double bolton = calculateOverallBoltonIndex(upperTeeth, lowerTeeth, upper, lower);
				tableWidget2->setItem(3, 1, new QTableWidgetItem(QString::number(bolton * 100, 'f', 1) + "%"));
				tableWidget2->setItem(4, 0, new QTableWidgetItem("中线关系"));
				double middle = calculate_middle(importantteethup, importantteethdown);
				if (std::abs(middle) >= 2)
				{
					//std::cout << "中线偏移量过大！" << std::endl;
					tableWidget2->setItem(4, 1, new QTableWidgetItem("中线偏移量过大"));
				}
				else
				{
					//std::cout << "中线偏移量正常！" << std::endl;
					tableWidget2->setItem(4, 1, new QTableWidgetItem("中线偏移量正常"));
				}
				int index = 5;
				for (int i = 0; i < upper; i++) {
					tableWidget2->setItem(index, 0, new QTableWidgetItem("上排第" + QString::number(i + 1) + "颗牙齿的长宽"));
					tableWidget2->setItem(index, 1, new QTableWidgetItem(QString::number(upperTeeth[i].length) + "mm, " + QString::number(upperTeeth[i].width) + "mm"));
					index++;
				}
				for (int i = 0; i <= lower; i++) {
					tableWidget2->setItem(index, 0, new QTableWidgetItem("下排第" + QString::number(i + 1) + "颗牙齿的长宽"));
					tableWidget2->setItem(index, 1, new QTableWidgetItem(QString::number(lowerTeeth[i].length) + "mm, " + QString::number(lowerTeeth[i].width) + "mm"));
					index++;
				}



			}
		}
	}
	/*
	QString imagePath = QFileDialog::getOpenFileName(this, "打开图片", "", "图片文件 (*.png *.jpg *.jpeg)");
	if (!imagePath.isEmpty())
	{
		QImage image(imagePath);
		if (!image.isNull())
		{
			QPixmap pixmap = QPixmap::fromImage(image);
			// 根据当前页码获取对应的 imageLabel
			if (stackedWidget->currentIndex() == 0)
			{
				QPixmap scaledPixmap = pixmap.scaledToWidth(400);
				imageLabel1->setPixmap(scaledPixmap);
				//result = QString::fromStdString(Message::runPythonFunction(5, 0, imagePath.toUtf8().constData(), ""));
				QString result = "{\"point\": [649, 794, 1296, 698, 1173, 1003, 427, 989, 1302, 1385, 1290, 1767, 1287, 1911, 1245, 1992, 1282, 1968, 575, 1682, 1339, 1561, 1356, 1577, 1473, 1502, 1470, 1663, 1455, 1363, 1413, 1927, 826, 1273, 1333, 1310, 470, 1144, 208, 1232, 430, 1198, 533, 984, 476, 1048, 510, 1575, 564, 1714, 652, 1500, 1330, 1644, 1279, 1641, 1242, 1885, 803, 970, 829, 1246, 1353, 1492, 1302, 1532, 1250, 1393, 1361, 1543, 1042, 1417, 1034, 1532, 1071, 1510, 968, 1478, 1250, 1754, 1011, 1636, 1068, 1556, 1088, 1580, 988, 1551, 1461, 869, 1276, 2077, 1367, 2040, 1401, 1735, 1461, 1591, 1396, 1583, 1393, 1564, 1455, 1524, 1464, 1446, 1584, 1321, 1595, 1256, 1512, 1021, 1336, 805, 1407, 615, 627, 1117, 641, 1965, 459, 1914, 510, 1559, 601, 1604, 664, 1347, 567, 1347, 592, 1270]}";
				QTimer* timer = new QTimer(this);
				timer->setInterval(10); // 设置间隔为 1 秒（根据需要进行调整）
				connect(timer, &QTimer::timeout, this, [=]() {
					if (!result.isEmpty()) {
						QString jsonString = result.mid(result.indexOf("{")); // 提取JSON字符串部分

						QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
						QJsonObject jsonObject = jsonDocument.object();
						QJsonValue jsonValue = jsonObject.value("point");
						QJsonArray jsonArray = jsonValue.toArray();
						Point2D s, n, a, b, po, oR, pog, T1, Me, Gn, p, q, r, s1;
						s.x = jsonArray[0].toDouble();
						s.y = jsonArray[1].toDouble();
						n.x = jsonArray[2].toDouble();
						n.y = jsonArray[3].toDouble();
						oR.x = jsonArray[4].toDouble();
						oR.y = jsonArray[5].toDouble();
						po.x = jsonArray[6].toDouble();
						po.y = jsonArray[7].toDouble();
						a.x = jsonArray[8].toDouble();
						a.y = jsonArray[9].toDouble();
						b.x = jsonArray[10].toDouble();
						b.y = jsonArray[11].toDouble();
						pog.x = jsonArray[12].toDouble();
						pog.y = jsonArray[13].toDouble();
						Me.x = jsonArray[14].toDouble();
						Me.y = jsonArray[15].toDouble();
						Gn.x = jsonArray[16].toDouble();
						Gn.y = jsonArray[17].toDouble();
						r.x = jsonArray[20].toDouble();
						r.y = jsonArray[21].toDouble();
						q.x = jsonArray[22].toDouble();
						q.y = jsonArray[23].toDouble();
						T1.x = jsonArray[48].toDouble();
						T1.y = jsonArray[49].toDouble();
						p.x = jsonArray[66].toDouble();
						p.y = jsonArray[67].toDouble();
						s1.x = jsonArray[78].toDouble();
						s1.y = jsonArray[79].toDouble();

						double sna = round(calculateSNA(s, n, a) * 10) / 10;
						double snb = round(calculateSNB(s, n, b) * 10) / 10;
						double FH_NPo = round(calculateFH_NPo(po, oR, n, pog) * 10) / 10;
						double NA_APo = round(calculateNA_APo(n, a, pog) * 10) / 10;
						double FH_MP = round(calculateFH_MP(po, oR, T1, Me) * 10) / 10;
						double SGn_FH = round(calculateSGn_FH(s, Gn, po, oR) * 10) / 10;
						double SN_MP = round(calculateSN_MP(s, n, T1, Me) * 10) / 10;
						double Po_NB = round(calculateDistanceToLine(pog, n, b) * 10) / 10;
						double U1_NA = round(calculateU1_NA(p, q, n, a) * 10) / 10;
						double U1_NAmm = round(calculateDistanceToLine(q, n, a) * 10) / 10;
						double L1_NB = round(calculateU1_NB(r, s1, n, b) * 10) / 10;
						double L1_NBmm = round(calculateDistanceToLine(r, n, b) * 10) / 10;
						double U1_SN = round(calculateU1_SN(s1, n, p, q) * 10) / 10;

						tableWidget->setRowCount(13);

						tableWidget->setItem(0, 0, new QTableWidgetItem("SNA"));
						tableWidget->setItem(0, 1, new QTableWidgetItem("82.8°(±4)"));
						tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(sna) + "°"));
						tableWidget->setItem(0, 3, new QTableWidgetItem(QString::number(sna - 82.8)));

						tableWidget->setItem(1, 0, new QTableWidgetItem("SNB"));
						tableWidget->setItem(1, 1, new QTableWidgetItem("80.1°(±3.9)"));
						tableWidget->setItem(1, 2, new QTableWidgetItem(QString::number(snb) + "°"));
						tableWidget->setItem(1, 3, new QTableWidgetItem(QString::number(snb - 80.1)));

						tableWidget->setItem(2, 0, new QTableWidgetItem("FH_NPo"));
						tableWidget->setItem(2, 1, new QTableWidgetItem("85.4°(±3.7)"));
						tableWidget->setItem(2, 2, new QTableWidgetItem(QString::number(FH_NPo) + "°"));
						tableWidget->setItem(2, 3, new QTableWidgetItem(QString::number(FH_NPo - 85.4)));

						tableWidget->setItem(3, 0, new QTableWidgetItem("NA_APo"));
						tableWidget->setItem(3, 1, new QTableWidgetItem("6°(±4.4)"));
						tableWidget->setItem(3, 2, new QTableWidgetItem(QString::number(NA_APo) + "°"));
						tableWidget->setItem(3, 3, new QTableWidgetItem(QString::number(NA_APo - 6)));

						tableWidget->setItem(4, 0, new QTableWidgetItem("FH_MP"));
						tableWidget->setItem(4, 1, new QTableWidgetItem("31.1°(±5.6)"));
						tableWidget->setItem(4, 2, new QTableWidgetItem(QString::number(FH_MP) + "°"));
						tableWidget->setItem(4, 3, new QTableWidgetItem(QString::number(FH_MP - 31.1)));

						tableWidget->setItem(5, 0, new QTableWidgetItem("SGn_FH"));
						tableWidget->setItem(5, 1, new QTableWidgetItem("66.3°(±7.1)"));
						tableWidget->setItem(5, 2, new QTableWidgetItem(QString::number(SGn_FH) + "°"));
						tableWidget->setItem(5, 3, new QTableWidgetItem(QString::number(SGn_FH - 66.3)));

						tableWidget->setItem(6, 0, new QTableWidgetItem("SN_MP"));
						tableWidget->setItem(6, 1, new QTableWidgetItem("32.5°(±5.2)"));
						tableWidget->setItem(6, 2, new QTableWidgetItem(QString::number(SN_MP) + "°"));
						tableWidget->setItem(6, 3, new QTableWidgetItem(QString::number(SN_MP - 32.5)));

						tableWidget->setItem(7, 0, new QTableWidgetItem("Po_NB"));
						tableWidget->setItem(7, 1, new QTableWidgetItem("1mm (±1.5)"));
						tableWidget->setItem(7, 2, new QTableWidgetItem(QString::number(Po_NB) + "mm"));
						tableWidget->setItem(7, 3, new QTableWidgetItem(QString::number(Po_NB - 1)));

						tableWidget->setItem(8, 0, new QTableWidgetItem("U1_NA"));
						tableWidget->setItem(8, 1, new QTableWidgetItem("22.8°(±5.7)"));
						tableWidget->setItem(8, 2, new QTableWidgetItem(QString::number(U1_NA) + "°"));
						tableWidget->setItem(8, 3, new QTableWidgetItem(QString::number(U1_NA - 22.8)));

						tableWidget->setItem(9, 0, new QTableWidgetItem("dU1_NA"));
						tableWidget->setItem(9, 1, new QTableWidgetItem("5.1mm(±2.4)"));
						tableWidget->setItem(9, 2, new QTableWidgetItem(QString::number(U1_NAmm) + "mm"));
						tableWidget->setItem(9, 3, new QTableWidgetItem(QString::number(U1_NAmm - 5.1)));

						tableWidget->setItem(10, 0, new QTableWidgetItem("L1_NB"));
						tableWidget->setItem(10, 1, new QTableWidgetItem("30.3°(±5.8)"));
						tableWidget->setItem(10, 2, new QTableWidgetItem(QString::number(L1_NB) + "°"));
						tableWidget->setItem(10, 3, new QTableWidgetItem(QString::number(L1_NB - 30.3)));

						tableWidget->setItem(11, 0, new QTableWidgetItem("dL1_NB"));
						tableWidget->setItem(11, 1, new QTableWidgetItem("6.7mm(±2.1)"));
						tableWidget->setItem(11, 2, new QTableWidgetItem(QString::number(L1_NBmm) + "mm"));
						tableWidget->setItem(11, 3, new QTableWidgetItem(QString::number(L1_NBmm - 6.7)));

						tableWidget->setItem(12, 0, new QTableWidgetItem("U1_SN"));
						tableWidget->setItem(12, 1, new QTableWidgetItem("105.7°(±6.3)"));
						tableWidget->setItem(12, 2, new QTableWidgetItem(QString::number(U1_SN) + "°"));
						tableWidget->setItem(12, 3, new QTableWidgetItem(QString::number(U1_SN - 105.7)));

					}

					timer->stop();
					delete timer;
					});

				timer->start();
			}
			else if (stackedWidget->currentIndex() == 1)
			{
				QPixmap scaledPixmap = pixmap.scaledToWidth(850);
				imageLabel2->setPixmap(scaledPixmap);

				const char* filename = "D:/Data/teeth.nii.gz";

				// 创建 NIFTI 图片读取器
				vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
				reader->SetFileName(filename);
				reader->Update();

				std::vector<Point3D> tooth_voxels;
				// 获取图像数据
				vtkSmartPointer<vtkImageData> imageData = reader->GetOutput();

				// 获取图像尺寸
				int dimensions[3];
				imageData->GetDimensions(dimensions);
				int nx = dimensions[0];  // 宽度
				int ny = dimensions[1];  // 高度
				int nz = dimensions[2];  // 深度

				double range[2];
				imageData->GetScalarRange(range);

				int maxRange = range[1];

				Tooth* upperTeeth;
				Tooth* lowerTeeth;
				Tooth* teeth = new Tooth[maxRange];

				int upper = 0, lower = 0;
				double sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;
				double length = 0.0, width = 0.0;

				Tooth tooth;

				// 访问体素值
				float* data = static_cast<float*>(imageData->GetScalarPointer());
				int i = 0;
				for (int i = 1; i <= maxRange; i++) {
					tooth_voxels.clear();

					// 遍历所有体素
					for (int z = 0; z < nz; ++z) {
						for (int y = 0; y < ny; ++y) {
							for (int x = 0; x < nx; ++x) {
								// 计算当前体素的索引
								int index = x + y * nx + z * nx * ny;

								// 获取体素值
								float voxel_value = data[index];
								if (voxel_value == i) {
									tooth = Tooth();
									double voxel_coordinates[3];
									imageData->GetOrigin(voxel_coordinates);
									double voxel_spacing[3];
									imageData->GetSpacing(voxel_spacing);
									double real_x = voxel_coordinates[0] + x * voxel_spacing[0];
									double real_y = voxel_coordinates[1] + y * voxel_spacing[1];
									double real_z = voxel_coordinates[2] + z * voxel_spacing[2];
									tooth_voxels.push_back({ real_x, real_y, real_z });
								}

							}
						}
					}

					length = 0;
					width = 0;
					sum_x = 0;
					sum_y = 0;
					sum_z = 0;

					for (const auto& voxel : tooth_voxels) {
						sum_x += voxel.x;
						sum_y += voxel.y;
						sum_z += voxel.z;
					}
					Point3D center;
					center.x = sum_x / tooth_voxels.size();
					center.y = sum_y / tooth_voxels.size();
					center.z = sum_z / tooth_voxels.size();

					tooth.center.x = center.x;
					tooth.center.y = center.y;
					tooth.center.z = center.z;

					// 计算长宽数据
					double min_x = tooth_voxels[0].x;
					double max_x = tooth_voxels[0].x;
					double min_y = tooth_voxels[0].y;
					double max_y = tooth_voxels[0].y;
					double min_z = tooth_voxels[0].z;
					double max_z = tooth_voxels[0].z;

					for (const auto& voxel : tooth_voxels) {
						min_x = std::min(min_x, voxel.x);
						max_x = std::max(max_x, voxel.x);
						min_y = std::min(min_y, voxel.y);
						max_y = std::max(max_y, voxel.y);
						min_z = std::min(min_z, voxel.z);
						max_z = std::max(max_z, voxel.z);
					}

					length = max_z - min_z;
					width = std::max(max_x - min_x, max_y - min_y);

					tooth.length = length;
					tooth.width = width;

					if (center.z <= 70) {
						lower++;
					}
					else { upper++; }

					teeth[i - 1] = tooth;
				}

				upperTeeth = new Tooth[upper];
				lowerTeeth = new Tooth[lower];

				int lowerIndex = 0, upperIndex = 0;

				for (int i = 0; i < maxRange; i++) {
					tooth = teeth[i];
					if (tooth.center.z < 70) {
						lowerTeeth[lowerIndex] = tooth;
						lowerIndex++;
					}
					else {
						upperTeeth[upperIndex] = tooth;
						upperIndex++;
					}
				}

				tableWidget2->setRowCount(5 + maxRange);

				tableWidget2->setItem(0, 0, new QTableWidgetItem("牙弓长度"));
				std::vector<Tooth> importantteethup = findExtremeTeeth1(upperTeeth, upper);
				std::vector<Tooth> importantteethdown = findExtremeTeeth1(lowerTeeth, lower);
				length = calculateArchLength(importantteethup[0], importantteethup[2], importantteethup[3], importantteethup[1]);
				tableWidget2->setItem(0, 1, new QTableWidgetItem(QString::number(length, 'f', 1) + "mm"));
				tableWidget2->setItem(1, 0, new QTableWidgetItem("牙弓宽度"));
				width = calculateArchWidth(importantteethup[2], importantteethup[3]);
				tableWidget2->setItem(1, 1, new QTableWidgetItem(QString::number(width, 'f', 1) + "mm"));
				tableWidget2->setItem(2, 0, new QTableWidgetItem("拥挤度"));
				double crowding = evaluateCrowdingLevel(upperTeeth, length, upper);
				if (crowding < 0.02) {
					//std::cout << "牙齿排列稀疏" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列稀疏"));
				}
				else if (crowding < 0.04) {
					//std::cout << "牙齿排列中等" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列中等"));
				}
				else {
					//std::cout << "牙齿排列拥挤" << std::endl;
					tableWidget2->setItem(2, 1, new QTableWidgetItem("牙齿排列拥挤"));
				}
				tableWidget2->setItem(3, 0, new QTableWidgetItem("Bolton指数"));
				double bolton = calculateOverallBoltonIndex(upperTeeth, lowerTeeth, upper, lower);
				tableWidget2->setItem(3, 1, new QTableWidgetItem(QString::number(bolton * 100, 'f', 1) + "%"));
				tableWidget2->setItem(4, 0, new QTableWidgetItem("中线关系"));
				double middle = calculate_middle(importantteethup, importantteethdown);
				if (std::abs(middle) >= 2)
				{
					//std::cout << "中线偏移量过大！" << std::endl;
					tableWidget2->setItem(4, 1, new QTableWidgetItem("中线偏移量过大"));
				}
				else
				{
					//std::cout << "中线偏移量正常！" << std::endl;
					tableWidget2->setItem(4, 1, new QTableWidgetItem("中线偏移量正常"));
				}
				int index = 5;
				for (int i = 0; i < upper; i++) {
					tableWidget2->setItem(index, 0, new QTableWidgetItem("上排第" + QString::number(i + 1) + "颗牙齿的长宽"));
					tableWidget2->setItem(index, 1, new QTableWidgetItem(QString::number(upperTeeth[i].length) + "mm, " + QString::number(upperTeeth[i].width) + "mm"));
					index++;
				}
				for (int i = 0; i <= lower; i++) {
					tableWidget2->setItem(index, 0, new QTableWidgetItem("下排第" + QString::number(i + 1) + "颗牙齿的长宽"));
					tableWidget2->setItem(index, 1, new QTableWidgetItem(QString::number(lowerTeeth[i].length) + "mm, " + QString::number(lowerTeeth[i].width) + "mm"));
					index++;
				}



			}
		}
	}*/
}

void PDFExport::onPageSwitchClicked()
{
	// 切换到下一页
	int nextPageIndex = (stackedWidget->currentIndex() + 1) % stackedWidget->count();
	stackedWidget->setCurrentIndex(nextPageIndex);
}
