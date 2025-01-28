#ifndef MyVtkInteractorStyleImage_H
#define MyVtkInteractorStyleImage_H

#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkObjectFactory.h"
#include "vtkInteractorObserver.h"
#include "vtkRenderWindowInteractor.h"
#include <vtkImageViewer2.h>
#include <qlineedit.h>
/**
 * @brief The MyVtkInteractorStyleImage class
 * 用户鼠标交互类
 */
class MyVtkInteractorStyleImage : public vtkInteractorStyleImage
{
public:
    static MyVtkInteractorStyleImage* New();
    vtkTypeMacro(MyVtkInteractorStyleImage, vtkInteractorStyleImage);
    void SetImageViewer(vtkImageViewer2* imageViewer);
    void SetRenderer(vtkRenderer* ren);
    void SetLineEdit(QLineEdit* lineEdit);

protected:
    virtual void OnMouseWheelForward() override;                            //鼠标滚轮向前滚动

    virtual void OnMouseWheelBackward() override;                           //鼠标滚轮向后滚动

    virtual void OnMiddleButtonDown() override;                             //鼠标中键按下

    virtual void OnMiddleButtonUp() override;                               //鼠标中键抬起

    virtual void OnLeftButtonDown() override;                               //鼠标左键按下

    virtual void OnLeftButtonUp() override;                                 //鼠标左键抬起

    virtual void OnMouseMove() override;                                    //鼠标移动

    virtual void OnRightButtonDown() override;                              //鼠标右键按下

    virtual void OnRightButtonUp() override;                                //鼠标右键抬起

    double GetVoxelValue(vtkImageData* imageData, double x, double y, double z);

private:
    vtkImageViewer2* ImageViewer;
    QLineEdit* lineEdit;
    vtkRenderer* ren;
    int Slice;
    int MinSlice;
    int MaxSlice;
};

#endif // MyVtkInteractorStyleImage_H
