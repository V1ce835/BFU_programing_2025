#ifndef BAG_H
#define BAG_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>

class Bag
{
public:
    Bag();
    QString getName() const;
    QString getDescription() const;
    QString getSlotCountRaw() const;
    QString getMaxWeightRaw() const;


    void setName(const QString &name);
    void setDescription(const QString &description);
    void setSlotCountRaw(const QString &slotCount);
    void setMaxWeightRaw(const QString &maxWeight);

    QJsonObject toJson() const;
    static Bag fromJson(const QJsonObject &obj);

private:
    QString name;
    QString description;
    QString slotCount;
    QString maxWeight;
};

#endif
