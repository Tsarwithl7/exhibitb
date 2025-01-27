/*
 *
 * 程序的数据管理模块
 * 作者：吴国正（与数据库的所有通信，包括增删改等）
 * 孙恒辉（基本的ui和表格布局设计）
 *
 */
#include "TableWidget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include "PatientWindow.h"

#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkImageReslice.h>
#include <vtkTransform.h>
#include <vtkMatrix4x4.h>

TableWidget::TableWidget(QWidget* parent) : QWidget(parent)
{
    std::wstring connectionString = L"DRIVER={SQL Server};SERVER=MS;DATABASE=Dicom;UID=sa;PWD=123456;";
    connection.connect(connectionString);
    std::vector<std::vector<std::string>> columnData = connection.readTableColumnsData(tableName, columnName);
    int numRows = columnData.size();

    QPushButton* addButton = new QPushButton("新建病例");
    addButton->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 2px solid #E0E0E0;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    QPushButton* deleteButton = new QPushButton("删除病例");
    deleteButton->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 2px solid #E0E0E0;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    QPushButton* openButton = new QPushButton("可视化查看");
    openButton->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 2px solid #E0E0E0;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    QPushButton* editButton = new QPushButton("编辑数据");
    editButton->setStyleSheet("QPushButton {"
        "border-radius: 20px;"
        "border: 2px solid #E0E0E0;"
        "}"
        "QPushButton:hover { background-color: #E6F1FB; }");
    tableWidget = new QTableWidget(numRows, 10);
    QFrame* frame = new QFrame;
    QHBoxLayout* layout = new QHBoxLayout(frame);
    QVBoxLayout* layout2 = new QVBoxLayout(this);
    layout->addWidget(addButton);
    layout->addWidget(deleteButton);
    layout->addWidget(openButton);
    layout->addWidget(editButton);
    layout2->addWidget(tableWidget);
    layout2->addWidget(frame);
    addButton->setFixedSize(130, 50);
    deleteButton->setFixedSize(130, 50);
    openButton->setFixedSize(130, 50);
    editButton->setFixedSize(130, 50);

    setWindowTitle("MainWindow");
    resize(1066, 766);

    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->resize(750, 690);

    connect(addButton, &QPushButton::clicked, this, &TableWidget::onAddButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &TableWidget::onDeleteButtonClicked);
    connect(openButton, &QPushButton::clicked, this, &TableWidget::open);
    connect(editButton, &QPushButton::clicked, this, &TableWidget::edit);

    // 获取水平表头
    QHeaderView* horizontalHeader = tableWidget->horizontalHeader();
    // 将水平表头设置为自适应模式
    horizontalHeader->setSectionResizeMode(QHeaderView::Stretch);
    /*
    tableWidget->setColumnWidth(0, 120);
    tableWidget->setColumnWidth(1, 120);
    tableWidget->setColumnWidth(2, 120);
    tableWidget->setColumnWidth(3, 120);
    tableWidget->setColumnWidth(4, 120);
    tableWidget->setColumnWidth(5, 120);
    tableWidget->setColumnWidth(6, 120);
    */
    tableWidget->setRowHeight(3, 25);

    QStringList header;
    header << "患者姓名" << "患者年龄" << "患者性别" << "检查日期" << "描述" << "成像设备" << "ACCESSION" << "CBCT数据" << "曲断数据" << "头侧数据";
    tableWidget->setHorizontalHeaderLabels(header);

    contextMenu = new QMenu(this);
    QAction* openAction = new QAction("可视化查看", this);
    QAction* editAction = new QAction("编辑数据", this);
    connect(openAction, &QAction::triggered, this, &TableWidget::open);
    connect(editAction, &QAction::triggered, this, &TableWidget::edit);
    contextMenu->addAction(openAction);
    contextMenu->addAction(editAction);

    tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    //optionLayout->setAlignment(levelLabel, Qt::AlignRight);

    connect(tableWidget, &QTableWidget::customContextMenuRequested, this, &TableWidget::showContextMenu);
    //connect(tableWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));

    for (int i = 0; i < numRows; i++) {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(columnData[i][0])));
        tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(columnData[i][1])));
        tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(columnData[i][2])));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(columnData[i][3])));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(columnData[i][4])));
        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(columnData[i][5])));
        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::fromStdString(columnData[i][6])));
        if (columnData[i][7] != "") 
        {
            tableWidget->setItem(i, 7, new QTableWidgetItem("有"));
        }
        else {
            tableWidget->setItem(i, 7, new QTableWidgetItem("无"));
        }
        if (columnData[i][8] != "")
        {
            tableWidget->setItem(i, 8, new QTableWidgetItem("有"));
        }
        else {
            tableWidget->setItem(i, 8, new QTableWidgetItem("无"));
        }
        if (columnData[i][9] != "")
        {
            tableWidget->setItem(i, 9, new QTableWidgetItem("有"));
        }
        else {
            tableWidget->setItem(i, 9, new QTableWidgetItem("无"));
        }
    }

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        QTableWidgetItem* item = tableWidget->item(row, 7);
        if (item) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }

        item = tableWidget->item(row, 8);
        if (item) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
        item = tableWidget->item(row, 9);
        if (item) {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }

    connect(tableWidget, &QTableWidget::itemSelectionChanged, this, &TableWidget::onItemSelectionChanged);
    connect(tableWidget, &QTableWidget::itemChanged, this, &TableWidget::onItemChanged);
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, &TableWidget::onCellDoubleClicked);
    
    fileLabel = new QLabel(this);
    fileLabel->setText("111");
    fileLabel->hide();
}

