#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QTableWidget>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include "showdatadialog.h"
#include "CsvParser.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Stock Visualizer"));
    this->Logo =  new QIcon(QPixmap(":/images/stock_visualizer_logo.png"));
    setWindowIcon(*this->Logo);
    // Set up the table widget and headers
    ui->tableWidget->setColumnCount(16);  // 16 columns for all the data
    ui->tableWidget->setHorizontalHeaderLabels({
        "Symbol", "Open", "High", "Low", "Prev. Close", "LTP", "Indicative Close", "Change", "% Change",
        "Volume (shares)", "Value (₹ Crores)", "52W High", "52W Low", "30D % Change", "365D % Change", "Date"
    });
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);   // To select entire rows
    ui->tableWidget->setSelectionMode(QAbstractItemView::ExtendedSelection); // For multiple selection
    ui->tableWidget->installEventFilter(this);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Create layout for the central widget
    QWidget* centralWidget = new QWidget(this);  // Create a central widget

    // Create vertical layout for the central widget
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);  // Set layout on the central widget

    // Create the table, button, and process button
    layout->addWidget(ui->tableWidget);  // Add the table widget
    QHBoxLayout *HButtonLayout  = new QHBoxLayout();
    HButtonLayout->addWidget(ui->pushButton);   // Add the button (Load CSV)
    // Create and add the process button
    QPushButton* clearButton =  new QPushButton("Clear Data", this);
    HButtonLayout->addWidget(clearButton);
    HButtonLayout->setStretch(0,0);
    HButtonLayout->setStretch(1,0);
    HButtonLayout->setStretch(2,0);
    layout->addLayout(HButtonLayout,0);
    layout->setStretch(0, 1);  // Table should take most of the space
    layout->setStretch(1, 1);  // Load CSV button should have fixed size
    // layout->setStretch(2, 0);  // Process button should also have fixed size
    // Set the central widget
    setCentralWidget(centralWidget);

    // Connect the process button to open the new window (dialog)
    connect(ui->pushButton,&QPushButton::clicked, this, &MainWindow::onLoadCsv);
    connect(clearButton,&QPushButton::clicked, this, &MainWindow::onClearButton);
    connect(ui->tableWidget, &QTableWidget::cellDoubleClicked , this, &MainWindow::ShowContent);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadCsv() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open CSV", "", "CSV Files (*.csv)");
    if (!filePath.isEmpty()) {
        try {
            auto stocks = CsvParser::parse(filePath.toStdString());
            for (const auto& stock : stocks) {
                int row = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(row);
                ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(stock.symbol)));
                ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(stock.open, 'f', 2)));
                ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(stock.high, 'f', 2)));
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(stock.low, 'f', 2)));
                ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(stock.prevClose, 'f', 2)));
                ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(stock.ltp, 'f', 2)));
                ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(stock.indicativeClose)));
                ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(stock.change, 'f', 2)));
                ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(stock.percentChange, 'f', 2)));
                ui->tableWidget->setItem(row, 9, new QTableWidgetItem(QString::number(stock.volumeShares)));
                ui->tableWidget->setItem(row, 10, new QTableWidgetItem(QString::number(stock.valueCrores, 'f', 2)));
                ui->tableWidget->setItem(row, 11, new QTableWidgetItem(QString::number(stock.fiftyTwoWeekHigh, 'f', 2)));
                ui->tableWidget->setItem(row, 12, new QTableWidgetItem(QString::number(stock.fiftyTwoWeekLow, 'f', 2)));
                ui->tableWidget->setItem(row, 13, new QTableWidgetItem(QString::number(stock.thirtyDayChange, 'f', 2)));
                ui->tableWidget->setItem(row, 14, new QTableWidgetItem(QString::number(stock.threeSixtyFiveDayChange, 'f', 2)));
                ui->tableWidget->setItem(row, 15, new QTableWidgetItem("28-Dec-2023")); // Example Date
            }

            ui->tableWidget->viewport()->update();  // Refresh the table view
            ui->pushButton->setEnabled(false);
        }
        catch (const std::exception& e) {
            QMessageBox::critical(this, "Error", e.what());
        }
    }

}

