#include "MainWindow2.h"
#include "Bag.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QLabel>
#include <QStatusBar>
#include <algorithm>
#include <QCloseEvent>
#include <QDateTime>
#include <QRegularExpression>

MainWindow2::MainWindow2(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);


    QHBoxLayout *tablesLayout = new QHBoxLayout;
    QVBoxLayout *validLayout = new QVBoxLayout;
    QVBoxLayout *invalidLayout = new QVBoxLayout;

    validLayout->addWidget(new QLabel("Корректные объекты"));
    validTable = new QTableWidget;
    validTable->setColumnCount(4);
    validTable->setHorizontalHeaderLabels({"Название", "Описание", "Слоты", "Макс вес"});
    validLayout->addWidget(validTable);

    invalidLayout->addWidget(new QLabel("Битые объекты (ошибки)"));
    invalidTable = new QTableWidget;
    invalidTable->setColumnCount(4);
    invalidTable->setHorizontalHeaderLabels({"Название", "Описание", "Слоты", "Макс вес"});
    invalidLayout->addWidget(invalidTable);

    tablesLayout->addLayout(validLayout);
    tablesLayout->addLayout(invalidLayout);
    mainLayout->addLayout(tablesLayout);


    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    loadJsonBtn = new QPushButton("Загрузить JSON");
    buttonLayout->addWidget(loadJsonBtn);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    statusBar()->showMessage("Готов. Нажмите 'Загрузить JSON'");

    connect(loadJsonBtn, &QPushButton::clicked, this, &MainWindow2::onLoadJson);
}

MainWindow2::~MainWindow2() {}

bool MainWindow2::isValid(const Bag &bag) {

    if (bag.getName().isEmpty() || bag.getDescription().isEmpty() ||
        bag.getSlotCountRaw().isEmpty() || bag.getMaxWeightRaw().isEmpty()) {
        return false;
    }


    bool slotOk;
    int slotValue = bag.getSlotCountRaw().toInt(&slotOk);
    if (!slotOk || slotValue <= 0) {
        return false;
    }


    bool weightOk;
    double weightValue = bag.getMaxWeightRaw().toDouble(&weightOk);

    if (!weightOk || weightValue <= 0.0) {
        return false;
    }

    if (!bag.getMaxWeightRaw().contains('.')) {
        return false;
    }

    QStringList parts = bag.getMaxWeightRaw().split('.');
    if (parts.size() != 2 || parts[1].isEmpty()) {
        return false;
    }

    return true;
}

void MainWindow2::sortValidBags() {
    std::sort(validBags.begin(), validBags.end(), [](const Bag &a, const Bag &b) {
        return QString::localeAwareCompare(a.getName(), b.getName()) > 0;
    });
}

void MainWindow2::displayTables() {
    sortValidBags();


    validTable->clearContents();
    validTable->setRowCount((int)validBags.size());
    for (int i = 0; i < (int)validBags.size(); ++i) {
        const Bag &b = validBags[i];
        validTable->setItem(i, 0, new QTableWidgetItem(b.getName()));
        validTable->setItem(i, 1, new QTableWidgetItem(b.getDescription()));
        validTable->setItem(i, 2, new QTableWidgetItem(b.getSlotCountRaw()));
        validTable->setItem(i, 3, new QTableWidgetItem(b.getMaxWeightRaw()));
    }


    invalidTable->clearContents();
    invalidTable->setRowCount((int)invalidBags.size());
    for (int i = 0; i < (int)invalidBags.size(); ++i) {
        const Bag &b = invalidBags[i];


        QTableWidgetItem *nameItem = new QTableWidgetItem(b.getName());
        QTableWidgetItem *descItem = new QTableWidgetItem(b.getDescription());
        QTableWidgetItem *slotItem = new QTableWidgetItem(b.getSlotCountRaw());
        QTableWidgetItem *weightItem = new QTableWidgetItem(b.getMaxWeightRaw());


        bool errorFound = false;


        if (!errorFound && b.getName().isEmpty()) {
            nameItem->setBackground(QColor(255, 100, 100));
            nameItem->setToolTip("Ошибка: название не может быть пустым");
            errorFound = true;
        }


        if (!errorFound && b.getDescription().isEmpty()) {
            descItem->setBackground(QColor(255, 100, 100));
            descItem->setToolTip("Ошибка: описание не может быть пустым");
            errorFound = true;
        }


        if (!errorFound) {
            bool slotError = false;
            if (b.getSlotCountRaw().isEmpty()) {
                slotError = true;
            } else {
                bool slotOk;
                int slotValue = b.getSlotCountRaw().toInt(&slotOk);
                if (!slotOk || slotValue <= 0) {
                    slotError = true;
                }
            }

            if (slotError) {
                slotItem->setBackground(QColor(255, 100, 100));
                slotItem->setToolTip("Ошибка: количество слотов должно быть целым положительным числом");
                errorFound = true;
            }
        }


        if (!errorFound) {
            bool weightError = false;
            if (b.getMaxWeightRaw().isEmpty()) {
                weightError = true;
            } else {
                bool weightOk;
                double weightValue = b.getMaxWeightRaw().toDouble(&weightOk);
                if (!weightOk || weightValue <= 0.0 || !b.getMaxWeightRaw().contains('.')) {
                    weightError = true;
                } else {
                    QStringList parts = b.getMaxWeightRaw().split('.');
                    if (parts.size() != 2 || parts[1].isEmpty()) {
                        weightError = true;
                    }
                }
            }

            if (weightError) {
                weightItem->setBackground(QColor(255, 100, 100));
                weightItem->setToolTip("Ошибка: вес должен быть положительным числом с точкой (например: 15.5)");
                errorFound = true;
            }
        }

        invalidTable->setItem(i, 0, nameItem);
        invalidTable->setItem(i, 1, descItem);
        invalidTable->setItem(i, 2, slotItem);
        invalidTable->setItem(i, 3, weightItem);
    }

    validTable->resizeColumnsToContents();
    invalidTable->resizeColumnsToContents();

    statusBar()->showMessage(QString("Всего: %1 | Корректные: %2 | Битые: %3")
                                 .arg(validBags.size() + invalidBags.size())
                                 .arg(validBags.size())
                                 .arg(invalidBags.size()));
}

