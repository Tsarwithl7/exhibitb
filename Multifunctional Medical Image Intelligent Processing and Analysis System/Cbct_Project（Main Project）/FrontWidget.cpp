#include "FrontWidget.h"
MeasurementTool1::MeasurementTool1(QLabel* distanceLabel, QSlider* contrastSlider, QSlider* brightnessSlider, QButtonGroup* modeButtonGroup, QWidget* parent)
    : QGraphicsView(parent), distanceLabel(distanceLabel), contrastSlider(contrastSlider), brightnessSlider(brightnessSlider)
{

    interactivePoints.clear(); // 初始化为空的点集
    scene = new QGraphicsScene(this);
    setScene(scene);
    setDragMode(QGraphicsView::RubberBandDrag);
    currentLineItem = nullptr;
    totalDistance = 0.0;
    mode = "测量模式";

    connect(contrastSlider, &QSlider::valueChanged, this, &MeasurementTool1::updateContrast);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MeasurementTool1::updateBrightness);
    connect(distanceLabel, &QLabel::customContextMenuRequested, this, &MeasurementTool1::clearMeasurements);
    // connect(modeButtonGroup, &QButtonGroup::buttonClicked, this, &MeasurementTool1::setButtonValue);
}

void MeasurementTool1::resizeEvent(QResizeEvent* event)
{
    fitImageInView();
    QGraphicsView::resizeEvent(event);
}

void MeasurementTool1::fitImageInView()
{
    if (scene->items().isEmpty())
        return;

    pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (pixmapItem) {
        fitInView(pixmapItem, Qt::KeepAspectRatio);
    }
}

void MeasurementTool1::mouseReleaseEvent(QMouseEvent* event)
{
    if (mode == "测量模式" && event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QPointF clickPos = mapToScene(event->pos());
        if (!startPoint.isNull()) {
            QGraphicsLineItem* lineItem = scene->addLine(QLineF(startPoint, clickPos));

            QPen pen(Qt::red);
            pen.setWidth(5);
            lineItem->setPen(pen);

            qreal distance = QLineF(startPoint, clickPos).length();
            totalDistance += distance;  // 累加距离
            distanceLabel->setText(QString("总距离: %1").arg(totalDistance));

            startPoint = QPointF(); // 重置startPoint为空
            lines.append(lineItem); // 添加绘制的线段到lines列表中

        }
        else {
            startPoint = clickPos; // 当startPoint为空时，将clickPos赋值给startPoint
        }
    }

    if (mode == "标记模式" && event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QPointF clickPos = mapToScene(event->pos());
        if (!startPoint.isNull()) {
            // 绘制矩形
            QRectF rect(startPoint, mapToScene(event->pos()));
            QGraphicsRectItem* rectItem = scene->addRect(rect);

            QPen pen(Qt::red);
            pen.setWidth(5);
            rectItem->setPen(pen);
            QPointF clickPos = mapToScene(event->pos());
            // 记录点击的坐标信息
           // 保存矩形的左上角和右下角两个点的坐标
            topLeft = startPoint;
            bottomRight = clickPos;
            interactivePoints.append(topLeft);
            interactivePoints.append(bottomRight);

            startPoint = QPointF(); // 重置startPoint为空
            rects.append(rectItem);
        }
        else {
            startPoint = mapToScene(event->pos()); // 当startPoint为空时，将鼠标点击位置映射到场景坐标系，并赋值给startPoint
        }
    }
    if (mode == "裁剪模式" && event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QPointF clickPos = mapToScene(event->pos());
        if (!startPoint.isNull()) {
            // 绘制矩形
            QRectF rect(startPoint, clickPos);
            QGraphicsRectItem* rectItem = scene->addRect(rect);
            //rectItem->setVisible(false);

            QPen pen(Qt::red);
            pen.setWidth(5);
            rectItem->setPen(pen);

            if (pixmapItem) {
                QPixmap originalPixmap = pixmapItem->pixmap();
                QPixmap croppedPixmap = originalPixmap.copy(rect.toRect());

                QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "是否保存刚刚所框选的矩形区域内的图片？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {

                    QString path = QFileDialog::getSaveFileName(this, tr("图像另存为"), ".", tr("Images(*.jpg *.png *.bmp)"));
                    if (!path.isEmpty())
                    {
                        QImage img = croppedPixmap.toImage();
                        img.save(path);
                    }
                }
                else {
                }

            }
            rectItem->setVisible(false);
            startPoint = QPointF(); // 重置startPoint为空
        }
        else {
            startPoint = clickPos; // 当startPoint为空时，将clickPos赋值给startPoint
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void MeasurementTool1::clearMeasurements()
{
    totalDistance = 0.0;
    distanceLabel->setText("总距离: 0");
    for (QGraphicsLineItem* lineItem : lines) {
        scene->removeItem(lineItem);
    }
}

void MeasurementTool1::clearRects()
{
    for (QGraphicsRectItem* rectItem : rects) {
        scene->removeItem(rectItem);
    }
}


void MeasurementTool1::updateContrast(int value)
{
    int red, green, blue;
    qreal contrast = static_cast<qreal>(value) / 150.0;
    qDebug() << 1;

    //QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (pixmapItem) {
        QPixmap pixmap = originalPixmap;

        QImage image = pixmap.toImage();
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QRgb pixel = image.pixel(x, y);

                long long nred = qRed(pixel);
                long long ngreen = qGreen(pixel);
                long long nblue = qBlue(pixel);

                red = nred + (nred - 127) * contrast;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = ngreen + (ngreen - 127) * contrast;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nblue + (nblue - 127) * contrast;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                image.setPixel(x, y, qRgb(red, green, blue));
            }
        }

        pixmap = QPixmap::fromImage(image);
        pixmapItem->setPixmap(pixmap);
    }
}

