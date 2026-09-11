#ifndef CARDDIALOG_H
#define CARDDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QPixmap>
#include <QFile>
#include <QFrame>

class Persistent;
class MagP;
class VragP;

class CardDialog : public QDialog {
    Q_OBJECT

private:
    Persistent* character;
    QLabel* imageLabel;
    QLabel* nameLabel;
    QLabel* healthLabel;
    QLabel* manaOrDamageLabel;
    QLabel* elementOrRarityLabel;
    QLabel* armorLabel;
    QPushButton* printButton;
    QPushButton* cancelButton;

    void loadStaticImage();

private slots:
    void onPrint();
    void onCancel();

signals:
    void characterDeleted(Persistent* character);

public:
    CardDialog(Persistent* ch, QWidget* parent = nullptr);
    ~CardDialog();
};

#endif // CARDDIALOG_H
