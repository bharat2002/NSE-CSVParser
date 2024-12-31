#ifndef SHOWDATADIALOG_H
#define SHOWDATADIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

struct Stock; // Forward declaration of the Stock struct

class ShowDataDialog : public QDialog {
    Q_OBJECT

public:
    explicit ShowDataDialog(QWidget *parent = nullptr);

    // Function to set stock data
    void setStockData(const Stock &stockData);

private:
    // Private helper functions
    void setupUI();
    void updateArrow(double change);

    // UI components
    QLabel *symbolLabel;
    QLabel *openLabel;
    QLabel *highLabel;
    QLabel *lowLabel;
    QLabel *prevCloseLabel;
    QLabel *ltpLabel;
    QLabel *indicativeCloseLabel;
    QLabel *changeLabel;
    QLabel *percentChangeLabel;
    QLabel *volumeSharesLabel;
    QLabel *valueCroresLabel;
    QLabel *fiftyTwoWeekHighLabel;
    QLabel *fiftyTwoWeekLowLabel;
    QLabel *thirtyDayChangeLabel;
    QLabel *threeSixtyFiveDayChangeLabel;
    QLabel *arrowLabel; // For change arrow
};

#endif // SHOWDATADIALOG_H
