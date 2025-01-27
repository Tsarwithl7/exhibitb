/*
 *
 * 程序的可视化主模块
 * 作者：徐震洋（负责vtk库的调用、二三维的基本显示、三维操作部分、多模态交互部分）
 * 吴国正（负责二维三视图的布局、层数显示、窗位窗宽调节、ui的布局优化、主程序的结构优化）
 *
 */
#include <iostream>
#include <sstream>
#include <iomanip>
#include "RenderWidget.h"

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2)

//自定义输出窗口对象，以禁止vtk显示outputwindow
class CustomOutputWindow : public vtkOutputWindow
{
public:
    static CustomOutputWindow* New();
    vtkTypeMacro(CustomOutputWindow, vtkOutputWindow);

    void DisplayText(const char* text) override
    {
        // 不执行任何操作，即禁止显示输出窗口的文本
    }
};
vtkStandardNewMacro(CustomOutputWindow);

 //自定义包围盒响应事件
class vtkBoxWidget3DCallback : public vtkCommand
{
public:
    static vtkBoxWidget3DCallback* New()
    {
        return new vtkBoxWidget3DCallback;
    }
    vtkTypeMacro(vtkBoxWidget3DCallback, vtkCommand);
    vtkBoxWidget3DCallback() : m_transform(vtkSmartPointer<vtkTransform>::New()) {}
    ~vtkBoxWidget3DCallback() = default;

    void setVolume(vtkVolume* t_volume) { m_volume = t_volume; }
    void Execute(vtkObject* caller, unsigned long, void*)
    {
        auto* const boxWidget =
            vtkBoxWidget2::SafeDownCast(caller);
        auto* const  boxRepresentation =
            vtkBoxRepresentation::SafeDownCast(boxWidget->GetRepresentation());
        boxRepresentation->SetInsideOut(1);
        boxRepresentation->GetPlanes(m_planes);
        m_volume->GetMapper()->SetClippingPlanes(m_planes);
    }
private:
    vtkVolume* m_volume = {};
    vtkNew<vtkPlanes> m_planes;
    vtkSmartPointer<vtkTransform> m_transform = {};
};