void MeasurementTool1::updateBrightness(int value)
{
    int red, green, blue;
    qreal contrast = static_cast<qreal>(value) / 200.0;

    //QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (pixmapItem) {
        QPixmap pixmap = originalPixmap;

        QImage image = pixmap.toImage();
        int pixels = image.width() * image.height();
        unsigned int* data = (unsigned int*)image.bits();
        for (int i = 0; i < pixels; ++i)
        {
            red = qRed(data[i]) + value;
            red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
            green = qGreen(data[i]) + value;
            green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
            blue = qBlue(data[i]) + value;
            blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }

        pixmap = QPixmap::fromImage(image);
        pixmapItem->setPixmap(pixmap);
    }
}

void MeasurementTool1::setButtonValue(QString value)
{
    mode = value;
}

FrontWidget::FrontWidget(QWidget* parent)
    : QMainWindow(parent)
{
    splitter = new QSplitter(this);

    dockWidget = new QDockWidget("工具栏", this);
    dockWidgetContent = new QWidget(dockWidget);
    dockWidgetLayout = new QVBoxLayout(dockWidgetContent);
    QFont font2;
    font2.setPointSize(12);
    QFont font;
    font.setPointSize(16);  // 设置字号为 16
    font.setBold(true);
    openButtonUpper = new QPushButton("选择图片", dockWidgetContent);
    openButtonUpper->setFixedSize(150, 50);
    dockWidgetLayout->addWidget(openButtonUpper);
    openButtonUpper->setFont(font2);
    openButtonUpper->setVisible(false);
    QFont font1;
    font1.setPointSize(13);

    contrastLabel = new QLabel("对比度", dockWidgetContent);
    dockWidgetLayout->addWidget(contrastLabel);
    contrastLabel->setFont(font1);
    contrastSlider = new QSlider(Qt::Horizontal, dockWidgetContent);
    contrastSlider->setRange(-100, 100);
    contrastSlider->setValue(0);
    dockWidgetLayout->addWidget(contrastSlider);

    brightnessLabel = new QLabel("亮度", dockWidgetContent);
    dockWidgetLayout->addWidget(brightnessLabel);
    brightnessLabel->setFont(font1);
    brightnessSlider = new QSlider(Qt::Horizontal, dockWidgetContent);
    brightnessSlider->setRange(-100, 100);
    brightnessSlider->setValue(0);
    dockWidgetLayout->addWidget(brightnessSlider);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 80));

    separatorLine1 = new QFrame(dockWidgetContent);
    separatorLine1->setFrameShape(QFrame::HLine);
    separatorLine1->setFrameShadow(QFrame::Sunken);
    dockWidgetLayout->addWidget(separatorLine1);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 30));

    segmentationLabel = new QLabel("选择牙齿分割算法", dockWidgetContent);
    dockWidgetLayout->addWidget(segmentationLabel);

    segmentationLabel->setFont(font);
    buttonGridLayout = new QGridLayout();
    buttonGroup = new QButtonGroup(this);
    // 设置字号为 16
    radioButton1 = new QRadioButton("区域生长算法", this);
    buttonGroup->addButton(radioButton1);
    buttonGridLayout->addWidget(radioButton1, 0, 0);
    radioButton1->setFont(font1);
    radioButton2 = new QRadioButton("区域分裂合并算法", this);
    buttonGroup->addButton(radioButton2);
    buttonGridLayout->addWidget(radioButton2, 0, 1);
    radioButton2->setFont(font1);
    radioButton3 = new QRadioButton("分水岭算法", this);
    buttonGroup->addButton(radioButton3);
    buttonGridLayout->addWidget(radioButton3, 1, 0);
    radioButton3->setFont(font1);
    radioButton4 = new QRadioButton("SAM", this);
    buttonGroup->addButton(radioButton4);
    buttonGridLayout->addWidget(radioButton4, 1, 1);
    radioButton4->setFont(font1);
    radioButton5 = new QRadioButton("交互式SAM", this);
    buttonGroup->addButton(radioButton5);
    buttonGridLayout->addWidget(radioButton5, 2, 0);
    radioButton1->setChecked(true);
    radioButton5->setFont(font1);
    dockWidgetLayout->addLayout(buttonGridLayout);

    segmentationButton = new QPushButton("查看牙齿分割结果", dockWidgetContent);
    segmentationButton->setFixedSize(165, 50);
    dockWidgetLayout->addWidget(segmentationButton);
    // 设置字号为 16