void TableWidget::onCellDoubleClicked(int row, int column) {
    if (tableWidget->item(row, column) == nullptr) 
        originText = "";
    else if (tableWidget->item(row, column)->text().isEmpty()) 
        originText = "";
    else
        originText = tableWidget->item(row, column)->text().toStdString();
}

void TableWidget::onItemChanged(QTableWidgetItem* item) {
    int row = item->row();
    int column = item->column();

    std::string valueCon = "";
    switch (column) {
    case 0:
        valueCon += "Name = ";
        break;
    case 1:
        valueCon += "Age = ";
        break;
    case 2:
        valueCon += "Sex = ";
        break;
    case 3:
        valueCon += "Date = ";
        break;
    case 4:
        valueCon += "Description = ";
        break;
    case 5:
        valueCon += "Modality = ";
        break;
    case 6:
        valueCon += "RIS = ";
        break;
    }
    valueCon += "'";
    valueCon += (item->text().toStdString());
    valueCon += "'";

    std::string condition = "";
    condition += "RIS = ";
    condition += "'";
    if (column == 6)
        condition += originText;
    else
        condition += (tableWidget->item(row, 6)->text().toStdString());
    condition += "'";

    if (!connection.updateData(tableName, valueCon, condition)) {
        item->setText(QString::fromStdString(originText));
    }
}

void TableWidget::onItemSelectionChanged()
{
    QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
    if (!selectedItems.isEmpty()) {
        currentRow = tableWidget->row(selectedItems.first());
        //fileLabel->setText(QString::fromStdString(std::to_string(currentRow)));
    }
    else {
        currentRow = -1;
    }
}

