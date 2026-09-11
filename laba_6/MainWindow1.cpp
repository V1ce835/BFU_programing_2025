#include "MainWindow1.h"
#include "Bag.h"
#include <QGridLayout>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QGridLayout *layout = new QGridLayout(central);

    layout->addWidget(new QLabel("Название:"), 0, 0);
    nameEdit = new QLineEdit;
    layout->addWidget(nameEdit, 0, 1);

    layout->addWidget(new QLabel("Описание:"), 1, 0);
    descEdit = new QLineEdit;
    layout->addWidget(descEdit, 1, 1);

    layout->addWidget(new QLabel("Кол-во слотов:"), 2, 0);
    slotEdit = new QSpinBox;
    slotEdit->setMinimum(0);
    layout->addWidget(slotEdit, 2, 1);

    layout->addWidget(new QLabel("Макс. вес:"), 3, 0);
    weightEdit = new QDoubleSpinBox;
    weightEdit->setMinimum(0);
    weightEdit->setSingleStep(0.5);
    layout->addWidget(weightEdit, 3, 1);

    saveBtn = new QPushButton("Сохранить в JSON");
    loadTxtBtn = new QPushButton("Загрузить из .TXT");
    statusLabel = new QLabel("Готов");

    layout->addWidget(saveBtn, 4, 0);
    layout->addWidget(loadTxtBtn, 4, 1);
    layout->addWidget(statusLabel, 5, 0, 1, 2);

    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onSaveToJson);
    connect(loadTxtBtn, &QPushButton::clicked, this, &MainWindow::onLoadFromTxt);
}

MainWindow::~MainWindow() {}

void MainWindow::onSaveToJson() {
    Bag bag(nameEdit->text(), descEdit->text(), slotEdit->value(), weightEdit->value());

    QString filename = QFileDialog::getSaveFileName(this, "Сохранить JSON", "", "*.json");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        statusLabel->setText("Ошибка открытия файла");
        return;
    }

    QJsonArray bagsArray;
    file.seek(0);
    QByteArray data = file.readAll();
    if (!data.isEmpty()) {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        if (doc.isArray()) {
            bagsArray = doc.array();
        }
    }

    bagsArray.append(bag.toJson());

    file.resize(0);
    QJsonDocument saveDoc(bagsArray);
    file.write(saveDoc.toJson());
    file.close();

    statusLabel->setText("Сохранено");
}

void MainWindow::onLoadFromTxt() {
    QString filename = QFileDialog::getOpenFileName(this, "Выбрать TXT файл", "", "*.txt");
    if (filename.isEmpty()) return;
    loadFromFile(filename, true);
}

void MainWindow::loadFromFile(const QString &filename, bool isTxt) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QTextStream stream(&file);
    QString line = stream.readLine();
    file.close();

    if (isTxt) {
        // Формат: название/описание/кол-во_слотов/макс_вес
        QStringList parts = line.split('/');
        if (parts.size() >= 4) {
            nameEdit->setText(parts[0]);
            descEdit->setText(parts[1]);
            slotEdit->setValue(parts[2].toInt());
            weightEdit->setValue(parts[3].toDouble());
            statusLabel->setText("Загружено из TXT");
        } else {
            QMessageBox::warning(this, "Ошибка", "Неверный формат TXT (нужно 4 поля через /)");
        }
    }
}
