#include "abouttextdialog.h"

AboutTextDialog::AboutTextDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	showText();
	connect(ui.closeButton, SIGNAL(clicked()), this, SLOT(closeText()));
}

AboutTextDialog::~AboutTextDialog()
{

}

void AboutTextDialog::showText()
{
	ui.aboutTeko->show();
}

void AboutTextDialog::closeText()
{
	ui.aboutTeko->close();
}

void AboutTextDialog::addText()
{
	ui.aboutTeko->setText(tr("Теко охуенная контора!!!"));
}