void TableWidget::onAddButtonClicked()
{
    QDialog dialog;
    dialog.setWindowTitle("新建病例信息");
    //dialog.setStyleSheet("background-color: white;");

    QLabel nameLabel("姓名:");
    QLineEdit nameLineEdit;
    QLabel ageLabel("年龄:");
    QLineEdit ageLineEdit;
    QLabel genderLabel("性别:");
    QLineEdit genderLineEdit;
    QLabel dateLabel("检查日期:");
    QLineEdit dateLineEdit;
    QLabel descriptionLabel("描述:");
    QLineEdit descriptionLineEdit;
    QLabel deviceLabel("成像设备:");
    QLineEdit deviceLineEdit;
    QLabel accessionLabel("ACCESSION:");
    QLineEdit accessionLineEdit;

    QLabel cbctLabel("CBCT路径：");
    QPushButton cbctButton("选择...");
    QString cbctPath = "";
    QLabel quduanLabel("曲断路径：");
    QPushButton quduanButton("选择...");
    QString quduanPath = "";
    QLabel touceLabel("头侧路径：");
    QPushButton touceButton("选择...");
    QString toucePath = "";

    QPushButton okButton("确定");
    QPushButton cancelButton("取消");

    QFormLayout layout(&dialog);
    layout.addRow(&nameLabel, &nameLineEdit);
    layout.addRow(&ageLabel, &ageLineEdit);
    layout.addRow(&genderLabel, &genderLineEdit);
    layout.addRow(&dateLabel, &dateLineEdit);
    layout.addRow(&descriptionLabel, &descriptionLineEdit);
    layout.addRow(&deviceLabel, &deviceLineEdit);
    layout.addRow(&accessionLabel, &accessionLineEdit);
    layout.addRow(&cbctLabel, &cbctButton);
    layout.addRow(&quduanLabel, &quduanButton);
    layout.addRow(&touceLabel, &touceButton);
    layout.addRow(&okButton, &cancelButton);

    QObject::connect(&cbctButton, &QPushButton::clicked, [&]() {
        cbctPath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("CBCT文件(*.dcm *.nii *.nii.gz)"));
        if (!cbctPath.isEmpty()) {
            std::string filePath = cbctPath.toStdString();
            // .dcm的话自动读一下数据
            if (filePath.substr(filePath.length() - 4) == ".dcm") {
                LoadInformation loader;
                if (nameLineEdit.text().isEmpty()) {
                    std::string name = loader.readName(loader.fileformat, filePath);
                    nameLineEdit.setText(QString::fromStdString(name));
                }
                if (ageLineEdit.text().isEmpty()) {
                    std::string age = loader.readAge(loader.fileformat, filePath);
                    ageLineEdit.setText(QString::fromStdString(age));
                }
                if (genderLineEdit.text().isEmpty()) {
                    std::string gender = loader.readSex(loader.fileformat, filePath);
                    genderLineEdit.setText(QString::fromStdString(gender));
                }
                if (dateLineEdit.text().isEmpty()) {
                    std::string date = loader.readSeriesDate(loader.fileformat, filePath);
                    dateLineEdit.setText(QString::fromStdString(date));
                }
                if (descriptionLineEdit.text().isEmpty()) {
                    std::string description = loader.readDescription(loader.fileformat, filePath);
                    descriptionLineEdit.setText(QString::fromStdString(description));
                }
                if (deviceLineEdit.text().isEmpty()) {
                    std::string device = loader.readModality(loader.fileformat, filePath);
                    deviceLineEdit.setText(QString::fromStdString(device));
                }
                if (accessionLineEdit.text().isEmpty()) {
                    std::string accession = loader.readAccessionNumber(loader.fileformat, filePath);
                    accessionLineEdit.setText(QString::fromStdString(accession));
                }
            }
            QMessageBox::information(nullptr, "提示", "CBCT路径选择成功");
            cbctButton.setText("更改...");
        }
        });

    QObject::connect(&quduanButton, &QPushButton::clicked, [&]() {
        quduanPath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("曲断文件(*.png *.jpg *.jpeg)"));
        if (!quduanPath.isEmpty()) {
            QMessageBox::information(nullptr, "提示", "曲断路径选择成功");
            quduanButton.setText("更改...");
        }
        });

    QObject::connect(&touceButton, &QPushButton::clicked, [&]() {
        toucePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("头侧文件(*.png *.jpg *.jpeg)"));
        if (!toucePath.isEmpty()) {
            QMessageBox::information(nullptr, "提示", "头侧路径选择成功");
            touceButton.setText("更改...");
        }
        });

    QObject::connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    QObject::connect(&okButton, &QPushButton::clicked, [&]() {
        // 获取文本框中的输入内容并进行相应处理
        std::string name = nameLineEdit.text().toStdString();
        std::string age = ageLineEdit.text().toStdString();
        std::string gender = genderLineEdit.text().toStdString();
        std::string date = dateLineEdit.text().toStdString();
        std::string description = descriptionLineEdit.text().toStdString();
        std::string device = deviceLineEdit.text().toStdString();
        std::string accession = accessionLineEdit.text().toStdString();

        std::string cbct = cbctPath.toStdString();
        std::string cbct_out = "";
        std::string quduan = quduanPath.toStdString();
        std::string touce = toucePath.toStdString();

        // 查看路径后三位，并判断路径指向的数据是.nii类型还是.dcm类型
        if (cbct.substr(cbct.length() - 4) == ".dcm") {
            // .dcm的话转成.nii
            // 读取DICOM数据
            std::string temp = cbct;
            size_t lastSlashPos = temp.rfind('/');
            if (lastSlashPos != std::string::npos) {
                temp = temp.substr(0, lastSlashPos);
            }
            vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
            dicomReader->SetDirectoryName(temp.c_str());
            dicomReader->Update();

            // 创建vtkImageReslice过滤器并设置翻转参数
            vtkSmartPointer<vtkImageReslice> resliceFilter = vtkSmartPointer<vtkImageReslice>::New();
            resliceFilter->SetInputConnection(dicomReader->GetOutputPort());
            resliceFilter->SetResliceAxesDirectionCosines(-1, 0, 0, 0, -1, 0, 0, 0, -1); // 设置翻转方向

            // 将DICOM数据转换为NIfTI数据
            cbct_out = "D:/Data/CBCT/" + name + ".nii";
            vtkSmartPointer<vtkNIFTIImageWriter> niftiWriter = vtkSmartPointer<vtkNIFTIImageWriter>::New();
            niftiWriter->SetFileName(cbct_out.c_str());
            niftiWriter->SetInputConnection(resliceFilter->GetOutputPort());
            niftiWriter->Write();
        }
        else if (cbct.substr(cbct.length() - 4) == ".nii" || cbct.substr(cbct.length() - 7) == ".nii.gz") {
            //.nii路径直接传递就可以
            cbct_out = cbct;
        }

        //数据库处理
        std::string fullColumnNames = "Name, Age, Sex, Date, Description, Modality, RIS, InputPath, OutputPath, QuduanPath, ToucePath";
        std::string fullValues = "'" + name + "', '" + age + "', '" + gender + "', '" + date + "', '" + description + "', '" + device + "', '" + accession + "', '" + cbct + "', '" + cbct_out + "', '" + quduan + "', '" + touce + "'";
        if (connection.insertData(tableName, fullColumnNames, fullValues)) {
            //表格处理
            int row = tableWidget->rowCount();
            tableWidget->insertRow(row);
            tableWidget->blockSignals(true);
            tableWidget->setItem(row, 0, new QTableWidgetItem(nameLineEdit.text()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(ageLineEdit.text()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(genderLineEdit.text()));
            if (date == "")
            {
                tableWidget->setItem(row, 3, new QTableWidgetItem("1900-01-01"));
            }
            else if (date.length() == 8) {
                std::string formattedDate = date.substr(0, 4) + "-" + date.substr(4, 2) + "-" + date.substr(6, 2);
                tableWidget->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(formattedDate)));
            }
            else {
                tableWidget->setItem(row, 3, new QTableWidgetItem(dateLineEdit.text()));
            }
            tableWidget->setItem(row, 4, new QTableWidgetItem(descriptionLineEdit.text()));
            tableWidget->setItem(row, 5, new QTableWidgetItem(deviceLineEdit.text()));
            tableWidget->setItem(row, 6, new QTableWidgetItem(accessionLineEdit.text()));
            if (cbct_out != "")
                tableWidget->setItem(row, 7, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 7, new QTableWidgetItem("无"));
            if (quduan != "")
                tableWidget->setItem(row, 8, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 8, new QTableWidgetItem("无"));
            if (touce != "")
                tableWidget->setItem(row, 9, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 9, new QTableWidgetItem("无"));
            tableWidget->item(row, 7)->setFlags(tableWidget->item(row, 7)->flags() & ~Qt::ItemIsEditable);
            tableWidget->item(row, 8)->setFlags(tableWidget->item(row, 7)->flags() & ~Qt::ItemIsEditable);
            tableWidget->item(row,9)->setFlags(tableWidget->item(row, 7)->flags() & ~Qt::ItemIsEditable);
            tableWidget->blockSignals(false);

            // 关闭对话框
            dialog.accept();
        }
        else {
            QMessageBox::information(&dialog, "提示", "数据添加失败，尝试更改数据后重试！");
        }

        });

    dialog.setLayout(&layout);
    dialog.exec();
}

