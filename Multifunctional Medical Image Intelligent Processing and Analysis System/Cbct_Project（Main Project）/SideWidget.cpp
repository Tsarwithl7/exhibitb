#include "SideWidget.h"
#include <Qstring>
#include "message.h"

MeasurementTool::MeasurementTool(QLabel* distanceLabel, QSlider* contrastSlider, QSlider* brightnessSlider, QWidget* parent)
    : QGraphicsView(parent), distanceLabel(distanceLabel), contrastSlider(contrastSlider), brightnessSlider(brightnessSlider)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setDragMode(QGraphicsView::RubberBandDrag);
    currentLineItem = nullptr;
    totalDistance = 0.0;
    mode = "测量模式";

    connect(contrastSlider, &QSlider::valueChanged, this, &MeasurementTool::updateContrast);
    connect(brightnessSlider, &QSlider::valueChanged, this, &MeasurementTool::updateBrightness);
    connect(distanceLabel, &QLabel::customContextMenuRequested, this, &MeasurementTool::clearMeasurements);
}

void MeasurementTool::resizeEvent(QResizeEvent* event)
{
    fitImageInView();
    QGraphicsView::resizeEvent(event);
}

void MeasurementTool::fitImageInView()
{
    if (scene->items().isEmpty())
        return;

    pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (pixmapItem) {
        fitInView(pixmapItem, Qt::KeepAspectRatio);
    }
}

