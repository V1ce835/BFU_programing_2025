#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QVector>
#include "persistent.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTableWidget* tableWidget;
    QPushButton* loadButton;
    QVector<Persistent*> characters;

    void setupUI();
    void loadFromFile(const QString& filename);
    void updateTable();

private slots:
    void onLoadButtonClicked();
    void onTableDoubleClicked(const QModelIndex& index);
    void onCharacterDeleted(Persistent* character);

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