void TableWidget::onDeleteButtonClicked()
{
    // 未选中任何行时直接返回
    if (tableWidget->selectedItems().isEmpty())
        return;

    //提示是否要关闭
    QDialog message;
    message.setWindowTitle("提示");
    //message.setStyleSheet("background-color: white;");

    QLabel label("您确定要删除这一行数据吗？");
    QPushButton okButton("确定");
    QPushButton cancelButton("取消");
    // 确定
    QObject::connect(&okButton, &QPushButton::clicked, [&]() {
        //数据库处理
        std::string condition = "";
        condition += "RIS=";
        condition += "'";
        condition += tableWidget->item(currentRow, 6)->text().toStdString();
        condition += "'";
        if (connection.deleteData(tableName, condition)) {
            //表格处理
            QList<QTableWidgetItem*> selectedItems = tableWidget->selectedItems();
            if (!selectedItems.isEmpty()) {
                int row = tableWidget->row(selectedItems.first());
                tableWidget->removeRow(row);
            }
        }

        message.accept();
        });

    QObject::connect(&cancelButton, &QPushButton::clicked, &message, &QDialog::reject);

    QFormLayout layout(&message);
    layout.addRow(&label);
    layout.addRow(&okButton, &cancelButton);
    message.setLayout(&layout);

    message.exec();
}