void MeasurementTool::mouseReleaseEvent(QMouseEvent* event)
{

    if (mode == "测量模式" && event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QPointF clickPos = mapToScene(event->pos());
        if (!startPoint.isNull()) {
            QGraphicsLineItem* lineItem = scene->addLine(QLineF(startPoint, clickPos));

            QPen pen(Qt::red);
            pen.setWidth(5);
            lineItem->setPen(pen);

            qreal distance = QLineF(startPoint, clickPos).length();
            if (trans == 0) {
                distanceLabel->setText("请先在比例尺测量模式中测量图片中比例尺一个小格的距离");
            }
            else {
                totalDistance += (distance / trans);  // 累加距离
                QString formattedDistance = QString::number(totalDistance, 'f', 1);
                distanceLabel->setText(QString("总距离: %1").arg(formattedDistance) + "cm");
            }

            startPoint = QPointF(); // 重置startPoint为空
            lines.append(lineItem); // 添加绘制的线段到lines列表中
        }
        else {
            startPoint = clickPos; // 当startPoint为空时，将clickPos赋值给startPoint
        }
    }

    if (mode == "比例尺测量模式" && event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier) {
        QPointF clickPos = mapToScene(event->pos());
        if (!startPoint.isNull()) {
            QGraphicsLineItem* lineItem = scene->addLine(QLineF(startPoint, clickPos));

            QPen pen(Qt::red);
            pen.setWidth(5);
            lineItem->setPen(pen);

            qreal distance = QLineF(startPoint, clickPos).length();
            trans = distance;

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

                // 绘制标记到裁剪后的图片
                QPainter painter(&croppedPixmap);
                for (QGraphicsRectItem* rect : rects) {
                    QRectF rectScene = rect->rect();
                    QRect rectPixmap = rectScene.toRect().translated(-rectScene.topLeft().toPoint());
                    painter.setPen(rect->pen());
                    painter.drawRect(rectPixmap);
                }
                painter.end();

                QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation", "是否保存刚刚所框选的矩形区域内的图片及标记？", QMessageBox::Yes | QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    QString path = QFileDialog::getSaveFileName(this, tr("图像另存为"), ".", tr("Images(*.jpg *.png *.bmp)"));
                    if (!path.isEmpty()) {
                        croppedPixmap.save(path);
                    }
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



void MeasurementTool::clearMeasurements()
{
    totalDistance = 0.0;
    distanceLabel->setText("总距离: 0");
    for (QGraphicsLineItem* lineItem : lines) {
        scene->removeItem(lineItem);
    }
}

void MeasurementTool::clearRects()
{
    for (QGraphicsRectItem* rectItem : rects) {
        scene->removeItem(rectItem);
    }
}

void MeasurementTool::updateContrast(int value)
{
    int red, green, blue;
    qreal contrast = static_cast<qreal>(value) / 150.0;

    //QGraphicsPixmapItem* pixmapItem = qgraphicsitem_cast<QGraphicsPixmapItem*>(scene->items().first());
    if (pixmapItem) {
        QPixmap pixmap = originalPixmap;

        QImage image = pixmap.toImage();
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                QRgb pixel = image.pixel(x, y);

                int nred = qRed(pixel);
                int ngreen = qGreen(pixel);
                int nblue = qBlue(pixel);

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

void MeasurementTool::updateBrightness(int value)
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

void MeasurementTool::setButtonValue(QString value)
{
    mode = value;
}

SideWidget::SideWidget(QWidget* parent)
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
    openButtonUpper->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    openButtonUpper->setFont(font2);
    openButtonUpper->setVisible(false);
    dockWidgetLayout->addWidget(openButtonUpper);
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

    dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 80));

    separatorLine1 = new QFrame(dockWidgetContent);
    separatorLine1->setFrameShape(QFrame::HLine);
    separatorLine1->setFrameShadow(QFrame::Sunken);
    dockWidgetLayout->addWidget(separatorLine1);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 30));

    keyPointsButton = new QPushButton("查看头侧关键点", dockWidgetContent);
    keyPointsButton->setFixedSize(165, 50);
    keyPointsButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    keyPointsButton->setFont(font2);
    dockWidgetLayout->addWidget(keyPointsButton);

    fusionButton = new QPushButton("头侧照片融合", dockWidgetContent);
    fusionButton->setFixedSize(165, 50);
    fusionButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    fusionButton->setFont(font2);
    dockWidgetLayout->addWidget(fusionButton);

    dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 50));

    separatorLine2 = new QFrame(dockWidgetContent);
    separatorLine2->setFrameShape(QFrame::HLine);
    separatorLine2->setFrameShadow(QFrame::Sunken);
    dockWidgetLayout->addWidget(separatorLine2);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 50));

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
    modeButtonLayout->addWidget(modeButton3, 0, 2);
    modeButton3->setFont(font1);

    modeButton4 = new QRadioButton("比例尺测量模式", this);
    modeButtonGroup->addButton(modeButton4);
    modeButtonLayout->addWidget(modeButton4, 0, 3);
    modeButton4->setFont(font1);

    modeButton1->setChecked(true);

    dockWidgetLayout->addLayout(modeButtonLayout);

    //dockWidgetLayout->addSpacerItem(new QSpacerItem(0, 30));

    distanceLabel = new QLabel("距离: 0", dockWidgetContent);
    distanceLabel->setAlignment(Qt::AlignLeft);
    dockWidgetLayout->addWidget(distanceLabel);
    distanceLabel->setFont(font);

    clearButton = new QPushButton("清空距离", dockWidgetContent);
    clearButton->setFixedSize(165, 50);
    clearButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    clearButton->setFont(font2);
    dockWidgetLayout->addWidget(clearButton);

    clearRectButton = new QPushButton("清空标记", dockWidgetContent);
    clearRectButton->setFixedSize(165, 50);
    clearRectButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    clearRectButton->setFont(font2);
    dockWidgetLayout->addWidget(clearRectButton);

    dockWidgetContent->setLayout(dockWidgetLayout);
    dockWidget->setWidget(dockWidgetContent);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
    dockWidget->setMinimumWidth(width() / 2);

    rightWidget = new QWidget(splitter);
    rightLayout = new QVBoxLayout(rightWidget);

    measurementTool1 = new MeasurementTool(distanceLabel, contrastSlider, brightnessSlider);
    rightLayout->addWidget(measurementTool1);

    rightWidget->setLayout(rightLayout);
    splitter->addWidget(rightWidget);
    saveButton = new QPushButton("保存当前帧", dockWidgetContent);
    saveButton->setFixedSize(165, 50);
    saveButton->setStyleSheet("QPushButton {"
        "border-radius: 25px;"
        "border: 2px solid #000000;"
        "}");
    saveButton->setFont(font2);
    dockWidgetLayout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, &SideWidget::saveCurrentFrame);

    connect(openButtonUpper, &QPushButton::clicked, this, &SideWidget::openImage);
    connect(clearButton, &QPushButton::clicked, this, &SideWidget::clearDistance);
    connect(clearRectButton, &QPushButton::clicked, this, &SideWidget::clearRects);
    connect(fusionButton, &QPushButton::clicked, this, &SideWidget::onUnitButtonClicked);
    connect(keyPointsButton, &QPushButton::clicked, this, &SideWidget::onkeyPointsButtonClicked);
    connect(modeButtonGroup, qOverload<QAbstractButton*>(&QButtonGroup::buttonClicked), [=](QAbstractButton* button) {
        QString mode = button->text();
        measurementTool1->setButtonValue(mode);
        });

    resize(1200, 800);
    setCentralWidget(splitter);
}
void MeasurementTool::clearPoints()
{
    for (QGraphicsEllipseItem* pointItem : points) {
        scene->removeItem(pointItem);
    }
    points.clear();
}

void SideWidget::open(const char* path)
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

