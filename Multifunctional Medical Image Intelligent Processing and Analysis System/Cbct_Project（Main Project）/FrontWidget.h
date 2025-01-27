
#ifndef FRONTWIDGET_H
#define FRONTWIDGET_H

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <QMainWindow>
#include <QSplitter>
#include <QDockWidget>
#include <QLabel>
#include <QSlider>
#include <QVBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPointF>
#include <QPixmap>
#include <QGraphicsLineItem>
#include <QGraphicsOpacityEffect>
#include <QFileDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <string>
#include <QTimer>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <stdlib.h>
#include <json/json.h>
#include "json/config.h"
#include "json/json_features.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
class Message_str {
public:
    static void runPythonFunction(int param1, const std::string& param2Json, const std::string& param3, const std::string& param4);
};

class MeasurementTool1 : public QGraphicsView
{
    Q_OBJECT

public:
    MeasurementTool1(QLabel* distanceLabel, QSlider* contrastSlider, QSlider* brightnessSlider, QButtonGroup* modeButtonGroup, QWidget* parent = nullptr);
    QList<QPointF> interactivePoints;
public slots:
    void updateContrast(int value);
    void updateBrightness(int value);
    void mouseReleaseEvent(QMouseEvent* event);
    void clearMeasurements();
    void setButtonValue(QString value);
    void clearRects();

public:
    void resizeEvent(QResizeEvent* event) override;
    //void mouseReleaseEvent(QMouseEvent* event) override;
    void fitImageInView();

public:
    QGraphicsScene* scene;
    QPointF startPoint;
    QGraphicsLineItem* currentLineItem;
    QLabel* distanceLabel;
    QSlider* contrastSlider;
    QSlider* brightnessSlider;
    QPixmap originalPixmap;
    qreal totalDistance;
    QList<QGraphicsLineItem*> lines;
    QString mode;
    QList<QGraphicsRectItem*> rects;
    QGraphicsPixmapItem* pixmapItem;
    QString currentFilePath;
    QPointF topLeft;
    QPointF bottomRight;
};

class FrontWidget : public QMainWindow
{
    Q_OBJECT

public:
    FrontWidget(QWidget* parent = nullptr); 
    void open(const char* path);

private slots:
    void openImage();
    void clearDistance();
    void clearRects();
    void onSegmentationButtonClicked();

    //void onSegmentationButtonClicked();
public:
    QSplitter* splitter;
    QDockWidget* dockWidget;
    QWidget* dockWidgetContent;
    QVBoxLayout* dockWidgetLayout;
    QLabel* contrastLabel;
    QLabel* contrastLabelleft;
    QLabel* contrastLabelright;
    QLabel* brightnessLabelleft;
    QLabel* brightnessLabelright;
    QSlider* contrastSlider;
    QLabel* brightnessLabel;
    QSlider* brightnessSlider;
    QLabel* distanceLabel;
    QPushButton* openButtonUpper;
    QPushButton* segmentationButton;
    QWidget* rightWidget;
    QVBoxLayout* rightLayout;
    MeasurementTool1* measurementTool1;
    QLabel* segmentationLabel;
    QRadioButton* radioButton1;
    QRadioButton* radioButton2;
    QRadioButton* radioButton3;
    QRadioButton* radioButton4;
    QRadioButton* radioButton5;
    QButtonGroup* buttonGroup;
    QGridLayout* buttonGridLayout;
    QGridLayout* labelGridLayout;
    QGridLayout* labelGridLayout2;
    QPushButton* clearButton;
    QFrame* separatorLine1;
    QFrame* separatorLine2;
    QLabel* modeLabel;
    QLabel* contrastRangeLabel;
    QLabel* brightnessRangeLabel;
    QRadioButton* modeButton1;
    QRadioButton* modeButton2;
    QRadioButton* modeButton3;
    QGridLayout* modeButtonLayout;
    QButtonGroup* modeButtonGroup;
    QPushButton* clearRectButton;
    QRadioButton* selectedRadioButton;
    int  SelectNumber, leftx, lefty, rightx, righty;
    QString filePath;
    QString paramJsonString = " ";
    std::string paramJsonUtf8 = " ";


};

#endif // FRONTWIDGET_H