void TableWidget::showContextMenu(const QPoint& pos)
{
    if (tableWidget->rowCount() == 0)
        return;

    // 在鼠标右键位置显示上下文菜单
    QPoint globalPos = tableWidget->viewport()->mapToGlobal(pos);
    contextMenu->exec(globalPos);
}

void TableWidget::open() {
    // 未选中任何行时直接返回
    if (tableWidget->selectedItems().isEmpty())
        return;
    if (currentRow == -1)
        return;

    // 获取选中行的RIS数据，并在数据库中查找
    int row = tableWidget->selectedItems().first()->row();
    std::string condition = "";
    condition += "RIS = ";
    condition += "'";
    condition += (tableWidget->item(row, 6)->text().toStdString());
    condition += "'";
    std::vector<std::vector<std::string>> columnData = connection.selectData(tableName, "Name, OutputPath, QuduanPath, ToucePath", condition);
    const char* name = columnData[0][0].c_str();
    const char* cbctPath = columnData[0][1].c_str();
    const char* quDuanPath = columnData[0][2].c_str();
    const char* touCePath = columnData[0][3].c_str();
    PatientWindow* p = new PatientWindow(this, name, cbctPath, columnData[0][1] != "", quDuanPath, columnData[0][2] != "", touCePath, columnData[0][3] != "");
    p->show();
}