RenderWidget::RenderWidget(QWidget* parent)
	: QWidget(parent)
{
    /*
    // 创建自定义的输出窗口对象
    CustomOutputWindow* customOutputWindow = CustomOutputWindow::New();
    // 替换默认的输出窗口
    vtkOutputWindow::SetInstance(customOutputWindow);
    */

    vtkOutputWindow::SetGlobalWarningDisplay(0);

	init();
}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::init()
{
    dcmReader = vtkDICOMImageReader::New();
    niiReader = vtkNIFTIImageReader::New();

    for (int i = 0; i < 3; i++)
    {
        openglrw[i] = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
        imagestyle[i] = vtkSmartPointer<MyVtkInteractorStyleImage>::New();
        cornerAnnotation[i] = vtkSmartPointer<vtkCornerAnnotation>::New();
        textActor[i] = vtkSmartPointer<vtkTextActor>::New();
    }

    //主layout
    mainLayout->addLayout(optionLayout, 0, 0, 1, 1);
    mainLayout->addLayout(renderLayout, 0, 1, 1, 1);
    mainLayout->setColumnMinimumWidth(0, 400);
    mainLayout->setColumnMinimumWidth(1, 400);
    mainLayout->setColumnStretch(0, 4);
    mainLayout->setColumnStretch(1, 7);
    mainLayout->setSpacing(4);

    this->setLayout(mainLayout);



    dockwidget = new QDockWidget(this);
    dockwidget->setObjectName("牙齿表格");
    dockwidget->setFloating(true);
    dockwidget->setWindowTitle("牙齿表格");
    //dockwidget->setTitleBarWidget(new QWidget(NULL));
    dockwidget->setAllowedAreas(Qt::NoDockWidgetArea);
    dockwidget->setMinimumSize(250, 400);
    dockwidget->setWindowFlags(dockwidget->windowFlags() | Qt::WindowStaysOnTopHint);
    dockwidget->setVisible(false);



    //左侧按钮相关的layout
    optionLayout->setAlignment(Qt::AlignTop);
    optionLayout->setColumnStretch(0, 1);
    optionLayout->setColumnStretch(1, 2);
    optionLayout->setColumnStretch(2, 2);
    optionLayout->setColumnStretch(3, 1);
    for (int i = 0; i < 23; i++)
    {
        optionLayout->setRowStretch(i, 1);
    }

    optionLayout->setSpacing(4);


    //空白占位符
    QLabel* spacer = new QLabel();
    spacer->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    spacer->setMinimumHeight(15);
    QFrame* line1 = new QFrame;
    line1->setFrameShape(QFrame::HLine);
    line1->setFrameShadow(QFrame::Sunken);
    line1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFrame* line2 = new QFrame;
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    line2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QFrame* line3 = new QFrame;
    line3->setFrameShape(QFrame::HLine);
    line3->setFrameShadow(QFrame::Sunken);
    line3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QFont buttonFont;
    buttonFont.setPointSize(12);
    QFont font;
    font.setPointSize(14);
    QFont titleFont;
    titleFont.setPointSize(18);

    QLabel* label2d = new QLabel(tr("二维调节："), this);
    label2d->setFont(titleFont);
    optionLayout->addWidget(label2d, 0, 0, 1, 4);

    QLabel* windowLabel = new QLabel(tr("    窗宽："), this);
    windowLabel->setFont(font);
    windowEdit = new QLineEdit();
    windowEdit->setFont(font);
    //windowEdit->setMaximumWidth(windowEdit->fontMetrics().horizontalAdvance("AAAA"));
    windowEdit->setText("2000");
    windowEdit->setValidator(new QIntValidator(windowEdit));
    connect(windowEdit, &QLineEdit::textChanged, this, &RenderWidget::changeWindow);
    optionLayout->addWidget(windowLabel, 1, 0, 1, 1);
    optionLayout->addWidget(windowEdit, 1, 1, 1, 1);

    QLabel* levelLabel = new QLabel(tr("    窗位："), this);
    levelLabel->setStyleSheet("text-align: right;");
    levelLabel->setFont(font);
    levelEdit = new QLineEdit();
    levelEdit->setFont(font);
    //levelEdit->setMaximumWidth(levelEdit->fontMetrics().horizontalAdvance("AAAA"));
    levelEdit->setText("500");
    levelEdit->setValidator(new QIntValidator(levelEdit));
    connect(levelEdit, &QLineEdit::textChanged, this, &RenderWidget::changeLevel);
    optionLayout->addWidget(levelLabel, 2, 0, 1, 1);
    optionLayout->addWidget(levelEdit, 2, 1, 1, 1);

    QLabel* setLabel = new QLabel(tr("    预设："), this);
    setLabel->setFont(font);
    ctButton[0] = new QPushButton(tr("CT-骨骼"), this);
    //ctButton[0]->setFixedSize(100, 35);
    ctButton[0]->setToolTip(tr("高亮显示骨骼"));
    ctButton[0]->setFont(buttonFont);
    ctButton[0]->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 1px solid #000000;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    ctButton[0]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(ctButton[0], &QPushButton::clicked, this, &RenderWidget::ctButton0changed);
    ctButton[1] = new QPushButton(tr("CT-空气"), this);
    ctButton[1]->setToolTip(tr("高亮显示空气"));
    ctButton[1]->setFont(buttonFont);
    ctButton[1]->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 1px solid #000000;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    connect(ctButton[1], &QPushButton::clicked, this, &RenderWidget::ctButton1changed);
    ctButton[2] = new QPushButton(tr("CT-大脑"), this);
    ctButton[2]->setToolTip(tr("高亮显示大脑"));
    ctButton[2]->setFont(buttonFont);
    ctButton[2]->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 1px solid #000000;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    connect(ctButton[2], &QPushButton::clicked, this, &RenderWidget::ctButton2changed);
    optionLayout->addWidget(setLabel, 3, 0, 1, 1);
    optionLayout->addWidget(ctButton[0], 3, 1, 1, 1);
    optionLayout->addWidget(ctButton[1], 3, 2, 1, 1);
    optionLayout->addWidget(ctButton[2], 4, 1, 1, 1);

    optionLayout->addWidget(line1, 5, 0, 1, 4);

    QLabel* label3d = new QLabel(tr("三维渲染："), this);
    label3d->setFont(titleFont);
    optionLayout->addWidget(label3d, 6, 0, 1, 4);

    QLabel* chooseMode = new QLabel(tr("    选择体绘制算法："), this);
    chooseMode->setFont(font);

    QPushButton* buttonRendering = new QPushButton(tr("启动渲染"), this);
    buttonRendering->setFont(buttonFont);
    buttonRendering->setToolTip(tr(""));
    buttonRendering->setObjectName("customButton");
    buttonRendering->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 1px solid #000000;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    connect(buttonRendering, SIGNAL(clicked()), this, SLOT(InitRendering()));

    //固定点光线投影算法
    QLabel* vtkFixedPointVolumeRayCast = new QLabel(tr("固定点光线投影算法"), this);
    vtkFixedPointVolumeRayCast->setFont(font);
    vtkFixedPointVolumeRayCast->setVisible(false);

    compositeRadioButton = new QRadioButton("复合混合渲染");
    compositeRadioButton->setFont(buttonFont);
    maximumIntensityRadioButton = new QRadioButton("最大强度投影");
    maximumIntensityRadioButton->setFont(buttonFont);
    minimumIntensityRadioButton = new QRadioButton("最小强度投影");
    minimumIntensityRadioButton->setFont(buttonFont);
    addictiveRadioButton = new QRadioButton("Addictive");
    addictiveRadioButton->setFont(buttonFont);

    connect(compositeRadioButton, SIGNAL(clicked()), this, SLOT(updateBlendMode()));
    connect(maximumIntensityRadioButton, SIGNAL(clicked()), this, SLOT(updateBlendMode()));
    connect(minimumIntensityRadioButton, SIGNAL(clicked()), this, SLOT(updateBlendMode()));
    connect(addictiveRadioButton, SIGNAL(clicked()), this, SLOT(updateBlendMode()));

    //基于GPU加速的光线投射体绘制算法
    vtkGPUVolumeRayCastRadioButton = new QRadioButton("基于GPU加速的光线投射渲染");
    vtkGPUVolumeRayCastRadioButton->setFont(buttonFont);
    connect(vtkGPUVolumeRayCastRadioButton, SIGNAL(clicked()), this, SLOT(updateBlendMode()));

    optionLayout->addWidget(chooseMode, 7, 0, 1, 2);
    //optionLayout->addWidget(vtkFixedPointVolumeRayCast);
    optionLayout->addWidget(compositeRadioButton, 8, 1, 1, 1);
    optionLayout->addWidget(maximumIntensityRadioButton, 8, 2, 1, 1);
    optionLayout->addWidget(minimumIntensityRadioButton, 9, 1, 1, 1);
    optionLayout->addWidget(vtkGPUVolumeRayCastRadioButton, 9, 2, 1, 2);
    optionLayout->addWidget(buttonRendering, 10, 1, 1, 1);



    QLabel* choosePresets = new QLabel(tr("    选择预设:"), this);
    choosePresets->setFont(font);

    QComboBox* comboBox = new QComboBox(this);
    comboBox->setFont(buttonFont);
    comboBox->addItem("默认");
    comboBox->addItem("皮肤");
    comboBox->addItem("骨骼");
    comboBox->addItem("CT-Bone");
    comboBox->addItem("CT-AAA");
    comboBox->addItem("CT-Cardiac3");
    comboBox->addItem("CT-MIP");
    comboBox->addItem("MR-Default");
    comboBox->setCurrentIndex(0);
    comboBox->setItemData(0, "<center>Text</center>", Qt::TextAlignmentRole);
    comboBox->setStyleSheet("QComboBox { border: 1px solid #000000; background-color: #FFFFFF; padding: 2px; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox::down-arrow { image: url(images/arrow.png); }"
        "QComboBox QAbstractItemView { border: none; background-color: #FFFFFF; }"
        "QComboBox QAbstractItemView::item { padding: 1px; }");
    connect(comboBox, SIGNAL(activated(int)), this, SLOT(onComboBoxActivated(int)));

    optionLayout->addWidget(choosePresets, 11, 0, 1, 2);
    optionLayout->addWidget(comboBox, 12, 1, 1, 1);



    //ROI操作
    ROI = new QLabel(tr("    ROI操作："), this);
    ROI->setFont(font);
    ROI->setVisible(false);

    QPushButton* buttonRoi = new QPushButton(tr("启动ROI"), this);
    buttonRoi->setFont(buttonFont);
    buttonRoi->setVisible(false);
    connect(buttonRoi, SIGNAL(clicked()), this, SLOT(initBoxWidget()));

    setBoxV = new QCheckBox("显示包围盒", this);
    setBoxV->setVisible(false);
    connect(setBoxV, SIGNAL(stateChanged(int)), this, SLOT(activateBoxWidget()));

    optionLayout->addWidget(ROI, 13, 0, 1, 2);
    //optionLayout->addWidget(buttonRoi, 14, 1, 1, 1);
    optionLayout->addWidget(setBoxV, 14, 1, 1, 1);



    optionLayout->addWidget(line2, 15, 0, 1, 4);

    QLabel* labelTeeth = new QLabel(tr("分割结果查看："), this);
    labelTeeth->setFont(titleFont);

    QLabel* labelTeeth2 = new QLabel(tr("    牙齿和牙槽骨渲染："), this);
    labelTeeth2->setFont(font);

    QPushButton* GetResultButton = new QPushButton(tr("启动"), this);
    GetResultButton->setFont(buttonFont);
    GetResultButton->setToolTip(tr(""));
    GetResultButton->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 1px solid #000000;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    connect(GetResultButton, SIGNAL(clicked()), this, SLOT(ResultRendering()));

    optionLayout->addWidget(labelTeeth, 16, 0, 1, 4);
    optionLayout->addWidget(labelTeeth2, 17, 0, 1, 2);
    optionLayout->addWidget(GetResultButton, 18, 1, 1, 1);



    //选择渲染目标
    QLabel* chooseResult = new QLabel(tr("    选择渲染目标："), this);
    chooseResult->setFont(font);
    before_checkbox = new QCheckBox("显示原图像", this);
    before_checkbox->setFont(buttonFont);
    teeth_checkbox = new QCheckBox("显示牙齿", this);
    teeth_checkbox->setFont(buttonFont);
    alveolar_bone_checkbox = new QCheckBox("显示牙槽骨", this);
    alveolar_bone_checkbox->setFont(buttonFont);
    connect(before_checkbox, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxStateChanged()));
    connect(teeth_checkbox, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxStateChanged()));
    connect(alveolar_bone_checkbox, SIGNAL(stateChanged(int)), this, SLOT(onCheckboxStateChanged()));


    lineEdit = new QLineEdit(tr("指针位置"), this);
    lineEdit->setFont(font);
    connect(lineEdit, &QLineEdit::textChanged, this, &RenderWidget::chooseTeethToShow);

    teethPiecewise = vtkSmartPointer<vtkPiecewiseFunction>::New();

    optionLayout->addWidget(chooseResult, 19, 0, 1, 2);
    optionLayout->addWidget(before_checkbox, 20, 1, 1, 1);
    optionLayout->addWidget(teeth_checkbox, 20, 2, 1, 1);
    optionLayout->addWidget(alveolar_bone_checkbox, 21, 1, 1, 1);
    optionLayout->addWidget(lineEdit, 22, 1, 1, 1);

    //渲染的layout
    //滑块、曲断信息相关
    for (int i = 0; i < 3; i++)
    {
        vBoxLayout[i] = new QVBoxLayout;
        hBoxLayout[i] = new QHBoxLayout;
        xyzLabel[i] = new QLabel;
        slider[i] = new QSlider(Qt::Horizontal);
        sliceLabel[i] = new QLabel;
    }

    center[0] = center[1] = center[2] = 1.0;
    extent[0] = extent[2] = extent[4] = 0;
    extent[1] = extent[3] = extent[5] = 2;
    spacing[0] = spacing[1] = spacing[2] = 1.0;

    xyzLabel[0]->setText(" 轴状面 ");  // 设置文本内容
    xyzLabel[0]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    xyzLabel[0]->setStyleSheet("font-size: 14px; background-color: rgba(255, 0, 0, 127);");  // 设置样式
    xyzLabel[0]->setMaximumHeight(16);
    hBoxLayout[0]->addWidget(xyzLabel[0]);
    slider[0]->setRange(0, 2);
    slider[0]->setValue(1);
    slider[0]->setStyleSheet("background-color: rgba(255, 0, 0, 191);");
    slider[0]->setMaximumHeight(16);
    hBoxLayout[0]->addWidget(slider[0]);
    sliceLabel[0]->setText(" S: 0.0000mm ");  // 设置文本内容
    sliceLabel[0]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    sliceLabel[0]->setStyleSheet("font-size: 14px; background-color: rgba(255, 0, 0, 127);");  // 设置样式
    sliceLabel[0]->setMaximumHeight(16);
    hBoxLayout[0]->addWidget(sliceLabel[0]);
    hBoxLayout[0]->setSpacing(0);
    vBoxLayout[0]->addLayout(hBoxLayout[0]);
    vBoxLayout[0]->addWidget(imgWidgetLeftUpper);
    vBoxLayout[0]->setStretch(0, 1);
    vBoxLayout[0]->setStretch(1, 16);
    vBoxLayout[0]->setSpacing(0);

    xyzLabel[1]->setText(" 冠状面 ");  // 设置文本内容
    xyzLabel[1]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    xyzLabel[1]->setStyleSheet("font-size: 14px; background-color: rgba(0, 255, 0, 127);");  // 设置样式
    xyzLabel[1]->setMaximumHeight(16);
    hBoxLayout[1]->addWidget(xyzLabel[1]);
    slider[1]->setRange(0, 2);
    slider[1]->setValue(1);
    slider[1]->setStyleSheet("background-color: rgba(0, 255, 0, 191);");
    slider[1]->setMaximumHeight(16);
    hBoxLayout[1]->addWidget(slider[1]);
    sliceLabel[1]->setText(" A: 0.0000mm ");  // 设置文本内容
    sliceLabel[1]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    sliceLabel[1]->setStyleSheet("font-size: 14px; background-color: rgba(0, 255, 0, 127);");  // 设置样式
    sliceLabel[1]->setMaximumHeight(16);
    hBoxLayout[1]->addWidget(sliceLabel[1]);
    hBoxLayout[1]->setSpacing(0);
    vBoxLayout[1]->addLayout(hBoxLayout[1]);
    vBoxLayout[1]->addWidget(imgWidgetLeftLower);
    vBoxLayout[1]->setStretch(0, 1);
    vBoxLayout[1]->setStretch(1, 16);
    vBoxLayout[1]->setSpacing(0);

    xyzLabel[2]->setText(" 矢状面 ");  // 设置文本内容
    xyzLabel[2]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    xyzLabel[2]->setStyleSheet("font-size: 14px; background-color: rgba(255, 255, 0, 127);");  // 设置样式
    xyzLabel[2]->setMaximumHeight(16);
    hBoxLayout[2]->addWidget(xyzLabel[2]);
    slider[2]->setRange(0, 2);
    slider[2]->setValue(1);
    slider[2]->setStyleSheet("background-color: rgba(255, 255, 0, 191);");
    slider[2]->setMaximumHeight(16);
    hBoxLayout[2]->addWidget(slider[2]);
    sliceLabel[2]->setText(" L: 0.0000mm ");  // 设置文本内容
    sliceLabel[2]->setAlignment(Qt::AlignCenter);  // 设置文本对齐方式
    sliceLabel[2]->setStyleSheet("font-size: 14px; background-color: rgba(255, 255, 0, 127);");  // 设置样式
    sliceLabel[2]->setMaximumHeight(16);
    hBoxLayout[2]->addWidget(sliceLabel[2]);
    hBoxLayout[2]->setSpacing(0);
    vBoxLayout[2]->addLayout(hBoxLayout[2]);
    vBoxLayout[2]->addWidget(imgWidgetRightLower);
    vBoxLayout[2]->setStretch(0, 1);
    vBoxLayout[2]->setStretch(1, 16);
    vBoxLayout[2]->setSpacing(0);


    //槽函数绑定
    //建立连接
    connect(slider[0], &QSlider::valueChanged, this, &RenderWidget::slider0Changed);
    connect(slider[1], &QSlider::valueChanged, this, &RenderWidget::slider1Changed);
    connect(slider[2], &QSlider::valueChanged, this, &RenderWidget::slider2Changed);

    //主要的四个显示窗口
    renderLayout->addLayout(vBoxLayout[0], 0, 0, 1, 1);
    renderLayout->addLayout(vBoxLayout[1], 1, 0, 1, 1);
    renderLayout->addLayout(vBoxLayout[2], 1, 1, 1, 1);
    renderLayout->addWidget(glWidget, 0, 1, 1, 1);
    renderLayout->setRowMinimumHeight(0, 256);
    renderLayout->setRowMinimumHeight(1, 256);
    renderLayout->setColumnMinimumWidth(0, 256);
    renderLayout->setColumnMinimumWidth(1, 256);
    renderLayout->setRowStretch(0, 1);
    renderLayout->setRowStretch(1, 1);
    renderLayout->setColumnStretch(0, 1);
    renderLayout->setColumnStretch(1, 1);
    renderLayout->setSpacing(1);

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(178, 183, 223));
    glWidget->setAutoFillBackground(true);
    glWidget->setPalette(palette);

    //SetBackground(0.70, 0.72, 0.88);

    //vtkGPUVolumeRayCastRadioButton->setChecked(true);
    //updateBlendMode();
    /*
    ren->SetBackground(0.70, 0.72, 0.88);
    renderWindow->AddRenderer(ren);
    glWidget->setRenderWindow(renderWindow);*/
}

