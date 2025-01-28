/*
 *
 * 程序的欢迎界面
 *
 */
#include "WelcomeWidget.h"
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include "CarousellmageWindow.h"

WelcomeWidget::WelcomeWidget(QWidget* parent)
    : QWidget(parent)
{
    CarouselImageWindow* carouselImageWindow = new CarouselImageWindow(this);
    carouselImageWindow->addImage("images/1.png");
    carouselImageWindow->addImage("images/2.png");
    carouselImageWindow->addImage("images/3.png");
    carouselImageWindow->addImage("images/4.png");
    carouselImageWindow->startPlay();
    //    SQLServerConnection connection;
    //    std::wstring connectionString = L"DRIVER={SQL Server};SERVER=MS;DATABASE=Dicom;UID=sa;PWD=123456;";
    //   connection.connect(connectionString);


    //    std::vector<std::vector<std::string>> columnData = connection.readTableColumnsData(tableName, columnName);
    //   int numRows = columnData.size();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 创建标题标签并设置样式
    label = new QLabel(this);
    label->setText("医学影像智能处理与分析系统");
    QFont font = label->font();
    font.setPointSize(24);
    font.setBold(true);
    label->setFont(font);
    label->setAlignment(Qt::AlignCenter);

    QLabel* introLabel0 = new QLabel(this);
    introLabel0->setText("面向精准正畸的多功能医学影像智能处理与分析系统");
    QFont font1 = label->font();
    font1.setPointSize(10);
    font1.setBold(true);
    introLabel0->setFont(font1);
    introLabel0->setAlignment(Qt::AlignCenter);


    carouselImageWindow->setFixedSize(600, 400);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->addWidget(carouselImageWindow);


    QLabel* imageNumberLabel = new QLabel(this);
    imageNumberLabel->setAlignment(Qt::AlignCenter);
    QFont font2 = label->font();
    font2.setPointSize(16);
    font2.setBold(true);
    imageNumberLabel->setFont(font2);
    imageNumberLabel->setText("数据管理模块");

    QLabel* imageNumberLabe2 = new QLabel(this);
    imageNumberLabe2->setAlignment(Qt::AlignCenter);
    QFont font3 = label->font();
    font3.setPointSize(10);
    font3.setBold(true);
    imageNumberLabe2->setFont(font3);
    imageNumberLabe2->setText("自标准化DICOM医学影像格式储存和管理，自动建立患者档案，支持数据迁移备份、脱敏导出");
    //  horizontalLayout->addWidget(imageNumberLabel);



    QLabel* introLabel1 = new QLabel(this);
    introLabel1->setText("欢迎使用医学影像智能处理与分析系统！");
    introLabel1->setAlignment(Qt::AlignCenter);

    QLabel* introLabel2 = new QLabel(this);
    introLabel2->setText("系统提供强大的图像处理算法和数据分析功能，帮助医生和研究人员");
    introLabel2->setAlignment(Qt::AlignCenter);

    QLabel* introLabel3 = new QLabel(this);
    introLabel3->setText("更快速、准确地分析和诊断医学影像数据。");
    introLabel3->setAlignment(Qt::AlignCenter);

    // 在label和carouselImageWindow之后加入一个垂直空白间距
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    QSpacerItem* verticalSpacer1 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    QSpacerItem* verticalSpacer2 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    // 添加介绍标签到mainLayout
    mainLayout->addWidget(label);
    mainLayout->addWidget(introLabel0);
    mainLayout->addItem(verticalSpacer1);
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addItem(verticalSpacer2);
    mainLayout->addWidget(imageNumberLabel);
    mainLayout->addWidget(imageNumberLabe2);
    mainLayout->addItem(verticalSpacer);
    mainLayout->addWidget(introLabel1);
    mainLayout->addWidget(introLabel2);
    mainLayout->addWidget(introLabel3);



    connect(carouselImageWindow, &CarouselImageWindow::currentImageChanged, this, [=]() {
        QString text;
        QString text1;
        if (carouselImageWindow->getCurrentImageIndex() + 1 == 1) {
            text = "可视化模块";
            text1 = "支持二维与三维影像的可视化与交互操作";
        }
        else if (carouselImageWindow->getCurrentImageIndex() + 1 == 2) {
            text = "图像智能算法模块";
            text1 = "基于算法实现医学图像中的自动或半自动分割，支持根据图像类型推荐可使用的分割算法";
        }
        else if (carouselImageWindow->getCurrentImageIndex() + 1 == 3) {
            text = "数据管理模块";
            text1 = "标准化DICOM医学影像格式储存和管理，自动建立患者档案，支持数据迁移备份、脱敏导出";
        }
        else if (carouselImageWindow->getCurrentImageIndex() + 1 == 4) {
            text = "数据分析模块";
            text1 = "根据测量结果，进行正畸学分析和诊断，导出分析报告";
        }
        imageNumberLabel->setText(text);
        imageNumberLabe2->setText(text1);
        });



    //  mainLayout->addStretch();

    //resize(1366, 768);
}

WelcomeWidget::~WelcomeWidget()
{

}

void WelcomeWidget::refresh()
{

}
 