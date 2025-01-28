#pragma once

#include "message.h"

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <QtWidgets>
#include <QtOpenGL>
#include <QVTKOpenGLNativeWidget.h>

#include "vtkAutoInit.h" 
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkNamedColors.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkOpenGLRenderer.h>
#include "vtkInteractorStyleImage.h"
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkImageData.h>
#include <vtkStructuredPoints.h>
#include <vtkStructuredPointsReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkAxesActor.h>
#include <vtkImageShiftScale.h>
#include <vtkImageCast.h>
#include <vtkNIFTIImageReader.h>
#include <vtkDICOMImageReader.h>
#include <vtkMetaImageReader.h>
#include <vtkImageViewer2.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToColors.h>
#include "vtkMatrix4x4.h"
#include "vtkLookupTable.h"
#include "vtkImageActor.h"
#include "vtkImageMapper3D.h"
#include "vtkCamera.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkCornerAnnotation.h"
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkImageThreshold.h>
#include <vtkMarchingCubes.h>
#include <vtkBoxRepresentation.h>
#include <vtkBoxWidget2.h>
#include <vtkObjectFactory.h>
#include <vtkNew.h>
#include <vtkPlanes.h>
#include "vtkTransform.h"
#include <vtkContourFilter.h>
#include <vtkPointData.h>
#include <vtkPolyDataNormals.h>
#include <vtkStripper.h>
#include <vtkOutputWindow.h>
#include <vtkCellPicker.h>
#include <vtkRendererCollection.h>
#include "MyVtkInteractorStyleImage.h"

#ifndef RenderWidget_h
#define RenderWidget_h

class RenderWidget : public QWidget
{
	Q_OBJECT

public:
	RenderWidget(QWidget* parent);
	~RenderWidget();
	void readDcm(const char* p);
	void readNii(const char* p);

protected:
	void init();
	void refresh();
	void chooseTeethToShow();

private slots:// 槽函数
	void slider0Changed();
	void slider1Changed();
	void slider2Changed();

	void changeLevel();
	void changeWindow();
	void ctButton0changed();
	void ctButton1changed();
	void ctButton2changed();
	//void ctButton5changed();

	void InitRendering();
	void ResultRendering();
	void activateBoxWidget();
	void updateBlendMode();
	void onCheckboxStateChanged();
	void initBoxWidget();
	void onComboBoxActivated(int index);
	void onRadioButtonEntered();
	void onRadioButtonLeft();

private:
	const char* path = nullptr;

	QGridLayout* mainLayout = new QGridLayout;

	QDockWidget* dockwidget;

	QGridLayout* optionLayout = new QGridLayout;
	QLineEdit* windowEdit;
	QLineEdit* levelEdit;
	QPushButton* ctButton[3];
	QRadioButton* compositeRadioButton;
	QRadioButton* maximumIntensityRadioButton;
	QRadioButton* minimumIntensityRadioButton;
	QRadioButton* addictiveRadioButton;
	QRadioButton* vtkGPUVolumeRayCastRadioButton;
	QLineEdit* lineEdit;
	QLabel* ROI;
	QCheckBox* setBoxV;
	QCheckBox* before_checkbox;
	QCheckBox* teeth_checkbox;
	QCheckBox* alveolar_bone_checkbox;

	QGridLayout* renderLayout = new QGridLayout;
	QVTKOpenGLNativeWidget* imgWidgetLeftUpper = new QVTKOpenGLNativeWidget();
	QVTKOpenGLNativeWidget* imgWidgetLeftLower = new QVTKOpenGLNativeWidget();
	QVTKOpenGLNativeWidget* imgWidgetRightLower = new QVTKOpenGLNativeWidget();
	QVTKOpenGLNativeWidget* glWidget = new QVTKOpenGLNativeWidget();
	QVBoxLayout* vBoxLayout[3];//竖直的
	QHBoxLayout* hBoxLayout[3];//水平的
	QLabel* xyzLabel[3];
	QSlider* slider[3];
	QLabel* sliceLabel[3];

	vtkSmartPointer<vtkDICOMImageReader> dcmReader = nullptr;//DICOM文件读取器
	vtkSmartPointer<vtkNIFTIImageReader> niiReader = nullptr;//NIFTI文件读取器

	vtkSmartPointer<vtkImageViewer2> viewer[3];//三个二维视图
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> openglrw[3];
	vtkSmartPointer<MyVtkInteractorStyleImage> imagestyle[3];
	vtkSmartPointer<vtkTextActor> textActor[3];
	vtkSmartPointer<vtkCornerAnnotation> cornerAnnotation[3];
	vtkCamera* cam;

	vtkSmartPointer<vtkNIFTIImageReader> teeth_reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	vtkSmartPointer<vtkNIFTIImageReader> alveolar_bone_reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	vtkSmartPointer<vtkPiecewiseFunction> teethArray[41];
	vtkSmartPointer<vtkPiecewiseFunction> teethPiecewise;
	vtkSmartPointer<vtkVolumeProperty>  teeth_volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();

	vtkSmartPointer<vtkVolume> teeth_volume = vtkSmartPointer<vtkVolume>::New();
	vtkSmartPointer<vtkVolume> alveolar_bone_volume = vtkSmartPointer<vtkVolume>::New();


	vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> volumeMapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
	vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapper1 = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
	vtkSmartPointer<vtkRenderer> ren= vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkBoxWidget2> m_boxWidget;

	int extent[6];//序列范围
	double spacing[3];//图像切片间距
	double origin[3];//图像原点
	double center[3];//物体中心点
	double current[3];//当前的切片位置。用 Slice * spacing - center （伪代码）计算。
};

#endif