void RenderWidget::readDcm(const char* path)
{
    dcmReader->SetDirectoryName(path);//设置文件夹路径
    dcmReader->Update();//更新
    dcmReader->GetOutput()->GetExtent(this->extent);//获取图像序列范围
    dcmReader->GetOutput()->GetSpacing(this->spacing);//获取数据间距
    dcmReader->GetOutput()->GetOrigin(this->origin);//获取原点

    volumeMapper->SetInputData(dcmReader->GetOutput());
    volumeMapper1->SetInputData(dcmReader->GetOutput());

    for (int i = 0; i < 3; i++)
    {
        if (!viewer[i]) {
            viewer[i] = vtkSmartPointer<vtkImageViewer2>::New();
            viewer[i]->GetRenderWindow()->ShowWindowOff();
        }
    }

    //获取中心点及设置滚动条
    int j = 0;
    slider[2]->setRange(extent[0], extent[1]);
    j = (extent[0] + extent[1]) / 2;
    slider[2]->setValue(j);
    center[0] = origin[0] + spacing[0] * j;

    slider[1]->setRange(extent[2], extent[3]);
    j = (extent[2] + extent[3]) / 2;
    slider[1]->setValue(j);
    center[1] = origin[1] + spacing[1] * j;

    slider[0]->setRange(extent[4], extent[5]);
    j = (extent[4] + extent[5]) / 2;
    slider[0]->setValue(j);
    center[2] = origin[2] + spacing[2] * j;

    for (int i = 0; i < 3; i++) {
        viewer[i]->SetRenderWindow(openglrw[i]);
        openglrw[i]->ShowWindowOff();
        viewer[i]->SetInputConnection(dcmReader->GetOutputPort());
        switch (i) {
        case 0:
            viewer[i]->SetSliceOrientationToXY();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(0, 0, 1);
            cam->SetViewUp(0, 1, 0);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        case 1:
            viewer[i]->SetSliceOrientationToXZ();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(0, 1, 0);
            cam->SetViewUp(0, 0, -1);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        case 2:
            viewer[i]->SetSliceOrientationToYZ();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(1, 0, 0);
            cam->SetViewUp(0, 0, -1);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        }

        viewer[i]->SetSlice(1 + (viewer[i]->GetSliceMax() + viewer[i]->GetSliceMin()) / 2);

        // 获取像素值范围信息
        //double range[2];
        //niiReader->GetOutput()->GetScalarRange(range);
        //viewer[i]->SetColorLevel((range[0] + range[1]) / 2);
        //viewer[i]->SetColorWindow(range[1] - range[0]);
        viewer[i]->SetColorLevel(500);
        viewer[i]->SetColorWindow(2000);

        viewer[i]->GetRenderer()->SetBackground(0, 0, 0);

    }

    imgWidgetLeftUpper->setRenderWindow(openglrw[0]);
    imgWidgetLeftLower->setRenderWindow(openglrw[1]);
    imgWidgetRightLower->setRenderWindow(openglrw[2]);

    for (int i = 0; i < 3; i++) {
        imagestyle[i]->SetImageViewer(viewer[i]);
        imagestyle[i]->SetRenderer(viewer[i]->GetRenderer());
        imagestyle[i]->SetLineEdit(lineEdit);
        viewer[i]->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imagestyle[i]);
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(4) << (slider[0]->value() * spacing[2] - center[2]);
    sliceLabel[0]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    stream.str("");
    stream << std::fixed << std::setprecision(4) << (slider[1]->value() * spacing[1] - center[1]);
    sliceLabel[1]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    stream.str("");
    stream << std::fixed << std::setprecision(4) << (slider[2]->value() * spacing[0] - center[0]);
    sliceLabel[2]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容

    for (int i = 0; i < 3; i++)
    {
        //cornerAnnotation[i]->GetTextProperty()->SetFontSize(14);
        cornerAnnotation[i]->GetTextProperty()->SetColor(1, 1, 1);
        //cornerAnnotation[i]->SetText(0, "<window>");
        //cornerAnnotation[i]->SetText(1, "<level>");
        viewer[i]->GetRenderer()->AddViewProp(cornerAnnotation[i]);
    }

    refresh();
}

