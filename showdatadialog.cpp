#include "ShowDataDialog.h"
#include "CsvParser.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QString>
#include <string>
#include <stdexcept> // for std::stod

ShowDataDialog::ShowDataDialog(QWidget *parent)
    : QDialog(parent) {
    setupUI(); // Call the setup function to initialize the UI
}

void ShowDataDialog::setStockData(const Stock &stockData) {
    // Update basic labels
    symbolLabel->setText(QString::fromStdString(stockData.symbol));
    openLabel->setText(QString::number(stockData.open, 'f', 2));
    highLabel->setText(QString::number(stockData.high, 'f', 2));
    lowLabel->setText(QString::number(stockData.low, 'f', 2));
    prevCloseLabel->setText(QString::number(stockData.prevClose, 'f', 2));
    ltpLabel->setText(QString::number(stockData.ltp, 'f', 2));

    // Handling indicativeClose (it can be "-" or a valid number)
    if (stockData.indicativeClose == "-") {
        indicativeCloseLabel->setText("N/A");
    } else {
        try {
            double indicativeClose = std::stod(stockData.indicativeClose); // Convert string to double
            indicativeCloseLabel->setText(QString::number(indicativeClose, 'f', 2));
        } catch (const std::invalid_argument &) {
            indicativeCloseLabel->setText("N/A");
        }
    }

    // Handle change (numeric or "-")
    try {
        if (stockData.change >= 0) {
            changeLabel->setText(QString::number(stockData.change, 'f', 2));
            changeLabel->setStyleSheet("color: green;");
            updateArrow(stockData.change); // Update arrow based on change
        } else {
            changeLabel->setText(QString::number(stockData.change, 'f', 2));
            changeLabel->setStyleSheet("color: red;");
            updateArrow(stockData.change); // Update arrow based on change
        }
    } catch (const std::invalid_argument &) {
        changeLabel->setText("N/A");
    }

    // Handle percentChange (numeric or "-")
    try {
        percentChangeLabel->setText(QString("%1%").arg(QString::number(stockData.percentChange, 'f', 2)));
        if (stockData.percentChange >= 0) {
            percentChangeLabel->setStyleSheet("color: green;");
        } else {
            percentChangeLabel->setStyleSheet("color: red;");
        }
    } catch (const std::invalid_argument &) {
        percentChangeLabel->setText("N/A");
    }

    // Handle other fields
    volumeSharesLabel->setText(QString::number(stockData.volumeShares));
    valueCroresLabel->setText(QString::number(stockData.valueCrores, 'f', 2));
    fiftyTwoWeekHighLabel->setText(QString::number(stockData.fiftyTwoWeekHigh, 'f', 2));
    fiftyTwoWeekLowLabel->setText(QString::number(stockData.fiftyTwoWeekLow, 'f', 2));
    thirtyDayChangeLabel->setText(QString::number(stockData.thirtyDayChange, 'f', 2));
    threeSixtyFiveDayChangeLabel->setText(QString::number(stockData.threeSixtyFiveDayChange, 'f', 2));
}


void ShowDataDialog::updateArrow(double change) {
    // Update the arrow based on the change value
    if (change >= 0) {
        arrowLabel->setPixmap(QPixmap(":/images/up_arrow.png").scaled(20, 20));
    } else {
        arrowLabel->setPixmap(QPixmap(":/images/down_arrow.png").scaled(20, 20));
    }
}


