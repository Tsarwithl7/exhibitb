#ifndef SIDEWIDGET_H_H
#define SIDEWIDGET_H_H

#include "message.h"
#include "SideWidget.h"

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include<Qstring>
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
#include <QMessageBox>
#include <Qtimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>
#include <json/json.h>

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
#include <QMessageBox>
#include <Qtimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>
class MeasurementTool : public QGraphicsView
{
    Q_OBJECT

public:
    MeasurementTool(QLabel* distanceLabel, QSlider* contrastSlider, QSlider* brightnessSlider, QWidget* parent = nullptr);

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
    QVector<QGraphicsEllipseItem*> points;
    void clearPoints();


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
    qreal trans;
    QList<QGraphicsRectItem*> rects;
    QGraphicsPixmapItem* pixmapItem;

};

class SideWidget : public QMainWindow
{
    Q_OBJECT

public:
    SideWidget(QWidget* parent = nullptr);
    static const std::string& getResult(); // 声明获取结果的静态函数
    void open(const char* path);
private slots:
    void openImage();
    void clearDistance();
    void clearRects();
    void onUnitButtonClicked();
    void onkeyPointsButtonClicked();
    void saveCurrentFrame();
private:
    std::vector<int> pointCoordinates;
    QString result;

public:
    QSplitter* splitter;
    QDockWidget* dockWidget;
    QWidget* dockWidgetContent;
    QVBoxLayout* dockWidgetLayout;
    QLabel* contrastLabel;
    QSlider* contrastSlider;
    QLabel* brightnessLabel;
    QSlider* brightnessSlider;
    QLabel* distanceLabel;
    QPushButton* openButtonUpper;
    QPushButton* keyPointsButton;
    QPushButton* fusionButton;
    QPushButton* saveButton;
    QWidget* rightWidget;
    QVBoxLayout* rightLayout;
    MeasurementTool* measurementTool1;
    QPushButton* clearButton;
    QFrame* separatorLine1;
    QFrame* separatorLine2;
    QLabel* modeLabel;
    QRadioButton* modeButton1;
    QRadioButton* modeButton2;
    QRadioButton* modeButton3;
    QRadioButton* modeButton4;
    QGridLayout* modeButtonLayout;
    QButtonGroup* modeButtonGroup;
    QString filePath;
    QPushButton* clearRectButton;
};



#endif // SIDEWIDGET_H