//  font2.setBold(true);
    segmentationButton->setFont(font2);
    segmentationButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 50));

    separatorLine2 = new QFrame(dockWidgetContent);
    separatorLine2->setFrameShape(QFrame::HLine);
    separatorLine2->setFrameShadow(QFrame::Sunken);
    dockWidgetLayout->addWidget(separatorLine2);

    //   dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 50));

    modeLabel = new QLabel("请选择操作模式", dockWidgetContent);
    dockWidgetLayout->addWidget(modeLabel);
    modeLabel->setFont(font);
    modeButtonLayout = new QGridLayout();
    modeButtonGroup = new QButtonGroup(this);

    modeButton1 = new QRadioButton("测量模式", this);
    modeButtonGroup->addButton(modeButton1);
    modeButtonLayout->addWidget(modeButton1, 0, 0);
    modeButton1->setFont(font1);
    modeButton2 = new QRadioButton("标记模式", this);
    modeButtonGroup->addButton(modeButton2);
    modeButtonLayout->addWidget(modeButton2, 0, 1);
    modeButton2->setFont(font1);
    modeButton3 = new QRadioButton("裁剪模式", this);
    modeButtonGroup->addButton(modeButton3);
    modeButtonLayout->addWidget(modeButton3, 1, 0);
    modeButton3->setFont(font1);
    modeButton1->setChecked(true);

    dockWidgetLayout->addLayout(modeButtonLayout);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 30));

    distanceLabel = new QLabel("距离: 0", dockWidgetContent);
    distanceLabel->setAlignment(Qt::AlignLeft);
    dockWidgetLayout->addWidget(distanceLabel);
    distanceLabel->setFont(font);
    clearButton = new QPushButton("清空距离", dockWidgetContent);
    clearButton->setFixedSize(165, 50);
    dockWidgetLayout->addWidget(clearButton);
    clearButton->setFont(font2);
    clearButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    clearRectButton = new QPushButton("清空标记", dockWidgetContent);
    clearRectButton->setFixedSize(165, 50);
    dockWidgetLayout->addWidget(clearRectButton);
    clearRectButton->setFont(font2);
    clearRectButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    dockWidgetContent->setLayout(dockWidgetLayout);
    dockWidget->setWidget(dockWidgetContent);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    dockWidget->setMinimumWidth(width() / 2);

    rightWidget = new QWidget(splitter);
    rightLayout = new QVBoxLayout(rightWidget);

    measurementTool1 = new MeasurementTool1(distanceLabel, contrastSlider, brightnessSlider, modeButtonGroup);
    rightLayout->addWidget(measurementTool1);

    rightWidget->setLayout(rightLayout);
    splitter->addWidget(rightWidget);

    connect(openButtonUpper, &QPushButton::clicked, this, &FrontWidget::openImage);
    connect(clearButton, &QPushButton::clicked, this, &FrontWidget::clearDistance);
    connect(clearRectButton, &QPushButton::clicked, this, &FrontWidget::clearRects);
    connect(clearRectButton, &QPushButton::clicked, this, &FrontWidget::clearRects);
    connect(segmentationButton, &QPushButton::clicked, this, &FrontWidget::onSegmentationButtonClicked);
    connect(modeButtonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), [=](QAbstractButton* button) {
        QString mode = button->text();
        measurementTool1->setButtonValue(mode);
        });
    resize(1200, 800);
    setCentralWidget(splitter);
}