void MainWindow2::clearAllData() {
    validBags.clear();
    invalidBags.clear();

    validTable->clearContents();
    validTable->setRowCount(0);

    invalidTable->clearContents();
    invalidTable->setRowCount(0);

    statusBar()->showMessage("Таблицы очищены", 2000);
}

void MainWindow2::loadEmptyFile(const QString &filepath, bool isValidTable) {
    QFile file(filepath);
    if (!file.exists()) return;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) return;

    QJsonArray arr = doc.array();

    if (arr.isEmpty()) {
        Bag emptyBag;
        if (isValidTable) {
            emptyBag.setName("=== ПУСТОЙ ФАЙЛ ДЛЯ БИТЫХ ===");
            emptyBag.setDescription("Нет битых объектов");
            emptyBag.setSlotCountRaw("-");
            emptyBag.setMaxWeightRaw("-");
            invalidBags.push_back(emptyBag);
        } else {
            emptyBag.setName("=== ПУСТОЙ ФАЙЛ ДЛЯ КОРРЕКТНЫХ ===");
            emptyBag.setDescription("Нет корректных объектов");
            emptyBag.setSlotCountRaw("-");
            emptyBag.setMaxWeightRaw("-");
            validBags.push_back(emptyBag);
        }
    }
}

void MainWindow2::onLoadJson() {
    QString filename = QFileDialog::getOpenFileName(this, "Выбрать JSON файл", "", "*.json");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        QMessageBox::warning(this, "Ошибка", "Файл должен содержать JSON массив");
        return;
    }

    QJsonArray arr = doc.array();
    std::vector<Bag> tempValid, tempInvalid;

    for (const auto &val : arr) {
        if (val.isObject()) {
            Bag bag = Bag::fromJson(val.toObject());
            if (isValid(bag))
                tempValid.push_back(bag);
            else
                tempInvalid.push_back(bag);
        }
    }

    clearAllData();

    validBags.insert(validBags.end(), tempValid.begin(), tempValid.end());
    invalidBags.insert(invalidBags.end(), tempInvalid.begin(), tempInvalid.end());

    QString baseName = QFileInfo(filename).baseName();
    QString dirPath = QFileInfo(filename).absolutePath();
    QString emptyInvalidFile, emptyValidFile;

    if (!tempValid.empty()) {
        emptyInvalidFile = dirPath + "/" + baseName + "_empty_invalid.json";
        QFile emptyInvalid(emptyInvalidFile);
        if (emptyInvalid.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emptyInvalid.close();
        }
        loadEmptyFile(emptyInvalidFile, true);
    }

    if (!tempInvalid.empty()) {
        emptyValidFile = dirPath + "/" + baseName + "_empty_valid.json";
        QFile emptyValid(emptyValidFile);
        if (emptyValid.open(QIODevice::WriteOnly | QIODevice::Text)) {
            emptyValid.close();
        }
        loadEmptyFile(emptyValidFile, false);
    }

    if (!tempInvalid.empty()) {
        QString brokenFilename = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + "_broken.json";
        QJsonArray invalidArray;
        for (const Bag &b : tempInvalid)
            invalidArray.append(b.toJson());

        QFile brokenFile(brokenFilename);
        if (brokenFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            brokenFile.write(QJsonDocument(invalidArray).toJson());
            brokenFile.close();
        }
    }

    displayTables();

    QMessageBox::information(this, "Загрузка завершена",
                             QString("Загружено объектов:\n✓ Корректных: %1\n✗ Битых: %2")
                                 .arg((int)tempValid.size())
                                 .arg((int)tempInvalid.size()));
}

void MainWindow2::closeEvent(QCloseEvent *event) {
    clearAllData();

    QFile autoSaveFile("auto_save.json");
    if (autoSaveFile.exists()) {
        autoSaveFile.remove();
    }

    event->accept();
}


