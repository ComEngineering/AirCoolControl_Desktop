#include "SelectConfigWidget.h"

SelectConfigWidget::SelectConfigWidget(std::vector<std::string>&& storage, QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    for (const auto& a_name : storage)
    {
        ui.list_configs->addItem(QString::fromStdString(a_name));
    }

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_edit, SIGNAL(clicked()), this, SLOT(editOK()));
    connect(ui.list_configs, SIGNAL(itemSelectionChanged()), this, SLOT(checkWidgetsEnabled()));
    connect(ui.list_configs, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(editOK(const QModelIndex&)));

    checkWidgetsEnabled();
}

SelectConfigWidget::~SelectConfigWidget()
{

}

int SelectConfigWidget::getSelectedIndex(void) const
{
    return ui.list_configs->currentRow();
}

void SelectConfigWidget::editOK(void)
{
    done(1);
}

void SelectConfigWidget::editOK(const QModelIndex & index)
{
    done(1);
}

void SelectConfigWidget::checkWidgetsEnabled()
{
    bool editAllowed = !ui.list_configs->selectedItems().isEmpty();

    ui.button_edit->setEnabled(editAllowed);
}