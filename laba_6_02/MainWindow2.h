#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <vector>
#include "Bag.h"

class MainWindow2 : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onLoadJson();

private:
    QTableWidget *validTable;
    QTableWidget *invalidTable;
    QPushButton *loadJsonBtn;
    QString getErrorField(const Bag &bag);

    std::vector<Bag> validBags;
    std::vector<Bag> invalidBags;

    void displayTables();
    void sortValidBags();
    bool isValid(const Bag &bag);
    void clearAllData();
    void loadEmptyFile(const QString &filepath, bool isValidTable);
};

#endif
