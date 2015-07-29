#include "NewParameterNameDialog.h"

NewParameterNameDialog::NewParameterNameDialog(const std::set<QString>& existingNames, QWidget *parent)
    : QDialog(parent),
    m_existingNames(existingNames)
{
    ui.setupUi(this);

    ui.label_status->setStyleSheet("QLabel { color : red; }");

    ui.label_status->setVisible(false);
    ui.button_ok->setEnabled(false);

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_ok, SIGNAL(clicked()), this, SLOT(finish()));
    connect(ui.lineEdit_name, SIGNAL(textChanged(const QString &)), this, SLOT(textEntered(const QString &)));
}

NewParameterNameDialog::~NewParameterNameDialog()
{

}

QString NewParameterNameDialog::getNewName() const
{
    return ui.lineEdit_name->text();
}

void NewParameterNameDialog::finish()
{
    done(1);
}

void NewParameterNameDialog::textEntered(const QString &text)
{
    bool state = m_existingNames.find(text) == m_existingNames.end();

    ui.label_status->setVisible(!state);
    ui.button_ok->setEnabled(state);
}