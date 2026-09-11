#ifndef PERSISTENT_H
#define PERSISTENT_H

#include <QString>
#include <QVector>
#include <QDebug>
#include <QWidget>

struct Armor {
    int helmet;
    int cuirass;
    int boots;

    Armor(int h = 0, int c = 0, int b = 0) : helmet(h), cuirass(c), boots(b) {}

    QString toString() const {
        return QString("<%1/%2/%3>").arg(helmet).arg(cuirass).arg(boots);
    }
};


class CardDialog;

class Persistent {
protected:
    QString name;
    int health;
    Armor armor;

public:
    Persistent(const QString& n = "", int h = 0, const Armor& a = Armor())
        : name(n), health(h), armor(a) {}

    virtual ~Persistent() {}

    QString getName() const { return name; }
    int getHealth() const { return health; }
    Armor getArmor() const { return armor; }

    virtual QString getType() const = 0;
    virtual void craft(QWidget* parent = nullptr) = 0;
    virtual QString toString() const = 0;
};

class MagP : public Persistent {
private:
    QString element;
    int mana;

public:
    MagP(const QString& n = "", const QString& e = "", int m = 0, int h = 0, const Armor& a = Armor())
        : Persistent(n, h, a), element(e), mana(m) {}

    QString getElement() const { return element; }
    int getMana() const { return mana; }

    QString getType() const override { return "Маг"; }

    void craft(QWidget* parent = nullptr) override;

    QString toString() const override {
        return QString("%1|%2|%3|%4|%5|%6|%7|%8")
        .arg(1)
            .arg(name)
            .arg(element)
            .arg(mana)
            .arg(health)
            .arg(armor.helmet)
            .arg(armor.cuirass)
            .arg(armor.boots);
    }
};

class VragP : public Persistent {
private:
    QString rarity;
    int damage;

public:
    VragP(const QString& n = "", const QString& r = "", int d = 0, int h = 0, const Armor& a = Armor())
        : Persistent(n, h, a), rarity(r), damage(d) {}

    QString getRarity() const { return rarity; }
    int getDamage() const { return damage; }

    QString getType() const override { return "Враг"; }

    void craft(QWidget* parent = nullptr) override;

    QString toString() const override {
        return QString("%1|%2|%3|%4|%5|%6|%7|%8")
        .arg(2)
            .arg(name)
            .arg(rarity)
            .arg(damage)
            .arg(health)
            .arg(armor.helmet)
            .arg(armor.cuirass)
            .arg(armor.boots);
    }
};

#endif // PERSISTENT_H
