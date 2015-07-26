#include "SelectConfigWidget.h"

SelectConfigWidget::SelectConfigWidget(ConfigStorage& storage, QWidget *parent)
    : QDialog(parent),
    m_storage(storage)
{
    ui.setupUi(this);

    for (const auto a_config : storage)
    {
        ui.list_configs->addItem(QString::fromStdString(a_config->getName()));
    }

    connect(ui.button_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.button_edit, SIGNAL(clicked()), this, SLOT(editOK()));
    connect(ui.list_configs, SIGNAL(itemSelectionChanged()), this, SLOT(checkWidgetsEnabled()));

    checkWidgetsEnabled();
}

SelectConfigWidget::~SelectConfigWidget()
{

}

ConfigMapShared SelectConfigWidget::getConfig(void) const
{
    int n = ui.list_configs->currentRow();

    std::list<ConfigMapShared>::iterator it;
    for (it = m_storage.begin(); it != m_storage.end(); it++)
        if (n-- == 0)
        {
            return (*it);
        }
}

void SelectConfigWidget::editOK(void)
{
    done(1);
}

void SelectConfigWidget::checkWidgetsEnabled()
{
    bool editAllowed = !ui.list_configs->selectedItems().isEmpty();

    ui.button_edit->setEnabled(editAllowed);
}