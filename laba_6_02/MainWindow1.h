#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveToJson();
    void onLoadFromTxt();

private:
    QLineEdit *nameEdit;
    QLineEdit *descEdit;
    QSpinBox *slotEdit;
    QDoubleSpinBox *weightEdit;
    QPushButton *saveBtn;
    QPushButton *loadTxtBtn;
    QLabel *statusLabel;

    void loadFromFile(const QString &filename, bool isTxt);
};

#endif // MAINWINDOW1_H
