#include "mainwindow.h"
#include "carddialog.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    setWindowTitle(u8"Таблица персонажей");
    setMinimumSize(800, 400);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Таблица
    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(8);
    QStringList headers;
    headers << u8"Тип" << u8"Имя" << u8"Стихия/Редкость" << u8"ХП" << u8"Мана/Урон"
            << u8"Шлем" << u8"Кираса" << u8"Сапоги";
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(tableWidget, &QTableWidget::doubleClicked,
            this, &MainWindow::onTableDoubleClicked);

    mainLayout->addWidget(tableWidget);


    loadButton = new QPushButton(u8"Загрузить", this);
    loadButton->setFixedWidth(200);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(loadButton);
    buttonLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
}

void MainWindow::onLoadButtonClicked() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    u8"Выберите файл с данными", "", u8"Текстовые файлы (*.txt);;Все файлы (*)");

    if (!filename.isEmpty()) {
        loadFromFile(filename);
    }
}

void MainWindow::loadFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, u8"Ошибка", u8"Не удалось открыть файл!");
        return;
    }


    for (Persistent* p : characters) {
        delete p;
    }
    characters.clear();

    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(' ');
        if (parts.size() < 8) {
            continue;
        }

        int code = parts[0].toInt();
        QString name = parts[1];

        if (code == 1) { // Маг
            QString element = parts[2];
            int mana = parts[3].toInt();
            int health = parts[4].toInt();
            int helmet = parts[5].toInt();
            int cuirass = parts[6].toInt();
            int boots = parts[7].toInt();

            characters.append(new MagP(name, element, mana, health, Armor(helmet, cuirass, boots)));

        } else if (code == 2) { // Враг
            QString rarity = parts[2];
            int damage = parts[3].toInt();
            int health = parts[4].toInt();
            int helmet = parts[5].toInt();
            int cuirass = parts[6].toInt();
            int boots = parts[7].toInt();

            characters.append(new VragP(name, rarity, damage, health, Armor(helmet, cuirass, boots)));
        }
    }

    file.close();
    updateTable();
}

void MainWindow::updateTable() {
    tableWidget->setRowCount(characters.size());

    for (int i = 0; i < characters.size(); ++i) {
        Persistent* p = characters[i];

        tableWidget->setItem(i, 0, new QTableWidgetItem(p->getType()));
        tableWidget->setItem(i, 1, new QTableWidgetItem(p->getName()));

        if (MagP* mag = dynamic_cast<MagP*>(p)) {
            tableWidget->setItem(i, 2, new QTableWidgetItem(mag->getElement()));
            tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(p->getHealth())));
            tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(mag->getMana())));
        } else if (VragP* vrag = dynamic_cast<VragP*>(p)) {
            tableWidget->setItem(i, 2, new QTableWidgetItem(vrag->getRarity()));
            tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(p->getHealth())));
            tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(vrag->getDamage())));
        }

        tableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(p->getArmor().helmet)));
        tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(p->getArmor().cuirass)));
        tableWidget->setItem(i, 7, new QTableWidgetItem(QString::number(p->getArmor().boots)));
    }

    tableWidget->resizeColumnsToContents();
}

void MainWindow::onTableDoubleClicked(const QModelIndex& index) {
    if (index.row() >= 0 && index.row() < characters.size()) {

        CardDialog* dialog = new CardDialog(characters[index.row()], this);


        connect(dialog, &CardDialog::characterDeleted,
                this, &MainWindow::onCharacterDeleted);

        dialog->exec();
        delete dialog;
    }
}


void MainWindow::onCharacterDeleted(Persistent* character) {

    int index = characters.indexOf(character);
    if (index != -1) {

        characters.removeAt(index);

        delete character;

        updateTable();

        qDebug() << "Персонаж удален из таблицы";
    }
}

MainWindow::~MainWindow() {
    for (Persistent* p : characters) {
        delete p;
    }
}