void RenderWidget::readNii(const char* path)
{
    this->path = path;
    niiReader->SetFileName(QString::fromUtf8(path).toUtf8());//设置文件路径
    niiReader->Update();//更新
    niiReader->GetOutput()->GetExtent(this->extent);//获取图像序列范围
    niiReader->GetOutput()->GetSpacing(this->spacing);//获取数据间距
    niiReader->GetOutput()->GetOrigin(this->origin);//获取原点

    volumeMapper->SetInputData(niiReader->GetOutput());
    volumeMapper1->SetInputData(niiReader->GetOutput());

    for (int i = 0; i < 3; i++) {
        if (!viewer[i]) {
            viewer[i] = vtkSmartPointer<vtkImageViewer2>::New();
            viewer[i]->GetRenderWindow()->ShowWindowOff();
        }
    }

    //获取中心点及设置滚动条
    int i = 0;
    slider[2]->setRange(extent[0], extent[1]);
    i = (extent[0] + extent[1]) / 2;
    slider[2]->setValue(i);
    center[0] = origin[0] + spacing[0] * i;

    slider[1]->setRange(extent[2], extent[3]);
    i = (extent[2] + extent[3]) / 2;
    slider[1]->setValue(i);
    center[1] = origin[1] + spacing[1] * i;

    slider[0]->setRange(extent[4], extent[5]);
    i = (extent[4] + extent[5]) / 2;
    slider[0]->setValue(i);
    center[2] = origin[2] + spacing[2] * i;

    //double range[2];
    for (int i = 0; i < 3; i++) {
        viewer[i]->SetRenderWindow(openglrw[i]);
        openglrw[i]->ShowWindowOff();
        viewer[i]->SetInputConnection(niiReader->GetOutputPort());
        switch (i) {
        case 0:
            viewer[i]->SetSliceOrientationToXY();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(0, 0, -1);
            cam->SetViewUp(0, -1, 0);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        case 1:
            viewer[i]->SetSliceOrientationToXZ();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(0, -1, 0);
            cam->SetViewUp(0, 0, 1);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        case 2:
            viewer[i]->SetSliceOrientationToYZ();
            cam = viewer[i]->GetRenderer() ? viewer[i]->GetRenderer()->GetActiveCamera() : nullptr;
            cam->SetFocalPoint(0, 0, 0);
            cam->SetPosition(1, 0, 0);
            cam->SetViewUp(0, 0, 1);
            if (viewer[i]->GetRenderer() && viewer[i]->GetInput())
            {
                double scale = viewer[i]->GetRenderer()->GetActiveCamera()->GetParallelScale();
                viewer[i]->GetRenderer()->ResetCamera();
                viewer[i]->GetRenderer()->GetActiveCamera()->SetParallelScale(scale);
            }
            break;
        }

        viewer[i]->SetSlice(1+(viewer[i]->GetSliceMax() + viewer[i]->GetSliceMin()) / 2);

        // 获取像素值范围信息
        //double range[2];
        //niiReader->GetOutput()->GetScalarRange(range);
        //viewer[i]->SetColorLevel((range[0] + range[1]) / 2);
        //viewer[i]->SetColorWindow(range[1] - range[0]);
        viewer[i]->SetColorLevel(500);
        viewer[i]->SetColorWindow(2000);

        viewer[i]->GetRenderer()->SetBackground(0, 0, 0);
    }

    imgWidgetLeftUpper->setRenderWindow(openglrw[0]);
    imgWidgetLeftLower->setRenderWindow(openglrw[1]);
    imgWidgetRightLower->setRenderWindow(openglrw[2]);
    
    for (int i = 0; i < 3; i++) {
        imagestyle[i]->SetImageViewer(viewer[i]);
        imagestyle[i]->SetRenderer(viewer[i]->GetRenderer());
        imagestyle[i]->SetLineEdit(lineEdit);
        viewer[i]->GetRenderWindow()->GetInteractor()->SetInteractorStyle(imagestyle[i]);
    }

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2) << (slider[0]->value() * spacing[2] - center[2]);
    sliceLabel[0]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    stream.str("");
    stream << std::fixed << std::setprecision(2) << (slider[1]->value() * spacing[1] - center[1]);
    sliceLabel[1]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    stream.str("");
    stream << std::fixed << std::setprecision(2) << (slider[2]->value() * spacing[0] - center[0]);
    sliceLabel[2]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容

    for (int i = 0; i < 3; i++)
    {
        //cornerAnnotation[i]->GetTextProperty()->SetFontSize(14);
        cornerAnnotation[i]->GetTextProperty()->SetColor(1, 1, 1);
        //cornerAnnotation[i]->SetText(0, std::to_string(range[0]).c_str());
        //cornerAnnotation[i]->SetText(1, std::to_string(range[1]).c_str());
        viewer[i]->GetRenderer()->AddViewProp(cornerAnnotation[i]);
    }

    /*
    for (int i = 0; i < 3; i++)
    {
        textActor[i]->SetDisplayPosition(5, 5);
        textActor[i]->GetTextProperty()->SetFontSize(14);
        textActor[i]->GetTextProperty()->SetFontFamily(VTK_FONT_FILE);
        textActor[i]->GetTextProperty()->SetFontFile(QString("./fonts/simsun.ttc").toUtf8());
        textActor[i]->GetTextProperty()->SetColor(1, 1, 1);
    }
    textActor[0]->SetInput(QString::fromUtf8(u8"轴状面1").toUtf8());
    textActor[1]->SetInput(QString::fromUtf8(u8"冠状面2").toUtf8());
    textActor[2]->SetInput(QString::fromUtf8(u8"矢状面3").toUtf8());
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->GetRenderer()->AddActor2D(textActor[i]);
    }
    */

    refresh();

    vtkGPUVolumeRayCastRadioButton->setChecked(true);
    updateBlendMode();
    
    /*ren->SetBackground(0.70, 0.72, 0.88);
    renderWindow->AddRenderer(ren);
    glWidget->setRenderWindow(renderWindow);*/
}