void MainWindow::onProcessData()
{
    // Create a new dialog window (use QDialog instead of QWidget)
    QDialog *processWindow = new QDialog(this);
    processWindow->setWindowTitle("Process Data");

    // Create a layout for the dialog
    QVBoxLayout *processLayout = new QVBoxLayout(processWindow);

    // Add content to the dialog (e.g., a label and a button)
    QLabel *label = new QLabel("Processing your data...", processWindow);
    processLayout->addWidget(label);

    // Show the dialog
    processWindow->resize(400, 200);
    processWindow->exec();  // Opens the dialog as a modal window
}


void MainWindow::onClearButton()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->viewport()->update();
    ui->pushButton->setEnabled(true);
}


void MainWindow::onClearSelection() {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm Delete", "Are you sure you want to delete selected rows?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Proceed with clearing rows
        // Get the currently selected row
        QModelIndexList selectedRows = ui->tableWidget->selectionModel()->selectedRows();

        if (!selectedRows.isEmpty()) {
            // Sort the selected rows in descending order to avoid row shifting issues
            std::sort(selectedRows.begin(), selectedRows.end(), [](const QModelIndex &a, const QModelIndex &b) {
                return a.row() > b.row(); // Compare rows in descending order
            });

            // Remove each selected row
            for (const QModelIndex &index : selectedRows) {
                ui->tableWidget->removeRow(index.row());
            }
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Delete) {
        onClearSelection(); // Call your clearSelectedRows or clearSelectedCells function
    } else {
        QMainWindow::keyPressEvent(event); // Pass other keys to the default handler
    }
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->tableWidget && event->type() == QEvent::KeyPress) {
       auto *keyEvent = dynamic_cast<QKeyEvent *>(event);
        if (keyEvent->key() == Qt::Key_Delete) {
            onClearSelection(); // Call your function to clear selected rows or cells
            return true; // Event handled
        }
    }
    return QMainWindow::eventFilter(watched, event); // Pass unhandled events to the base class
}

void MainWindow::ShowContent(int row, int column)
{
    Q_UNUSED(column); // We only need the row for this implementation

    // Extract the data from the selected row
    Stock stockData;
    stockData.symbol = ui->tableWidget->item(row, 0)->text().toStdString();
    stockData.open = ui->tableWidget->item(row, 1)->text().toDouble();
    stockData.high = ui->tableWidget->item(row, 2)->text().toDouble();
    stockData.low = ui->tableWidget->item(row, 3)->text().toDouble();
    stockData.prevClose = ui->tableWidget->item(row, 4)->text().toDouble();
    stockData.ltp = ui->tableWidget->item(row, 5)->text().toDouble();
    stockData.indicativeClose = ui->tableWidget->item(row, 6)->text().toStdString();
    stockData.change = ui->tableWidget->item(row, 7)->text().toDouble();
    stockData.percentChange = ui->tableWidget->item(row, 8)->text().toDouble();
    stockData.volumeShares = ui->tableWidget->item(row, 9)->text().toLong();
    stockData.valueCrores = ui->tableWidget->item(row, 10)->text().toDouble();
    stockData.fiftyTwoWeekHigh = ui->tableWidget->item(row, 11)->text().toDouble();
    stockData.fiftyTwoWeekLow = ui->tableWidget->item(row, 12)->text().toDouble();
    stockData.thirtyDayChange = ui->tableWidget->item(row, 13)->text().toDouble();
    stockData.threeSixtyFiveDayChange = ui->tableWidget->item(row, 14)->text().toDouble();
    ShowDataDialog *DataDlg  = new ShowDataDialog();
    this->openDialogs.append(DataDlg);
    connect(DataDlg, &QDialog::finished, DataDlg, &QObject::deleteLater);
    DataDlg->setStockData(stockData);
    DataDlg->show();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    // Optionally, you can show a confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Quit", "Are you sure you want to quit?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        event->accept();  // Close the main window and quit the application
    } else {
        event->ignore();  // Prevent the main window from closing
        return;
    }

    // Check if there are any open modeless dialogs
    foreach (QWidget *widget, QApplication::topLevelWidgets())
    {
        if (QDialog *dialog = qobject_cast<QDialog *>(widget))
        {
            dialog->close();  // Close the dialog if open
        }
    }
}

