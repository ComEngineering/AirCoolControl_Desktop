#include "AddNewConfigDialog.h"
#include <QFileInfo>
#include <qmessagebox.h>

#include "Configurator.h"

AddNewConfigDialog::AddNewConfigDialog(ConfigStorage& storage, QWidget *parent)
    : QDialog(parent),
    m_storage(storage)
{
    ui.setupUi(this);

    connect(ui.copy_button, SIGNAL(clicked()), this, SLOT(copyFromAnotherConfig()));
    connect(ui.add_button, SIGNAL(clicked()), this, SLOT(addConfig()));
    connect(ui.cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui.select_graph, SIGNAL(clicked()), this, SLOT(selectGraphFile()));
    
    connect(ui.config_name_edit, SIGNAL(textChanged(const QString &)), this, SLOT(checkWidgetsEnabled()));
    connect(ui.old_configs_list, SIGNAL(itemSelectionChanged()), this, SLOT(checkWidgetsEnabled()));
   
    for (const auto a_name : storage.getNames())
    {
        ui.old_configs_list->addItem(QString::fromStdString(a_name));
    }

    m_map = std::make_shared<ConfigMap>();
    fillFields();

    checkWidgetsEnabled();
}

AddNewConfigDialog::~AddNewConfigDialog()
{

}

void AddNewConfigDialog::fillFields(void)
{
    std::string vendor, product;
    VersionStorage max, min;

    m_map->fetchInfo(vendor, product, max, min);

    ui.vendor_edit->setText(QString::fromStdString(vendor));
    ui.product_edit->setText(QString::fromStdString(product));
    ui.min_version->setVersion(min);
    ui.max_version->setVersion(max);
}

void AddNewConfigDialog::copyFromAnotherConfig()
{
    int n = ui.old_configs_list->currentRow();

    std::list<ConfigMapShared>::iterator it;
    for (it = m_storage.begin(); it != m_storage.end(); it++)
        if (n-- == 0)
        {
            (*m_map) = *(*it);
            fillFields();
        }
    
}

void AddNewConfigDialog::addConfig()
{
    QString path = Configurator::getConfigFilesPath() + ui.config_name_edit->text() + ".xml";
    QFileInfo checkFile(path);
    if (checkFile.exists())
    {
        QMessageBox::critical(this, tr("Invalid File name"), tr("Document with this name exists.\nGive it another name"));
        return;
    }
    m_map->setFilePath(ui.config_name_edit->text().toStdString());
    m_map->setName(ui.config_name_edit->text().toStdString());
    m_storage.push_back(m_map);
    m_map->save();
    done(1);
}

void AddNewConfigDialog::selectGraphFile()
{

}

void AddNewConfigDialog::checkWidgetsEnabled()
{
    bool addAllowed = !ui.config_name_edit->text().isEmpty();
    bool copyAllowed = !ui.old_configs_list->selectedItems().isEmpty();

    ui.add_button->setEnabled(addAllowed);
    ui.label_name->setStyleSheet(addAllowed ? "QLabel { color : black; }" : "QLabel { color : red; }");
    ui.copy_button->setEnabled(copyAllowed);
}