void RenderWidget::refresh()//刷新
{
    for (int i = 0; i < 3; i++) {
        openglrw[i]->Render();
    }
}

void RenderWidget::changeLevel() {

    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorLevel(levelEdit->text().toDouble());
        openglrw[i]->Render();
    }

}

void RenderWidget::changeWindow() {
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorWindow(windowEdit->text().toDouble());
        openglrw[i]->Render();
    }
}

void RenderWidget::ctButton0changed() {
    windowEdit->setText("1000");
    levelEdit->setText("400");
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorWindow(1000);
        viewer[i]->SetColorLevel(400);
        openglrw[i]->Render();
    }
}
void RenderWidget::ctButton1changed() {
    windowEdit->setText("1000");
    levelEdit->setText("-426");
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorWindow(1000);
        viewer[i]->SetColorLevel(-426);
        openglrw[i]->Render();
    }
}
void RenderWidget::ctButton2changed() {
    windowEdit->setText("100");
    levelEdit->setText("50");
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorWindow(100);
        viewer[i]->SetColorLevel(50);
        openglrw[i]->Render();
    }
}
/*
void RenderWidget::ctButton5changed() {
    windowEdit->setText("0");
    levelEdit->setText("0");
    for (int i = 0; i < 3; i++)
    {
        viewer[i]->SetColorWindow(0);
        viewer[i]->SetColorLevel(0);
        openglrw[i]->Render();
    }
}*/

void RenderWidget::slider0Changed() {
    std::ostringstream stream;
    //stream << std::fixed << std::setprecision(4) << slider[0]->value() * spacing[2] - center[2];
    stream << std::fixed << std::setprecision(2) << (slider[0]->value() * spacing[2] - center[2]);
    sliceLabel[0]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    if (viewer[0]) {
        viewer[0]->SetSlice(viewer[0]->GetSliceMax() + viewer[0]->GetSliceMin() - slider[0]->value());
        viewer[0]->GetRenderWindow()->Render();
    }
}

void RenderWidget::slider1Changed() {
    std::ostringstream stream;
    //stream << std::fixed << std::setprecision(4) << slider[0]->value() * spacing[2] - center[2];
    stream << std::fixed << std::setprecision(2) << (slider[1]->value() * spacing[1] - center[1]);
    sliceLabel[1]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    if (viewer[1]) {
        viewer[1]->SetSlice(slider[1]->value());
        viewer[1]->GetRenderWindow()->Render();
    }
}

void RenderWidget::slider2Changed() {
    std::ostringstream stream;
    //stream << std::fixed << std::setprecision(4) << slider[0]->value() * spacing[2] - center[2];
    stream << std::fixed << std::setprecision(2) << (slider[2]->value() * spacing[0] - center[0]);
    sliceLabel[2]->setText(QString::fromUtf8(" S: " + (stream.str().append("mm "))));  // 设置文本内容
    if (viewer[2]) {
        viewer[2]->SetSlice(slider[2]->value());
        viewer[2]->GetRenderWindow()->Render();
    }
}

