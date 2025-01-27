#include "MyVtkInteractorStyleImage.h"
#include <QtDebug>
#include "RenderWidget.h"
#include "vtkImageData.h"
#include "vtkPropPicker.h"
#include "vtkCollection.h"
#include "vtkAssemblyNode.h"
#include "vtkProp.h"
#include "vtkAssemblyPath.h"
#include "vtkImageActor.h"
#include "vtkRenderWindow.h"
vtkStandardNewMacro(MyVtkInteractorStyleImage);                             //标准类生成宏


void MyVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer)
{
    this->ImageViewer = imageViewer;
    this->MinSlice = imageViewer->GetSliceMin();
    this->MaxSlice = imageViewer->GetSliceMax();
    this->Slice = imageViewer->GetSlice();
}

void MyVtkInteractorStyleImage::SetRenderer(vtkRenderer* ren)
{
    this->ren = ren;
}

void MyVtkInteractorStyleImage::SetLineEdit(QLineEdit* lineEdit)
{
    this->lineEdit = lineEdit;
}

double MyVtkInteractorStyleImage::GetVoxelValue(vtkImageData* imageData, double x, double y, double z)
{
    int imageSize[3];
    imageData->GetDimensions(imageSize);

    int extent[6];
    imageData->GetExtent(extent);

    double spacing[3];
    imageData->GetSpacing(spacing);

    int indexX = static_cast<int>((x - extent[0]) / spacing[0]);
    int indexY = static_cast<int>((y - extent[2]) / spacing[1]);
    int indexZ = static_cast<int>((z - extent[4]) / spacing[2]);

    if (indexX < 0 || indexX >= imageSize[0] ||
        indexY < 0 || indexY >= imageSize[1] ||
        indexZ < 0 || indexZ >= imageSize[2])
    {
        return 0.0; // 超出范围，返回默认值
    }

    vtkPointData* pointData = imageData->GetPointData();
    vtkDataArray* scalars = pointData->GetScalars();
    if (scalars == nullptr)
    {
        return 0.0;
    }

    int scalarIndex = indexX + indexY * imageSize[0] + indexZ * imageSize[0] * imageSize[1];
    return scalars->GetTuple1(scalarIndex);
}

/**
 * @brief MyVtkInteractorStyleImage::OnMouseWheelForward
 * 鼠标滚轮向前滚动
 * 切换图层显示
 */
void MyVtkInteractorStyleImage::OnMouseWheelForward()
{
    /*
    this->Slice = ImageViewer->GetSlice();
    if (this->Slice < this->MaxSlice)
    {
        this->Slice += 1;
        this->ImageViewer->SetSlice(this->Slice);
        this->ImageViewer->GetRenderWindow()->Render();
    }*/
}
/**
 * @brief MyVtkInteractorStyleImage::OnMouseWheelBackward
 * 鼠标滚轮向后滚动
 * 切换图层显示
 */
void MyVtkInteractorStyleImage::OnMouseWheelBackward()
{
    /*
    this->Slice = ImageViewer->GetSlice();
    if (this->Slice > this->MinSlice)
    {
        this->Slice -= 1;
        this->ImageViewer->SetSlice(this->Slice);
        this->ImageViewer->GetRenderWindow()->Render();
        //bppWidget->emitSliceChangedSignal(currentSlice);            //信息外传？
    }*/
}
/**
 * @brief MyVtkInteractorStyleImage::OnMiddleButtonDown
 * 鼠标中键按下
 */
void MyVtkInteractorStyleImage::OnMiddleButtonDown()
{

}
/**
 * @brief MyVtkInteractorStyleImage::OnMiddleButtonUp
 * 鼠标中键抬起
 */
void MyVtkInteractorStyleImage::OnMiddleButtonUp()
{

}
/**
 * @brief MyVtkInteractorStyleImage::OnLeftButtonDown
 * 鼠标左键按下
 */
void MyVtkInteractorStyleImage::OnLeftButtonDown()
{
    int* pos = this->GetInteractor()->GetEventPosition();
    int x = pos[0];
    int y = pos[1];

    double point[4];
    ren->SetDisplayPoint(x, y, 0);
    ren->DisplayToWorld();
    ren->GetWorldPoint(point);

    // 更新坐标显示
  //  QString text = "位置: (" + QString::number(point[0]) + ", " + QString::number(point[1]) + ", " + QString::number(point[2]) + ")";

 //   statusLabel->setText(text);
    // 获取体素值
    vtkImageData* imageData = static_cast<vtkImageData*>(ImageViewer->GetInput());
    int voxelValue = GetVoxelValue(imageData, point[0], point[1], point[2]);
    // 更新体素值显示
    QString voxelText = QString::number(voxelValue);
    lineEdit->setText(voxelText);

    // 调用基类的 OnMouseMove() 函数以保持原始交互行为
    vtkInteractorStyleImage::OnLeftButtonDown();
}
/**
 * @brief MyVtkInteractorStyleImage::OnLeftButtonUp
 * 鼠标左键抬起
 */
void MyVtkInteractorStyleImage::OnLeftButtonUp()
{
    
}
/**
 * @brief MyVtkInteractorStyleImage::OnMouseMove
 * 鼠标移动
 */
void MyVtkInteractorStyleImage::OnMouseMove()
{
    
}
/**
 * @brief MyVtkInteractorStyleImage::OnRightButtonDown
 * 鼠标右键按下
 */
void MyVtkInteractorStyleImage::OnRightButtonDown()
{
}
/**
 * @brief MyVtkInteractorStyleImage::OnRightButtonUp
 * 鼠标右键抬起
 */
void MyVtkInteractorStyleImage::OnRightButtonUp()
{
}