void SideWidget::openImage()
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
void SideWidget::onkeyPointsButtonClicked()
{
    //  result = QString::fromStdString(Message::runPythonFunction(5, 0, filePath.toUtf8().constData(), ""));
    result = "{\"point\": [649, 794, 1296, 698, 1173, 1003, 427, 989, 1302, 1385, 1290, 1767, 1287, 1911, 1245, 1992, 1282, 1968, 575, 1682, 1339, 1561, 1356, 1577, 1473, 1502, 1470, 1663, 1455, 1363, 1413, 1927, 826, 1273, 1333, 1310, 470, 1144, 208, 1232, 430, 1198, 533, 984, 476, 1048, 510, 1575, 564, 1714, 652, 1500, 1330, 1644, 1279, 1641, 1242, 1885, 803, 970, 829, 1246, 1353, 1492, 1302, 1532, 1250, 1393, 1361, 1543, 1042, 1417, 1034, 1532, 1071, 1510, 968, 1478, 1250, 1754, 1011, 1636, 1068, 1556, 1088, 1580, 988, 1551, 1461, 869, 1276, 2077, 1367, 2040, 1401, 1735, 1461, 1591, 1396, 1583, 1393, 1564, 1455, 1524, 1464, 1446, 1584, 1321, 1595, 1256, 1512, 1021, 1336, 805, 1407, 615, 627, 1117, 641, 1965, 459, 1914, 510, 1559, 601, 1604, 664, 1347, 567, 1347, 592, 1270]}";
    QTimer* timer = new QTimer(this);
    timer->setInterval(2000); // 设置间隔为 1 秒（根据需要进行调整）
    connect(timer, &QTimer::timeout, this, [=]() {
        if (!result.isEmpty()) {
            QString jsonString = result.mid(result.indexOf("{")); // 提取JSON字符串部分

            QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject jsonObject = jsonDocument.object();
            QJsonValue jsonValue = jsonObject.value("point");
            QJsonArray jsonArray = jsonValue.toArray();

            // 清除先前的点
            measurementTool1->clearPoints();

            // 在场景中绘制点
            for (int i = 0; i < jsonArray.size(); i += 2) {
                qreal x = jsonArray.at(i).toDouble();
                qreal y = jsonArray.at(i + 1).toDouble();
                QPointF point(x, y);

                QGraphicsEllipseItem* ellipseItem = measurementTool1->scene->addEllipse(-2, -2, 16, 16);
                ellipseItem->setPos(point);
                ellipseItem->setBrush(Qt::red);
                measurementTool1->points.append(ellipseItem);
            }

            // 调整视图以适应点的位置
            measurementTool1->fitInView(measurementTool1->scene->itemsBoundingRect(), Qt::KeepAspectRatio);
        }

        timer->stop();
        delete timer;
        });

    timer->start();
}

void SideWidget::onUnitButtonClicked()
{
    // 融合照片
    QString filePath1 = QFileDialog::getOpenFileName(nullptr, "Open Image", "", "Image Files (*.png *.jpg *.bmp)");

    if (!filePath1.isEmpty()) {
        std::string filePathUtf8 = filePath1.toUtf8().constData();
        Message::runPythonFunction(8, 0, filePath.toUtf8().constData(), filePathUtf8);

    }
    // 在这里执行相关操作，使用selectedRadioButton进行判断或调用其他函数
    QTimer* timer = new QTimer(this);
    timer->setInterval(1000); // 设置间隔为 1 秒（根据需要进行调整）
    connect(timer, &QTimer::timeout, this, [=]() {
        QDir directory("D:/Data/");
        QStringList files = directory.entryList(QStringList() << "return.jpg", QDir::Files);
        if (files.size() > 0) {
            QString imagePath = directory.absoluteFilePath(files.at(0));
            //QString imagePath = "D:/CBCT/return.JPG";
            QPixmap image(imagePath);
            QGraphicsPixmapItem* pixmapItem = new QGraphicsPixmapItem(image);
            measurementTool1->originalPixmap = image;
            measurementTool1->scene->clear();
            measurementTool1->scene->addItem(pixmapItem);
            //measurementTool1->fitImageInView();
            // 计算图片在窗口中心的位置
            QRectF sceneRect = measurementTool1->scene->itemsBoundingRect();
            measurementTool1->scene->setSceneRect(sceneRect);
            measurementTool1->fitInView(sceneRect, Qt::KeepAspectRatio);
            timer->stop();
            delete timer;
        }
        });
    timer->start();
}void SideWidget::saveCurrentFrame()
{
    if (measurementTool1->scene->items().isEmpty())
        return;

    QString savePath = QFileDialog::getSaveFileName(this, tr("保存当前帧"), ".", tr("Images (*.jpg)"));
    if (savePath.isEmpty())
        return;

    QPixmap pixmap(measurementTool1->scene->sceneRect().size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    measurementTool1->scene->render(&painter);

    pixmap.save(savePath, "JPG");
}


void SideWidget::clearDistance()
{
    measurementTool1->clearMeasurements();
}

void SideWidget::clearRects()
{
    measurementTool1->clearRects();
}