//初始化包围盒  QPushButton* buttonRoi
void RenderWidget::initBoxWidget()
{
    if (!renderer || !volume)
    {
        return;
    }
    m_boxWidget = vtkSmartPointer<vtkBoxWidget2>::New();
    m_boxWidget->SetInteractor(renderWindow->GetInteractor());
    m_boxWidget->CreateDefaultRepresentation();

    vtkSmartPointer<vtkBoxRepresentation> boxRepresentation = vtkSmartPointer<vtkBoxRepresentation>::New();
    m_boxWidget->SetRepresentation(boxRepresentation);

    // 获取包围盒的属性对象
    vtkSmartPointer<vtkProperty> boxProperty = boxRepresentation->GetHandleProperty();
    vtkSmartPointer<vtkProperty> boxLine = boxRepresentation->GetOutlineProperty();
    // 设置属性的颜色
    boxProperty->SetColor(0.0, 0.0, 1.0);
    boxLine->SetColor(1.0, 0.0, 0.8);
    // 更新包围盒的外观
    m_boxWidget->GetRepresentation()->BuildRepresentation();
    m_boxWidget->GetRepresentation()->SetPlaceFactor(1);
    m_boxWidget->GetRepresentation()->PlaceWidget(volume->GetBounds());

    vtkSmartPointer<vtkBoxWidget3DCallback> m_boxWidgetCallback = vtkSmartPointer<vtkBoxWidget3DCallback>::New();
    m_boxWidgetCallback->setVolume(volume);
    m_boxWidget->AddObserver(vtkCommand::InteractionEvent, m_boxWidgetCallback);
    m_boxWidget->SetEnabled(TRUE);
    m_boxWidget->TranslationEnabledOff();
    m_boxWidget->RotationEnabledOff();
    renderWindow->Render();
}

//包围盒开启关闭
void RenderWidget::activateBoxWidget()
{
    if (!m_boxWidget)
    {
        initBoxWidget();
    }
    if (setBoxV->isChecked())
    {
        m_boxWidget->SetEnabled(true);
        renderWindow->Render();
    }
    else
    {
        m_boxWidget->SetEnabled(false);
        renderWindow->Render();
    }

}

//不同算法渲染  QPushButton* buttonRendering
void RenderWidget::InitRendering()
{
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4);
    volumeProperty->SetDiffuse(0.6);
    volumeProperty->SetSpecular(0.2);

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    compositeOpacity->AddPoint(70, 0.00);
    compositeOpacity->AddPoint(90, 0.40);
    compositeOpacity->AddPoint(180, 0.60);
    volumeProperty->SetScalarOpacity(compositeOpacity);

    //创建一个vtkColorTransferFunction对象color来定义体积渲染的颜色传递函数。
    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
    color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
    color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
    color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
    volumeProperty->SetColor(color);

    //建一个vtkPiecewiseFunction对象gradientOpacity来定义体积数据的梯度不透明度。
    // 添加点（梯度数值，不透明度）对到gradientOpacity。这些定义了在体积数据中不同梯度数值对应的不透明度。
    //将gradientOpacity设置为volumeProperty的梯度不透明度。
  //  vtkSmartPointer<vtkPiecewiseFunction> gradientOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
  //  gradientOpacity->AddPoint(10, 0.0);
  //  gradientOpacity->AddPoint(90, 0.5);
  //  gradientOpacity->AddPoint(100, 1.0);
   // volumeProperty->SetGradientOpacity(gradientOpacity);

    volume->SetProperty(volumeProperty);
    ren->SetBackground(0.70, 0.72, 0.88);
    ren->AddVolume(volume);
    renderWindow->AddRenderer(ren);

    ROI->setVisible(true);
    setBoxV->setVisible(true);
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    glWidget->setRenderWindow(renderWindow);
    glWidget->show();
}

