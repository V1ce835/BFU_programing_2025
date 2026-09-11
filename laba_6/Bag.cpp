#include "Bag.h"
#include <QRegularExpression>

Bag::Bag() {}

QString Bag::getName() const { return name; }
QString Bag::getDescription() const { return description; }
QString Bag::getSlotCountRaw() const { return slotCount; }
QString Bag::getMaxWeightRaw() const { return maxWeight; }

void Bag::setName(const QString &name) { this->name = name; }
void Bag::setDescription(const QString &description) { this->description = description; }
void Bag::setSlotCountRaw(const QString &slotCount) { this->slotCount = slotCount; }
void Bag::setMaxWeightRaw(const QString &maxWeight) { this->maxWeight = maxWeight; }

QJsonObject Bag::toJson() const {
    QJsonObject obj;
    obj["name"] = name;
    obj["description"] = description;
    obj["slotCount"] = slotCount;
    obj["maxWeight"] = maxWeight;
    return obj;
}

Bag Bag::fromJson(const QJsonObject &obj) {
    Bag bag;
    bag.name = obj["name"].toString();
    bag.description = obj["description"].toString();
    bag.slotCount = obj["slotCount"].toString();
    bag.maxWeight = obj["maxWeight"].toString();
    return bag;
}
