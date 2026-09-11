#include "persistent.h"
#include "carddialog.h"

void MagP::craft(QWidget* parent) {
    CardDialog* dialog = new CardDialog(this, parent);
    dialog->exec();
    delete dialog;
}

void VragP::craft(QWidget* parent) {
    CardDialog* dialog = new CardDialog(this, parent);
    dialog->exec();
    delete dialog;
}
