#include "addnewtestdialog.h"
#include <QFileInfo>
#include <qmessagebox.h>

#include "Configurator.h"

AddNewTestDialog::AddNewTestDialog(TestStorage& storage, ConfigStorage& configs, QWidget *parent)
    : QDialog(parent),
    m_storage(storage),
    m_configs(configs),
    m_cloned(false)
{
    ui.setupUi(this);

    connect(ui.copy_button, SIGNAL(clicked()), this, SLOT(copyFromAnotherTest()));
    connect(ui.add_button, SIGNAL(clicked()), this, SLOT(addTest()));
    connect(ui.cancel_button, SIGNAL(clicked()), this, SLOT(reject()));

    connect(ui.test_name_edit, SIGNAL(textChanged(const QString &)), this, SLOT(checkWidgetsEnabled()));
    connect(ui.old_tests_list, SIGNAL(itemSelectionChanged()), this, SLOT(checkWidgetsEnabled()));
    connect(ui.config_list, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(configNameSelected(QListWidgetItem *, QListWidgetItem *)));

    for (const auto a_name : storage.getNames())
    {
        ui.old_tests_list->addItem(QString::fromStdString(a_name));
    }

    m_map = std::make_shared<SimpleTest>();

    m_selectedIcon.addFile(QStringLiteral(":/Images/togle_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    m_notSelectedIcon.addFile(QStringLiteral(":/Images/not_togle_icon.png"), QSize(), QIcon::Normal, QIcon::Off);
    
    std::vector<std::string> configs_names = m_configs.getNames();
 
    for (const auto& a_config_name : configs_names)
    {//
        QListWidgetItem *newItem = new QListWidgetItem(m_notSelectedIcon, QString::fromStdString(a_config_name));
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui.config_list->addItem(newItem);
    }

    checkWidgetsEnabled();
}

AddNewTestDialog::~AddNewTestDialog()
{

}


void AddNewTestDialog::copyFromAnotherTest()
{
    int n = ui.old_tests_list->currentRow();

    std::list<SimpleTestShared>::iterator it;
    for (it = m_storage.begin(); it != m_storage.end(); it++)
        if (n-- == 0)
        {
            (*m_map) = *(*it);
        }

    ui.old_tests_list->setEnabled(false);
    m_cloned = true;
}

void AddNewTestDialog::addTest()
{
    QString name(ui.test_name_edit->text());
    QString path(Configurator::getTestFilesPath() + name + ".xml");
    QFileInfo checkFile(path);
    if (checkFile.exists())
    {
        QMessageBox::critical(this, tr("Invalid File name"), tr("Document with this name exists.\nGive it another name"));
        return;
    }
    
    m_map->setConfig(findConfig());
    m_map->setFilePath(name.toStdString());
    m_map->setName(ui.test_name_edit->text().toStdString());
    m_storage.push_back(m_map);
    m_map->save();

    done(1);
}

void AddNewTestDialog::checkWidgetsEnabled()
{
    bool addAllowed = !(ui.test_name_edit->text().isEmpty() || ui.config_list->selectedItems().isEmpty()) || m_cloned;
    bool copyAllowed = !ui.old_tests_list->selectedItems().isEmpty();

    ui.add_button->setEnabled(addAllowed);
    ui.label_name->setStyleSheet(addAllowed ? "QLabel { color : black; }" : "QLabel { color : red; }");
    ui.copy_button->setEnabled(copyAllowed);
}

void AddNewTestDialog::configNameSelected(QListWidgetItem * current, QListWidgetItem * previous)
{
    current->setIcon(m_selectedIcon);
    if (previous)
        previous->setIcon(m_notSelectedIcon);
    checkWidgetsEnabled();
}

ConfigMapShared AddNewTestDialog::findConfig() const
{
    QList<QListWidgetItem *> config = ui.config_list->selectedItems();
    QString config_name((*(config.begin()))->text());
    return m_configs.getConfig(config_name.toStdString());
}