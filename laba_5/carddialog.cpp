#include "carddialog.h"
#include "persistent.h"
#include <QPainter>

CardDialog::CardDialog(Persistent* ch, QWidget* parent)
    : QDialog(parent), character(ch) {

    setWindowTitle(u8"Карточка персонажа");
    setMinimumSize(350, 500);
    setMaximumSize(400, 600);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);


    QWidget* infoWidget = new QWidget();
    QVBoxLayout* infoLayout = new QVBoxLayout(infoWidget);
    infoLayout->setSpacing(10);

    // Имя персонажа
    nameLabel = new QLabel(character->getName());
    QFont nameFont = nameLabel->font();
    nameFont.setPointSize(18);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    nameLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(nameLabel);

    // Разделительная линия
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    infoLayout->addWidget(line);

    // Сетка с характеристиками
    QGridLayout* statsLayout = new QGridLayout();
    statsLayout->setSpacing(10);
    statsLayout->setHorizontalSpacing(20);

    int row = 0;

    // ХП
    QLabel* hpTitle = new QLabel(u8"ХП:");
    hpTitle->setFont(QFont("Arial", 12, QFont::Bold));
    healthLabel = new QLabel(QString::number(character->getHealth()));
    healthLabel->setFont(QFont("Arial", 12));
    if (character->getType() == u8"Маг") {
        healthLabel->setStyleSheet("color: green; font-weight: bold;");
    } else {
        healthLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    statsLayout->addWidget(hpTitle, row, 0);
    statsLayout->addWidget(healthLabel, row, 1);
    row++;

    // Специфичные поля (Мана для мага или Урон для врага)
    if (MagP* mag = dynamic_cast<MagP*>(character)) {
        // Мана
        QLabel* manaTitle = new QLabel(u8"Мана:");
        manaTitle->setFont(QFont("Arial", 12, QFont::Bold));
        manaOrDamageLabel = new QLabel(QString::number(mag->getMana()));
        manaOrDamageLabel->setFont(QFont("Arial", 12));
        manaOrDamageLabel->setStyleSheet("color: blue; font-weight: bold;");
        statsLayout->addWidget(manaTitle, row, 0);
        statsLayout->addWidget(manaOrDamageLabel, row, 1);
        row++;

        // Стихия
        QLabel* elementTitle = new QLabel(u8"Стихия:");
        elementTitle->setFont(QFont("Arial", 12, QFont::Bold));
        elementOrRarityLabel = new QLabel(mag->getElement());
        elementOrRarityLabel->setFont(QFont("Arial", 12));
        QString elementColor = "black";
        if (mag->getElement() == u8"огонь") elementColor = "red";
        else if (mag->getElement() == u8"вода") elementColor = "blue";
        else if (mag->getElement() == u8"земля") elementColor = "brown";
        elementOrRarityLabel->setStyleSheet(QString("color: %1;").arg(elementColor));
        statsLayout->addWidget(elementTitle, row, 0);
        statsLayout->addWidget(elementOrRarityLabel, row, 1);
        row++;

    } else if (VragP* vrag = dynamic_cast<VragP*>(character)) {
        // Урон
        QLabel* damageTitle = new QLabel(u8"Урон:");
        damageTitle->setFont(QFont("Arial", 12, QFont::Bold));
        manaOrDamageLabel = new QLabel(QString::number(vrag->getDamage()));
        manaOrDamageLabel->setFont(QFont("Arial", 12));
        statsLayout->addWidget(damageTitle, row, 0);
        statsLayout->addWidget(manaOrDamageLabel, row, 1);
        row++;

        // Редкость
        QLabel* rarityTitle = new QLabel(u8"Редкость:");
        rarityTitle->setFont(QFont("Arial", 12, QFont::Bold));
        elementOrRarityLabel = new QLabel(vrag->getRarity());
        elementOrRarityLabel->setFont(QFont("Arial", 12));
        QString rarityColor = "black";
        if (vrag->getRarity() == u8"Редкий") rarityColor = "blue";
        else if (vrag->getRarity() == u8"Обычный") rarityColor = "green";
        else if (vrag->getRarity() == u8"Легенда") rarityColor = "gold";
        elementOrRarityLabel->setStyleSheet(QString("color: %1; font-weight: bold;").arg(rarityColor));
        statsLayout->addWidget(rarityTitle, row, 0);
        statsLayout->addWidget(elementOrRarityLabel, row, 1);
        row++;
    }

    // Броня
    QLabel* armorTitle = new QLabel(u8"Броня:");
    armorTitle->setFont(QFont("Arial", 12, QFont::Bold));
    armorLabel = new QLabel(character->getArmor().toString());
    armorLabel->setFont(QFont("Arial", 12));
    statsLayout->addWidget(armorTitle, row, 0);
    statsLayout->addWidget(armorLabel, row, 1);
    row++;

    infoLayout->addLayout(statsLayout);


    QHBoxLayout* armorDetailsLayout = new QHBoxLayout();
    armorDetailsLayout->setSpacing(10);

    infoLayout->addLayout(armorDetailsLayout);

    mainLayout->addWidget(infoWidget);


    loadStaticImage();
    mainLayout->addWidget(imageLabel, 0, Qt::AlignCenter);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(20);

    printButton = new QPushButton(u8"Печать");
    cancelButton = new QPushButton(u8"Отмена");

    printButton->setFixedWidth(100);
    cancelButton->setFixedWidth(100);

    printButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 8px; border-radius: 5px; }"
                               "QPushButton:hover { background-color: #45a049; }");
    cancelButton->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 8px; border-radius: 5px; }"
                                "QPushButton:hover { background-color: #da190b; }");

    connect(printButton, &QPushButton::clicked, this, &CardDialog::onPrint);
    connect(cancelButton, &QPushButton::clicked, this, &CardDialog::onCancel);

    buttonLayout->addStretch();
    buttonLayout->addWidget(printButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);
}

void CardDialog::loadStaticImage() {
    imageLabel = new QLabel();


    QString imagePath;

    if (character->getType() == u8"Маг") {
        imagePath = "C:/Users/Admin/source/repos/laba_5/build/QT6-Debug/debug/basePath/mag_res/mag.png.png";
    } else {
        imagePath = "C:/Users/Admin/source/repos/laba_5/build/QT6-Debug/debug/basePath/vrag_res/skel.png.png";
    }


    QPixmap pixmap;

    if (QFile::exists(imagePath)) {
        pixmap.load(imagePath);
    }


    if (pixmap.isNull()) {
        pixmap = QPixmap(200, 200);
        pixmap.fill(Qt::lightGray);
        QPainter painter(&pixmap);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(pixmap.rect(), Qt::AlignCenter,
                         character->getType() + "\n" + character->getName());
    }

    imageLabel->setPixmap(pixmap.scaled(180, 180, Qt::KeepAspectRatio));
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border: 2px solid #ccc; border-radius: 10px; padding: 5px;");
}

void CardDialog::onPrint() {
    emit characterDeleted(character);
    accept();
}

void CardDialog::onCancel() {
    reject();
}

CardDialog::~CardDialog() {}