void ShowDataDialog::setupUI() {
    this->setWindowTitle("Stock Details");
    this->resize(500, 400); // Adjusted for a larger and better layout

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);  // Adding space between sections
    mainLayout->setContentsMargins(10, 10, 10, 10); // Set margins around the layout

    // ---- Basic Info Section ----
    QLabel *symbolTitle = new QLabel("Symbol:", this);
    symbolTitle->setStyleSheet("font-weight: bold; font-size: 14px;");
    symbolLabel = new QLabel(this);
    symbolLabel->setStyleSheet("font-size: 14px;");

    QHBoxLayout *basicInfoLayout = new QHBoxLayout();
    basicInfoLayout->setSpacing(10); // Ensure spacing between widgets
    basicInfoLayout->addWidget(symbolTitle);
    basicInfoLayout->addWidget(symbolLabel);

    // ---- Market Performance Section ----
    QVBoxLayout *performanceLayout = new QVBoxLayout();
    QLabel *performanceTitle = new QLabel("Market Performance", this);
    performanceTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #007BFF;");
    performanceLayout->addWidget(performanceTitle);

    openLabel = new QLabel(this);  // Initializing the labels
    highLabel = new QLabel(this);
    lowLabel = new QLabel(this);
    ltpLabel = new QLabel(this);
    prevCloseLabel = new QLabel(this); // Added missing initializations
    thirtyDayChangeLabel = new QLabel(this);
    threeSixtyFiveDayChangeLabel = new QLabel(this);

    QGridLayout *performanceGrid = new QGridLayout();
    performanceGrid->setHorizontalSpacing(20); // Add some space between grid elements
    performanceGrid->setVerticalSpacing(10); // Ensure vertical spacing as well

    performanceGrid->addWidget(new QLabel("Open:"), 0, 0);
    performanceGrid->addWidget(openLabel, 0, 1);
    performanceGrid->addWidget(new QLabel("High:"), 1, 0);
    performanceGrid->addWidget(highLabel, 1, 1);
    performanceGrid->addWidget(new QLabel("Low:"), 2, 0);
    performanceGrid->addWidget(lowLabel, 2, 1);
    performanceGrid->addWidget(new QLabel("LTP:"), 3, 0);
    performanceGrid->addWidget(ltpLabel, 3, 1);
    performanceGrid->addWidget(new QLabel("Prev Close:"), 4, 0);
    performanceGrid->addWidget(prevCloseLabel, 4, 1);
    performanceGrid->addWidget(new QLabel("30 Day Change:"), 5, 0);
    performanceGrid->addWidget(thirtyDayChangeLabel, 5, 1);
    performanceGrid->addWidget(new QLabel("365 Day Change:"), 6, 0);
    performanceGrid->addWidget(threeSixtyFiveDayChangeLabel, 6, 1);

    performanceLayout->addLayout(performanceGrid);

    // ---- Indicative Close Section ----
    QHBoxLayout *indicativeCloseLayout = new QHBoxLayout();
    QLabel *indicativeCloseTitle = new QLabel("Indicative Close:", this);
    indicativeCloseLayout->addWidget(indicativeCloseTitle);
    indicativeCloseLabel = new QLabel(this);
    indicativeCloseLayout->addWidget(indicativeCloseLabel);

    // ---- Change Indicators ----
    QLabel *changeTitle = new QLabel("Change Indicators", this);
    changeTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #FF5733;");
    arrowLabel = new QLabel(this);
    arrowLabel->setPixmap(QPixmap(":/images/up_arrow.png").scaled(20, 20));
    changeLabel = new QLabel(this);
    changeLabel->setStyleSheet("font-size: 14px;");

    QHBoxLayout *changeLayout = new QHBoxLayout();
    changeLayout->addWidget(changeTitle);
    changeLayout->addWidget(arrowLabel);
    changeLayout->addWidget(changeLabel);

    // ---- Percent Change Section ----
    QLabel *percentChangeTitle = new QLabel("Percent Change:", this);
    percentChangeLabel = new QLabel(this);
    percentChangeLabel->setStyleSheet("font-size: 14px;");
    QHBoxLayout *percentChangeLayout = new QHBoxLayout();
    percentChangeLayout->addWidget(percentChangeTitle);
    percentChangeLayout->addWidget(percentChangeLabel);

    // ---- 52W Stats Section ----
    QLabel *statsTitle = new QLabel("52 Week Stats", this);
    statsTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #28B463;");
    fiftyTwoWeekHighLabel = new QLabel(this); // Added initialization
    fiftyTwoWeekLowLabel = new QLabel(this);  // Added initialization
    QGridLayout *statsGrid = new QGridLayout();
    statsGrid->setHorizontalSpacing(20); // Add some space between grid elements
    statsGrid->setVerticalSpacing(10); // Ensure vertical spacing as well
    statsGrid->addWidget(statsTitle);
    statsGrid->addWidget(new QLabel("52W High:"), 1, 0);
    statsGrid->addWidget(fiftyTwoWeekHighLabel, 1, 1);
    statsGrid->addWidget(new QLabel("52W Low:"), 2, 0);
    statsGrid->addWidget(fiftyTwoWeekLowLabel, 2, 1);

    // ---- Volume & Value Stats Section ----
    QVBoxLayout *volumeValueLayout = new QVBoxLayout();
    QLabel *volumeTitle = new QLabel("Volume & Value", this);
    volumeTitle->setStyleSheet("font-weight: bold; font-size: 16px; color: #FFC107;");
    volumeSharesLabel = new QLabel(this); // Added initialization
    valueCroresLabel = new QLabel(this);  // Added initialization

    volumeValueLayout->addWidget(volumeTitle);
    volumeValueLayout->addWidget(new QLabel("Volume Shares:"));
    volumeValueLayout->addWidget(volumeSharesLabel);
    volumeValueLayout->addWidget(new QLabel("Value Crores:"));
    volumeValueLayout->addWidget(valueCroresLabel);

    // ---- Add all sections to main layout ----
    mainLayout->addLayout(basicInfoLayout);
    mainLayout->addLayout(performanceLayout);
    mainLayout->addLayout(indicativeCloseLayout);
    mainLayout->addLayout(changeLayout);
    mainLayout->addLayout(percentChangeLayout);
    mainLayout->addLayout(volumeValueLayout);
    mainLayout->addLayout(statsGrid);

    // ---- Final Layout Setup ----
    this->setLayout(mainLayout);

    // Optional: Adding padding for more space between sections (helps in a clean look)
    mainLayout->setContentsMargins(10, 10, 10, 10);  // Left, Top, Right, Bottom margin
    setStyleSheet("QLabel { margin-bottom: 5px; }");  // Adding space below each label for better alignment
}

