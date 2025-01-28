#pragma once
#ifndef PDFEXPORT_H
#define PDFEXPORT_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDockWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QFont>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QMessageBox>
#include <QPrintDialog>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <random>
#include <vtkSmartPointer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkImageData.h>


class PDFExport : public QMainWindow
{
    Q_OBJECT

public:
    PDFExport(QWidget* parent = nullptr);
    ~PDFExport();
    void open(const char* quDuanPath, const char* touCePath);

private:
    void exportToPDF(QDockWidget* dockWidget, const QString& filePath);
    void previewPDF(QWidget* widget);
private:
    QPixmap pixmap;
    // 在类的私有成员变量中添加以下声明
private:
    QDockWidget* rightDockWidget;
    QStackedWidget* stackedWidget;
    QTableWidget* tableWidget;
    QTableWidget* tableWidget2;
    QWidget* page1Widget;
    QWidget* page2Widget;

private slots:
    void onExportClicked();
    void onPrintClicked();
    void onOpenImage1Clicked();
    //void onOpenImage2Clicked();
    void onPageSwitchClicked();
private:
    QLabel* imageLabel1;
    QLabel* imageLabel2;
    QString quDuan;
    QString touCe;
};

#endif // PDFEXPORT_H