//渲染分割结果
void RenderWidget::ResultRendering()
{
    /*
    std::string filePathUtf8 = path;
    Message message;
    Message::runPythonFunction(6, 0, filePathUtf8, " ");
    Message::runPythonFunction(7, 0, filePathUtf8, " ");
    */

    teeth_reader->SetFileName("D:/Data/teeth.nii.gz");
    teeth_reader->Update();

    alveolar_bone_reader->SetFileName("D:/Data/bone.nii.gz");
    alveolar_bone_reader->Update();

    //所有牙齿
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> teeth_volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    teeth_volumeMapper->SetInputData(teeth_reader->GetOutput());;
    teeth_volumeMapper->SetSampleDistance(teeth_volumeMapper->GetSampleDistance() / 2);

    teeth_volumeProperty->SetInterpolationTypeToLinear();
    teeth_volumeProperty->ShadeOn();
    teeth_volumeProperty->SetAmbient(0.4);
    teeth_volumeProperty->SetDiffuse(0.6);
    teeth_volumeProperty->SetSpecular(0.2);

    vtkSmartPointer<vtkPiecewiseFunction> teeth_compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    teeth_compositeOpacity->AddPoint(0.00, 0.00);
    teeth_compositeOpacity->AddPoint(1.00, 1.00);
    //  teeth_compositeOpacity->AddPoint(30.00, 1.00);
    teeth_volumeProperty->SetScalarOpacity(teeth_compositeOpacity);

    vtkSmartPointer<vtkColorTransferFunction> teeth_color = vtkSmartPointer<vtkColorTransferFunction>::New();
    teeth_color->AddRGBPoint(0.00, 0.00, 0.00, 0.00);
    teeth_color->AddRGBPoint(1.00, 1.00, 1.00, 1.00);
    teeth_volumeProperty->SetColor(teeth_color);


    teeth_volume->SetMapper(teeth_volumeMapper);
    teeth_volume->SetProperty(teeth_volumeProperty);

    //牙槽骨

    vtkSmartPointer<vtkGPUVolumeRayCastMapper> alveolar_bone_volumeMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    alveolar_bone_volumeMapper->SetInputData(alveolar_bone_reader->GetOutput());;
    alveolar_bone_volumeMapper->SetSampleDistance(alveolar_bone_volumeMapper->GetSampleDistance() / 2);

    vtkSmartPointer<vtkVolumeProperty>  alveolar_bone_volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
    alveolar_bone_volumeProperty->SetInterpolationTypeToLinear();
    alveolar_bone_volumeProperty->ShadeOn();
    alveolar_bone_volumeProperty->SetAmbient(0.4);
    alveolar_bone_volumeProperty->SetDiffuse(0.6);
    alveolar_bone_volumeProperty->SetSpecular(0.2);

    vtkSmartPointer<vtkPiecewiseFunction> alveolar_bone_compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    alveolar_bone_compositeOpacity->AddPoint(0.00, 0.00);
    alveolar_bone_compositeOpacity->AddPoint(1.00, 1.00);
    alveolar_bone_volumeProperty->SetScalarOpacity(alveolar_bone_compositeOpacity);

    vtkSmartPointer<vtkColorTransferFunction> alveolar_bone_color = vtkSmartPointer<vtkColorTransferFunction>::New();
    alveolar_bone_color->AddRGBPoint(0.00, 0.00, 0.00, 0.00);
    alveolar_bone_color->AddRGBPoint(1.00, 1.00, 1.00, 1.00);
    alveolar_bone_volumeProperty->SetColor(alveolar_bone_color);


    alveolar_bone_volume->SetMapper(alveolar_bone_volumeMapper);
    alveolar_bone_volume->SetProperty(alveolar_bone_volumeProperty);

    //ren->SetBackground(1.0, 1.0, 1.0);
    ren->SetUseDepthPeeling(true);
    ren->SetOcclusionRatio(0.1);
    ren->AddVolume(alveolar_bone_volume); 
    ren->AddVolume(teeth_volume);
    renderWindow->AddRenderer(ren);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    glWidget->setRenderWindow(renderWindow);
    glWidget->show();



    vtkSmartPointer<vtkImageData> imageData = teeth_reader->GetOutput();
    double range[2];
    imageData->GetScalarRange(range);
    //   qDebug() << "体素值范围：" << range[0] << " - " << range[1];

    QTableWidget* tableWidget = new QTableWidget(dockwidget);
    tableWidget->setRowCount(range[1]+1);  // 行数为体素值的最大值
    tableWidget->setColumnCount(2);  // 列数为2，一列显示牙齿编号，一列显示复选框和显示状态

    // 填充表格内容
    for (int i = 0; i <= range[1]+1; ++i) {
        QTableWidgetItem* item;
        if (i == 0) {
            item = new QTableWidgetItem("显示/隐藏全部");
        }
        else {
            item = new QTableWidgetItem(QString("%1号牙齿").arg(i));
        }
        tableWidget->setItem(i, 0, item);

        float totalOpacity = 0.0f;

        QWidget* widget = new QWidget;
        QHBoxLayout* layout = new QHBoxLayout(widget);
        layout->setAlignment(Qt::AlignCenter);

        QCheckBox* checkBox = new QCheckBox;


        if (i == 0) {
            connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
                if (state == Qt::Checked) {
                    teethPiecewise->RemoveAllPoints();
                    teethPiecewise->AddPoint(0.00, 0.00);
                    teethPiecewise->AddPoint(1.00, 1.00);
                    teeth_volumeProperty->SetScalarOpacity(teethPiecewise);
                    renderWindow->Render();
                }
                else {
                    teethPiecewise->RemoveAllPoints();
                    teethPiecewise->AddPoint(0, 0.00);
                    teethPiecewise->AddPoint(41, 0.00);
                    teeth_volumeProperty->SetScalarOpacity(teethPiecewise);
                    renderWindow->Render();
                }
                });

        }
        else {
            connect(checkBox, &QCheckBox::stateChanged, [=](int state) {
                // 根据复选框状态显示或隐藏对应牙齿
                // 在此处编写显示或隐藏牙齿的代码
                if (state == Qt::Checked) {
                    teethPiecewise->AddPoint(0, 0.00);
                    teethPiecewise->AddPoint(i - 0.2, 0.00);
                    teethPiecewise->AddPoint(i, 1.00);
                    teethPiecewise->AddPoint(i + 0.2, 0.00);
                    teethPiecewise->AddPoint(41, 0.00);
                    teeth_volumeProperty->SetScalarOpacity(teethPiecewise);
                    renderWindow->Render();
                }
                else {
                    teethPiecewise->RemovePoint(i - 0.2);
                    teethPiecewise->RemovePoint(i);
                    teethPiecewise->RemovePoint(i + 0.2);
                    teeth_volumeProperty->SetScalarOpacity(teethPiecewise);
                    renderWindow->Render();
                }
                });
        }
        layout->addWidget(checkBox);

        QLabel* label = new QLabel("显示状态");
        layout->addWidget(label);

        tableWidget->setCellWidget(i, 1, widget);
    }
    // 显示表格
    tableWidget->setMinimumSize(245, 400);
    tableWidget->show();


    before_checkbox->setChecked(Qt::Checked);
    teeth_checkbox->setChecked(Qt::Checked);
    alveolar_bone_checkbox->setChecked(Qt::Checked);

    dockwidget->setVisible(true);
}

//选择不同算法
void RenderWidget::updateBlendMode()
{
    if (compositeRadioButton->isChecked())
    {
        volumeMapper->SetBlendModeToComposite();
        volume->SetMapper(volumeMapper);
        renderWindow->Render();
    }
    else if (maximumIntensityRadioButton->isChecked())
    {
        volumeMapper->SetBlendModeToMaximumIntensity();
        volume->SetMapper(volumeMapper);
        renderWindow->Render();
    }
    else if (minimumIntensityRadioButton->isChecked())
    {
        volumeMapper->SetBlendModeToMinimumIntensity();
        volume->SetMapper(volumeMapper);
        renderWindow->Render();
    }
    else if (addictiveRadioButton->isChecked())
    {
        volumeMapper->SetBlendModeToAdditive();
        volume->SetMapper(volumeMapper);
        renderWindow->Render();
    }
    else if (vtkGPUVolumeRayCastRadioButton->isChecked())
    {
        volumeMapper1->SetSampleDistance(volumeMapper1->GetSampleDistance() / 2);
        volume->SetMapper(volumeMapper1);
        renderWindow->Render();
    }
}

//选择显示对象
void RenderWidget::onCheckboxStateChanged()
{
    if (before_checkbox->isChecked())
    {
        volume->SetVisibility(true); // 设置原图像可见
    }
    else
    {
        volume->SetVisibility(false);
    }

    renderWindow->Render();

    if (teeth_checkbox->isChecked())
    {
        viewer[0]->SetInputConnection(teeth_reader->GetOutputPort());
        viewer[1]->SetInputConnection(teeth_reader->GetOutputPort());
        viewer[2]->SetInputConnection(teeth_reader->GetOutputPort());
        for (int i = 0; i < 3; i++)
        {
            windowEdit->setReadOnly(true);
            viewer[i]->SetColorWindow(0);
            levelEdit->setReadOnly(true);
            viewer[i]->SetColorLevel(0);
        }
        teeth_volume->SetVisibility(true); // 设置所有牙齿可见
    }
    else
    {
        viewer[0]->SetInputConnection(niiReader->GetOutputPort());
        viewer[1]->SetInputConnection(niiReader->GetOutputPort());
        viewer[2]->SetInputConnection(niiReader->GetOutputPort());
        for (int i = 0; i < 3; i++)
        {
            windowEdit->setReadOnly(false);
            viewer[i]->SetColorWindow(windowEdit->text().toDouble());
            levelEdit->setReadOnly(false);
            viewer[i]->SetColorLevel(levelEdit->text().toDouble());
        }
        teeth_volume->SetVisibility(false);
    }
    viewer[0]->Render();
    viewer[1]->Render();
    viewer[2]->Render();
    renderWindow->Render();

    if (alveolar_bone_checkbox->isChecked())
    {
        alveolar_bone_volume->SetVisibility(true); // 设置牙槽骨可见
    }
    else
    {
        alveolar_bone_volume->SetVisibility(false);
    }
    renderWindow->Render();
}