void TableWidget::edit() {
    // 未选中任何行时直接返回
    if (tableWidget->selectedItems().isEmpty())
        return;
    if (currentRow == -1)
        return;

    QDialog dialog;
    dialog.setWindowTitle("编辑病例信息");
    //dialog.setStyleSheet("background-color: white;"); 
     
    // 获取选中行的RIS数据，并在数据库中查找
    int row = tableWidget->selectedItems().first()->row();
    std::string condition = "";
    condition += "RIS = ";
    condition += "'";
    condition += (tableWidget->item(row, 6)->text().toStdString());
    condition += "'";
    std::vector<std::vector<std::string>> columnData = connection.selectData(tableName, "Name, Age, Sex, Date, Description, Modality, RIS, InputPath, QuduanPath, ToucePath", condition);

    QLabel nameLabel("姓名:");
    QLineEdit nameLineEdit(QString::fromStdString(columnData[0][0]));
    QLabel ageLabel("年龄:");
    QLineEdit ageLineEdit(QString::fromStdString(columnData[0][1]));
    QLabel genderLabel("性别:");
    QLineEdit genderLineEdit(QString::fromStdString(columnData[0][2]));
    QLabel dateLabel("检查日期:");
    QLineEdit dateLineEdit(QString::fromStdString(columnData[0][3]));
    QLabel descriptionLabel("描述:");
    QLineEdit descriptionLineEdit(QString::fromStdString(columnData[0][4]));
    QLabel deviceLabel("成像设备:");
    QLineEdit deviceLineEdit(QString::fromStdString(columnData[0][5]));
    QLabel accessionLabel("ACCESSION:");
    QLineEdit accessionLineEdit(QString::fromStdString(columnData[0][6]));

    QLabel cbctLabel("CBCT路径：");
    QPushButton cbctButton("选择...");
    QString cbctPath = QString::fromStdString(columnData[0][7]);
    if (!cbctPath.isEmpty()) {
        cbctButton.setText("更改...");
    }
    QLabel quduanLabel("曲断路径：");
    QPushButton quduanButton("选择...");
    QString quduanPath = QString::fromStdString(columnData[0][8]);
    if (!quduanPath.isEmpty()) {
        quduanButton.setText("更改...");
    }
    QLabel touceLabel("头侧路径：");
    QPushButton touceButton("选择...");
    QString toucePath = QString::fromStdString(columnData[0][9]);
    if (!toucePath.isEmpty()) {
        touceButton.setText("更改...");
    }

    QPushButton okButton("确定");
    QPushButton cancelButton("取消");

    QFormLayout layout(&dialog);
    layout.addRow(&nameLabel, &nameLineEdit);
    layout.addRow(&ageLabel, &ageLineEdit);
    layout.addRow(&genderLabel, &genderLineEdit);
    layout.addRow(&dateLabel, &dateLineEdit);
    layout.addRow(&descriptionLabel, &descriptionLineEdit);
    layout.addRow(&deviceLabel, &deviceLineEdit);
    layout.addRow(&accessionLabel, &accessionLineEdit);
    layout.addRow(&cbctLabel, &cbctButton);
    layout.addRow(&quduanLabel, &quduanButton);
    layout.addRow(&touceLabel, &touceButton);
    layout.addRow(&okButton, &cancelButton);

    QObject::connect(&cbctButton, &QPushButton::clicked, [&]() {
        cbctPath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("CBCT文件(*.dcm *.nii *.nii.gz)"));
        if (!cbctPath.isEmpty()) {
            std::string filePath = cbctPath.toStdString();
            // .dcm的话自动读一下数据
            if (filePath.substr(filePath.length() - 4) == ".dcm") {
                LoadInformation loader;
                if (nameLineEdit.text().isEmpty()) {
                    std::string name = loader.readName(loader.fileformat, filePath);
                    nameLineEdit.setText(QString::fromStdString(name));
                }
                if (ageLineEdit.text().isEmpty()) {
                    std::string age = loader.readAge(loader.fileformat, filePath);
                    ageLineEdit.setText(QString::fromStdString(age));
                }
                if (genderLineEdit.text().isEmpty()) {
                    std::string gender = loader.readSex(loader.fileformat, filePath);
                    genderLineEdit.setText(QString::fromStdString(gender));
                }
                if (dateLineEdit.text().isEmpty()) {
                    std::string date = loader.readSeriesDate(loader.fileformat, filePath);
                    dateLineEdit.setText(QString::fromStdString(date));
                }
                if (descriptionLineEdit.text().isEmpty()) {
                    std::string description = loader.readDescription(loader.fileformat, filePath);
                    descriptionLineEdit.setText(QString::fromStdString(description));
                }
                if (deviceLineEdit.text().isEmpty()) {
                    std::string device = loader.readModality(loader.fileformat, filePath);
                    deviceLineEdit.setText(QString::fromStdString(device));
                }
                if (accessionLineEdit.text().isEmpty()) {
                    std::string accession = loader.readAccessionNumber(loader.fileformat, filePath);
                    accessionLineEdit.setText(QString::fromStdString(accession));
                }
            }
            QMessageBox::information(nullptr, "提示", "CBCT路径选择成功");
            cbctButton.setText("更改...");
        }
        });

    QObject::connect(&quduanButton, &QPushButton::clicked, [&]() {
        quduanPath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("曲断文件(*.png *.jpg *.jpeg)"));
        if (!quduanPath.isEmpty()) {
            QMessageBox::information(nullptr, "提示", "曲断路径选择成功");
            quduanButton.setText("更改...");
        }
        });

    QObject::connect(&touceButton, &QPushButton::clicked, [&]() {
        toucePath = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("头侧文件(*.png *.jpg *.jpeg)"));
        if (!toucePath.isEmpty()) {
            QMessageBox::information(nullptr, "提示", "头侧路径选择成功");
            touceButton.setText("更改...");
        }
        });

    QObject::connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    QObject::connect(&okButton, &QPushButton::clicked, [&]() {
        
        // 获取文本框中的输入内容并进行相应处理
        std::string name = nameLineEdit.text().toStdString();
        std::string age = ageLineEdit.text().toStdString();
        std::string gender = genderLineEdit.text().toStdString();
        std::string date = dateLineEdit.text().toStdString();
        std::string description = descriptionLineEdit.text().toStdString();
        std::string device = deviceLineEdit.text().toStdString();
        std::string accession = accessionLineEdit.text().toStdString();

        std::string cbct = cbctPath.toStdString();
        std::string cbct_out = "";
        std::string quduan = quduanPath.toStdString();
        std::string touce = toucePath.toStdString();

        // 查看路径后三位，并判断路径指向的数据是.nii类型还是.dcm类型
        if (cbct.substr(cbct.length() - 4) == ".dcm") {
            // .dcm的话转成.nii
            // 读取DICOM数据
            std::string temp = cbct;
            size_t lastSlashPos = temp.rfind('/');
            if (lastSlashPos != std::string::npos) {
                temp = temp.substr(0, lastSlashPos);
            }
            vtkSmartPointer<vtkDICOMImageReader> dicomReader = vtkSmartPointer<vtkDICOMImageReader>::New();
            dicomReader->SetDirectoryName(temp.c_str());
            dicomReader->Update();

            // 创建vtkImageReslice过滤器并设置翻转参数
            vtkSmartPointer<vtkImageReslice> resliceFilter = vtkSmartPointer<vtkImageReslice>::New();
            resliceFilter->SetInputConnection(dicomReader->GetOutputPort());
            resliceFilter->SetResliceAxesDirectionCosines(-1, 0, 0, 0, -1, 0, 0, 0, -1); // 设置翻转方向

            // 将DICOM数据转换为NIfTI数据
            cbct_out = "D:/Data/CBCT/" + name + ".nii";
            vtkSmartPointer<vtkNIFTIImageWriter> niftiWriter = vtkSmartPointer<vtkNIFTIImageWriter>::New();
            niftiWriter->SetFileName(cbct_out.c_str());
            niftiWriter->SetInputConnection(resliceFilter->GetOutputPort());
            niftiWriter->Write();
        }
        else if (cbct.substr(cbct.length() - 4) == ".nii" || cbct.substr(cbct.length() - 7) == ".nii.gz") {
            //.nii路径直接传递就可以
            cbct_out = cbct;
        }

        //数据库处理
        std::string valueConditions = "Name = '" + name + "', Age = '" + age + "', Sex = '" + gender + "', Date = '" + date + "', Description = '" + description + "', Modality = '" + device + "', RIS = '" + accession + "', InputPath = '" + cbct + "', OutputPath = '" + cbct_out + "', QuduanPath = '" + quduan + "', ToucePath = '" + touce + "'";
        std::string fullColumnNames = "Name, Age, Sex, Date, Description, Modality, RIS, InputPath, OutputPath, QuduanPath, ToucePath";
        std::string fullValues = "'" + name + "', '" + age + "', '" + gender + "', '" + date + "', '" + description + "', '" + device + "', '" + accession + "', '" + cbct + "', '" + cbct_out + "', '" + quduan + "', '" + touce + "'";
        if (connection.updateData(tableName, valueConditions, condition)) {
            //表格处理
            int row = currentRow;
            tableWidget->blockSignals(true);
            tableWidget->setItem(row, 0, new QTableWidgetItem(nameLineEdit.text()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(ageLineEdit.text()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(genderLineEdit.text()));
            tableWidget->setItem(row, 3, new QTableWidgetItem(dateLineEdit.text()));
            tableWidget->setItem(row, 4, new QTableWidgetItem(descriptionLineEdit.text()));
            tableWidget->setItem(row, 5, new QTableWidgetItem(deviceLineEdit.text()));
            tableWidget->setItem(row, 6, new QTableWidgetItem(accessionLineEdit.text()));
            if (cbct_out != "")
                tableWidget->setItem(row, 7, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 7, new QTableWidgetItem("无"));
            if (quduan != "")
                tableWidget->setItem(row, 8, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 8, new QTableWidgetItem("无"));
            if (touce != "")
                tableWidget->setItem(row, 9, new QTableWidgetItem("有"));
            else
                tableWidget->setItem(row, 9, new QTableWidgetItem("无"));
            tableWidget->blockSignals(false);

            // 关闭对话框
            dialog.accept();
        }
        else {
            QMessageBox::information(&dialog, "提示", "数据更改失败，尝试更改数据后重试！");
        }


        });

    dialog.setLayout(&layout);
    dialog.exec();
}
