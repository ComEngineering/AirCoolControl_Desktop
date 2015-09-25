#include "AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(reject()));
}

AboutDialog::~AboutDialog()
{

}