//阈值颜色预设  QComboBox* comboBox
void RenderWidget::onComboBoxActivated(int index)
{
    if (index == 0) {
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(70, 0.00);
        compositeOpacity->AddPoint(90, 0.40);
        compositeOpacity->AddPoint(180, 0.60);
        volumeProperty->SetScalarOpacity(compositeOpacity);


        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
        color->AddRGBPoint(64.00, 1.00, 0.52, 0.30);
        color->AddRGBPoint(190.0, 1.00, 1.00, 1.00);
        color->AddRGBPoint(220.0, 0.20, 0.20, 0.20);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 1) {//皮肤
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(-500, 0.00);
        compositeOpacity->AddPoint(-350, 1.00);
        compositeOpacity->AddPoint(0, 0.00);
        compositeOpacity->AddPoint(1000, 0.00);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-1000, 0.00, 0.00, 0.00);
        color->AddRGBPoint(-500.0, 0.80, 0.65, 0.56);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 2) {//骨骼
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(500, 0.00);
        compositeOpacity->AddPoint(1000, 1.00);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.000, 0.00, 0.00, 0.00);
        color->AddRGBPoint(500.0, 1.00, 1.00, 1.00);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 3) {//CT-Bone
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(-3024, 0.00);
        compositeOpacity->AddPoint(-16.45, 0.00);
        compositeOpacity->AddPoint(641.38, 0.72);
        compositeOpacity->AddPoint(3071, 0.71);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-3024, 0.00, 0.00, 0.00);
        color->AddRGBPoint(-16.45, 0.73, 0.65, 0.56);
        color->AddRGBPoint(641.38, 0.91, 0.82, 0.55);
        color->AddRGBPoint(3071, 1, 1, 1);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 4) {//CT-AAA
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(-3024, 0.00);
        compositeOpacity->AddPoint(143.56, 0.00);
        compositeOpacity->AddPoint(166.22, 0.69);
        compositeOpacity->AddPoint(214.39, 0.70);
        compositeOpacity->AddPoint(419.74, 0.83);
        compositeOpacity->AddPoint(3071, 0.80);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-3024, 0.00, 0.00, 0.00);
        color->AddRGBPoint(143.56, 0.62, 0.36, 0.18);
        color->AddRGBPoint(166.22, 0.88, 0.60, 0.29);
        color->AddRGBPoint(214.39, 1, 1, 1);
        color->AddRGBPoint(419.74, 1, 0.94, 0.95);
        color->AddRGBPoint(3071, 0.83, 066, 1);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 5) {//CT-Cardiac3
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(-3024, 0.00);
        compositeOpacity->AddPoint(-86.98, 0.00);
        compositeOpacity->AddPoint(45.38, 0.17);
        compositeOpacity->AddPoint(139.92, 0.59);
        compositeOpacity->AddPoint(347.91, 0.61);
        compositeOpacity->AddPoint(1224.16, 0.61);
        compositeOpacity->AddPoint(3071, 0.62);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-3024, 0.00, 0.00, 0.00);
        color->AddRGBPoint(-86.98, 0.00, 0.25, 1);
        color->AddRGBPoint(45.38, 1, 0, 0);
        color->AddRGBPoint(139.92, 1, 0.89, 0.89);
        color->AddRGBPoint(347.91, 1, 1, 0.25);
        color->AddRGBPoint(1224.16, 1, 1, 1);
        color->AddRGBPoint(3071, 0.83, 0.66, 1);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }

    else if (index == 6) {//CT-MIP
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(-3024, 0.00);
        compositeOpacity->AddPoint(-637.62, 0.00);
        compositeOpacity->AddPoint(700, 1.00);
        compositeOpacity->AddPoint(3071, 1.00);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(-3024, 0.00, 0.00, 0.00);
        color->AddRGBPoint(-637.62, 1, 1, 1);
        color->AddRGBPoint(700, 1, 1, 1);
        color->AddRGBPoint(3071, 1, 1, 1);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 7) {//MR-Default
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(0.00, 0.00);
        compositeOpacity->AddPoint(20.00, 0.00);
        compositeOpacity->AddPoint(40.00, 0.15);
        compositeOpacity->AddPoint(120.00, 0.30);
        compositeOpacity->AddPoint(220.00, 0.38);
        compositeOpacity->AddPoint(1024.00, 0.50);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.00, 0.00, 0.00, 0.00);
        color->AddRGBPoint(20.00, 0.17, 0, 0);
        color->AddRGBPoint(40.00, 0.40, 0.15, 0.08);
        color->AddRGBPoint(120.00, 0.78, 0.61, 0.38);
        color->AddRGBPoint(220.00, 0.85, 0.84, 0.79);
        color->AddRGBPoint(1024.00, 1, 1, 1);
        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
    else if (index == 8) {//分割结果
        vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        compositeOpacity->AddPoint(0.00, 0.00);
        compositeOpacity->AddPoint(24.00, 1.00);
        compositeOpacity->AddPoint(30.00, 1.00);
        volumeProperty->SetScalarOpacity(compositeOpacity);

        vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
        color->AddRGBPoint(0.00, 0.00, 0.00, 0.00);
        color->AddRGBPoint(30.00, 1.00, 1.00, 1.00);

        volumeProperty->SetColor(color);
        renderWindow->Render();
    }
}

//鼠标移入移出单选框事件
void RenderWidget::onRadioButtonEntered()
{
    /*   QRadioButton* radioButton = qobject_cast<QRadioButton*>(sender());
       if (radioButton && radioButtons.contains(radioButton)) {
           QToolTip::showText(radioButton->mapToGlobal(QPoint(0, 0)), radioButtons.value(radioButton));
       }*/
}

//移出
void RenderWidget::onRadioButtonLeft()
{
    // QToolTip::hideText();
}

void RenderWidget::chooseTeethToShow()
{
    QString value = lineEdit->text();
    bool ok;
    int intValue = value.toInt(&ok);

    if (ok) {
        if (intValue == 0) {
            teethPiecewise->RemoveAllPoints();
            teethPiecewise->AddPoint(0.00, 0.00);
            teethPiecewise->AddPoint(1.00, 1.00);
        }
        else {
            teethPiecewise->RemoveAllPoints();
            for (int i = 1; i < 41; ++i) {
                if (intValue == i) {

                    teethPiecewise->AddPoint(0, 0.00);
                    teethPiecewise->AddPoint(i - 0.2, 0.00);
                    teethPiecewise->AddPoint(i, 1.00);
                    teethPiecewise->AddPoint(i + 0.2, 0.00);
                    teethPiecewise->AddPoint(41, 0.00);
                }

            }

        }   teeth_volumeProperty->SetScalarOpacity(teethPiecewise);
        renderWindow->Render();

    }
    else {
        // TODO: 在这里添加值无效的操作
    }
}