#pragma once
#ifndef TABLEWIDGETAPP_H
#define TABLEWIDGETAPP_H

#include <QWidget>
#include <QTableWidget>
#include "SQLServerConnection.cpp"
#include "LoadInformation.cpp"
#include <QtWidgets>

class TableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TableWidget(QWidget* parent = nullptr);

private slots:
    void showContextMenu(const QPoint& pos);
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void open(); 
    void edit();
    void onItemSelectionChanged();
    void onCellDoubleClicked(int row, int column);
    void onItemChanged(QTableWidgetItem* item);

private:
    SQLServerConnection connection;
    std::string tableName = "PatientInformation";
    std::vector<std::string> columnName = { "Name", "Age", "Sex" , "Date", "Description", "Modality", "RIS", "OutputPath", "QuduanPath", "ToucePath" };
    std::string columnNames = "Name, Age, Sex, Date, Description, Modality, RIS, OutputPath, QuduanPath, ToucePath";

    QTableWidget* tableWidget;
    QMenu* contextMenu;
    QLabel* fileLabel;
    std::string originText;

    int currentRow = -1;
    char* p;
};

#endif // TABLEWIDGETAPP_H
