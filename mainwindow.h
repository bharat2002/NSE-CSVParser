#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QIcon>
#include "showdatadialog.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void onLoadCsv();
    void onProcessData();
    void onClearButton();
    void onClearSelection();
    bool eventFilter(QObject *watched, QEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void ShowContent(int row, int column);
    void closeEvent(QCloseEvent *event) override;
    QList<ShowDataDialog*> openDialogs;
private:
    QIcon *Logo= nullptr;
    Ui::MainWindow *ui;
    QTableWidget* tableWidget;
    QPushButton* loadButton;

};
#endif // MAINWINDOW_H
