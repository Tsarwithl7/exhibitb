#include <QtWidgets>
#include <QTableWidget>

//#include "SQLServerConnection.cpp"

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    WelcomeWidget(QWidget* parent = nullptr);
    ~WelcomeWidget();

public:
    void refresh();

private:

    QLabel* label;
    QPushButton* CBCTButton;
    QPushButton* quduanButton;
    QPushButton* touceButton;
    QPushButton* editButton;
    QTableWidget* tableWidget;
};


