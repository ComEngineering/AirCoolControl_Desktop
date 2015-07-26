#include "EditConfigWindow.h"

EditConfigWindow::EditConfigWindow(ConfigMapShared a_map, QWidget *parent)
    : QDialog(parent),
    m_map(a_map)
{
    ui.setupUi(this);
}

EditConfigWindow::~EditConfigWindow()
{

}