void FrontWidget::open(const char* path)
{
    QString str(path);
    if (!str.isEmpty()) {
        filePath = str;
        QPixmap image(filePath);
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
        measurementTool1->originalPixmap = image;
        measurementTool1->scene->clear();
        measurementTool1->scene->addItem(pixmapItem);
        measurementTool1->fitImageInView();
    }
}
void FrontWidget::openImage()
{
    filePath = QFileDialog::getOpenFileName(nullptr, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");
    //QString filePath = "D://CBCT//1.JPG";

    if (!filePath.isEmpty()) {
        QPixmap image(filePath);
        QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
        measurementTool1->originalPixmap = image;
        measurementTool1->scene->clear();
        measurementTool1->scene->addItem(pixmapItem);
        measurementTool1->fitImageInView();
    }
}
void FrontWidget::onSegmentationButtonClicked()
{
    std::string jsonString = "0";
    // 检查哪个radioButton被选中
    if (radioButton1->isChecked()) {
        SelectNumber = 1;

        qDebug() << "  select 1";
        Json::Value param;  // 创建JSON对象

        // 构建内部嵌套的数组
        Json::Value box(Json::arrayValue);
        Json::Value point1(Json::arrayValue);
        point1.append(400);
        point1.append(600);
        Json::Value point2(Json::arrayValue);
        point2.append(1200);
        point2.append(1000);
        box.append(point1);
        box.append(point2);

        // 添加键值对到param对象中
        param["box"] = box;

        // 将JSON对象转换为字符串
        Json::FastWriter writer;
        std::string jsonString = writer.write(param);
        // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
        std::string filePathUtf8 = filePath.toUtf8().constData();
        //Message_str::runPythonFunction(SelectNumber, jsonString, filePathUtf8, " ");
        qDebug() << " 1通讯函数启动";
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
        connect(timer, &QTimer::timeout, this, [=]() {
            QDir directory("D:/Data");
            QStringList files = directory.entryList(QStringList() << "return.jpg", QDir::Files);
            if (files.size() > 0) {
                QString imagePath = directory.absoluteFilePath(files.at(0));
                QPixmap image(imagePath);
                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
                measurementTool1->scene->clear();
                measurementTool1->scene->addItem(pixmapItem);
                measurementTool1->fitImageInView();
                timer->stop();
                delete timer;
            }
            });

        timer->start();
    }
    else if (radioButton2->isChecked()) {
        SelectNumber = 2;
        qDebug() << "  select 2";
        Json::Value param;  // 创建JSON对象

        // 构建内部嵌套的数组
        Json::Value box(Json::arrayValue);
        Json::Value point1(Json::arrayValue);
        point1.append(400);
        point1.append(600);
        Json::Value point2(Json::arrayValue);
        point2.append(1200);
        point2.append(1000);
        box.append(point1);
        box.append(point2);

        // 添加键值对到param对象中
        param["box"] = box;

        // 将JSON对象转换为字符串
        Json::FastWriter writer;
        std::string jsonString = writer.write(param);
        // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
        std::string filePathUtf8 = filePath.toUtf8().constData();
        Message_str::runPythonFunction(SelectNumber, jsonString, filePathUtf8, " ");
        qDebug() << " 2通讯函数启动";
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
        connect(timer, &QTimer::timeout, this, [=]() {
            QDir directory("C:/Users/sun13/Desktop/TEST");
            QStringList files = directory.entryList(QStringList() << "return.jpg", QDir::Files);
            if (files.size() > 0) {
                QString imagePath = directory.absoluteFilePath(files.at(0));
                QPixmap image(imagePath);
                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
                measurementTool1->scene->clear();
                measurementTool1->scene->addItem(pixmapItem);
                measurementTool1->fitImageInView();
                timer->stop();
                delete timer;
            }
            });

        timer->start();
    }
    else if (radioButton3->isChecked()) {
        SelectNumber = 3;
        qDebug() << "  select 3";
        Json::Value param;  // 创建JSON对象

        // 构建内部嵌套的数组
        Json::Value box(Json::arrayValue);
        Json::Value point1(Json::arrayValue);
        point1.append(400);
        point1.append(600);
        Json::Value point2(Json::arrayValue);
        point2.append(1200);
        point2.append(1000);
        box.append(point1);
        box.append(point2);

        // 添加键值对到param对象中
        param["box"] = box;

        // 将JSON对象转换为字符串
        Json::FastWriter writer;
        std::string jsonString = writer.write(param);
        // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
        std::string filePathUtf8 = filePath.toUtf8().constData();
        Message_str::runPythonFunction(SelectNumber, jsonString, filePathUtf8, " ");
        qDebug() << " 3通讯函数启动";
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
        connect(timer, &QTimer::timeout, this, [=]() {
            QDir directory("C:/Users/sun13/Desktop/TEST");
            QStringList files = directory.entryList(QStringList() << "return.jpg", QDir::Files);
            if (files.size() > 0) {
                QString imagePath = directory.absoluteFilePath(files.at(0));
                QPixmap image(imagePath);
                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
                measurementTool1->scene->clear();
                measurementTool1->scene->addItem(pixmapItem);
                measurementTool1->fitImageInView();
                timer->stop();
                delete timer;
            }
            });

        timer->start();
    }
    else if (radioButton4->isChecked()) {
        SelectNumber = 4;
        qDebug() << " select 4";
        Json::Value param;  // 创建JSON对象

        // 构建内部嵌套的数组
        Json::Value box(Json::arrayValue);
        Json::Value point1(Json::arrayValue);
        point1.append(400);
        point1.append(600);
        Json::Value point2(Json::arrayValue);
        point2.append(1200);
        point2.append(1000);
        box.append(point1);
        box.append(point2);

        // 添加键值对到param对象中
        param["box"] = box;

        // 将JSON对象转换为字符串
        Json::FastWriter writer;
        std::string jsonString = writer.write(param);
        // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
        std::string filePathUtf8 = filePath.toUtf8().constData();
        Message_str::runPythonFunction(SelectNumber, jsonString, filePathUtf8, " ");
        qDebug() << " 4通讯函数启动";
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
        connect(timer, &QTimer::timeout, this, [=]() {
            QDir directory("C:/Users/sun13/Desktop/TEST");
            QStringList files = directory.entryList(QStringList() << "return.jpg", QDir::Files);
            if (files.size() > 0) {
                QString imagePath = directory.absoluteFilePath(files.at(0));
                QPixmap image(imagePath);
                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
                measurementTool1->scene->clear();
                measurementTool1->scene->addItem(pixmapItem);
                measurementTool1->fitImageInView();
                timer->stop();
                delete timer;
            }
            });

        timer->start();
    }
    else if (radioButton5->isChecked()) {
        SelectNumber = 11;
        // 获取interactivePoints的坐标信息
        Json::Value param;  // 创建JSON对象
        if (!measurementTool1->interactivePoints.isEmpty()) {
            QPointF topLeft = measurementTool1->interactivePoints.first();
            QPointF bottomRight = measurementTool1->interactivePoints.last();
            Json::Value box(Json::arrayValue);
            Json::Value point1(Json::arrayValue);
            point1.append(static_cast<int>(topLeft.x()));
            point1.append(static_cast<int>(topLeft.y()));
            Json::Value point2(Json::arrayValue);
            point2.append(static_cast<int>(bottomRight.x()));
            point2.append(static_cast<int>(bottomRight.y()));
            box.append(point1);
            box.append(point2);
            param["point"] = point1;

        }
        // 将JSON对象转换为字符串
        Json::FastWriter writer;
        jsonString = writer.write(param);
        qDebug() << jsonString;
        qDebug() << " select 5";
        // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
        std::string filePathUtf8 = filePath.toUtf8().constData();
        //Message_str::runPythonFunction(SelectNumber, jsonString, filePathUtf8, " ");
        qDebug() << " 5通讯函数启动";
        QTimer* timer = new QTimer(this);
        timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
        connect(timer, &QTimer::timeout, this, [=]() {
            QDir directory("C:/Users/sun13/Desktop/massive/");
            QStringList files = directory.entryList(QStringList() << "sam.jpg", QDir::Files);
            if (files.size() > 0) {
                QString imagePath = directory.absoluteFilePath(files.at(0));
                QPixmap image(imagePath);
                QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
                measurementTool1->scene->clear();
                measurementTool1->scene->addItem(pixmapItem);
                measurementTool1->fitImageInView();
                timer->stop();
                delete timer;
            }
            });

        timer->start();
    }

}

void FrontWidget::clearDistance()
{
    measurementTool1->clearMeasurements();
}

void FrontWidget::clearRects()
{
    measurementTool1->clearRects();
}
