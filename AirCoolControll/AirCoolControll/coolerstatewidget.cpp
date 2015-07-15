#include "coolerstatewidget.h"
#include <qtoolbutton.h>
#include <qspinbox.h>
#include <QMutexLocker>
#include "DeviceExplorer.h"
#include <QIntValidator>
#include "Configurator.h"

CoolerStateWidget::CoolerStateWidget(DeviceExplorer *parent)
    : QWidget(NULL),
    m_parent(parent),
    m_needSetTimeRange(true),
    m_colorEnumerator(0)
{ 
    ui.setupUi(this);

    QList<int> sizes;
    int splitterLength = size().width();
    int tabLength = splitterLength * .4;
    
    sizes.push_back(tabLength);
    sizes.push_back(splitterLength - tabLength);
    ui.splitter->setSizes(sizes);

    ui.inputParametersTable->setSortingEnabled(false);

    m_tables[ConfigMap::INPUT_REGISTER] = ui.inputParametersTable;
    m_tables[ConfigMap::OUTPUT_REGISTER] = ui.outputParametersTable;
    m_tables[ConfigMap::COIL] = ui.coilsTable;
    
    ui.frame->resize(500,ui.frame->height());
    initPlotter();
    setUpdatesEnabled(true);
    startTimer(Configurator::getPullInterval());
}

CoolerStateWidget::~CoolerStateWidget()
{

}

void CoolerStateWidget::timerEvent(QTimerEvent *event)
{
    QMutexLocker locker(&m_updateMutex);

    for (auto i : m_tables)
    {
        i->update();
    }

    QVector<qreal> x, y;
    for (const auto& i : m_plotList)
    {
        m_parent->getHistoryForRegesty(i.first, x, y);
        if (m_needSetTimeRange && x.size() > 0)
        {
            ui.plotView->xAxis->setRange(x[0], x[0] + 3600.);
            m_needSetTimeRange = false;
        }
        i.second->setData(x, y);
        ui.plotView->replot();
        x.clear();
        y.clear();
    }
}

void CoolerStateWidget::setParameterList(const std::vector<std::pair<std::string, std::string>>& list, ConfigMap::RegisterType type)
{
    m_tables[type]->blockSignals(true);
    m_tables[type]->setRowCount(list.size());

    int currentRow = 0;
    for (std::pair<std::string, std::string> a_line : list)
    {
        QTableWidgetItem* newItem = new QTableWidgetItem(QString::fromStdString(a_line.second));
        newItem->setFlags(Qt::ItemIsEnabled);
        m_tables[type]->setItem(currentRow, 2, newItem);

        QCheckBox* check = new QCheckBox(this);
        m_tables[type]->setCellWidget(currentRow, 1, check);
        check->setProperty("name", QVariant(QString::fromStdString(a_line.first)));
        connect(check, SIGNAL(clicked()), this, SLOT(onPlotCheckChanged()));

        if (type == ConfigMap::COIL)
        {
            QToolButton* button = new QToolButton(this);
            button->setCheckable(true);
            button->setProperty("name", QVariant(QString::fromStdString(a_line.first)));
            button->setText(tr("Off"));
            connect(button, SIGNAL(clicked()), this, SLOT(onCoilChanged()));
            m_tables[type]->setCellWidget(currentRow, 0, button);
        }
        else if (type == ConfigMap::INPUT_REGISTER)
        {
            newItem = new QTableWidgetItem(QString());
            newItem->setData(Qt::UserRole, QVariant(QString::fromStdString(a_line.first)));
            newItem->setData(Qt::UserRole + 1, QVariant(static_cast<quint16>(type)));
            m_tables[type]->setItem(currentRow, 0, newItem);
        }
        else if (type == ConfigMap::OUTPUT_REGISTER)
        {
            QSpinBox * edit = new QSpinBox(this);
            edit->setProperty("name", QVariant(QString::fromStdString(a_line.first)));
            edit->setMaximum(10000);
            edit->setMinimum(-10000);
            edit->setSingleStep(1);
            edit->setFrame(false);
            connect(edit, SIGNAL(valueChanged(int)), this, SLOT(registerSet(int)));
            m_tables[type]->setCellWidget(currentRow, 0, edit);
        }
        else
        {
            assert(false);
        }
        currentRow++;
    }

    m_tables[type]->blockSignals(false);
}

void CoolerStateWidget::updateParameter(int n, QVariant value, ConfigMap::RegisterType type)
{
    QMutexLocker locker(&m_updateMutex);

    m_tables[type]->blockSignals(true);
    
    bool isValid;
    int number = value.toInt(&isValid);
    
    if (ConfigMap::INPUT_REGISTER == type)
    {
        QTableWidgetItem *aItem = m_tables[type]->item(n, 0);
        if (NULL != aItem)
        {
            if (isValid)
            {
                aItem->setText(QString::number(number));
                aItem->setBackgroundColor(QColor(Qt::white));
            }
            else
            {
                aItem->setText(value.toString());
                aItem->setBackgroundColor(QColor(Qt::red));
            }
        }
    }
    else if (ConfigMap::OUTPUT_REGISTER == type)
    {
        QSpinBox * spin = qobject_cast<QSpinBox*>(m_tables[type]->cellWidget(n, 0));
        
        if (NULL != spin && isValid)
        {
            spin->blockSignals(true);
            spin->setValue(number);
            spin->blockSignals(false);
        }
    }

    m_tables[type]->blockSignals(false);
}

void CoolerStateWidget::registerSet(int v)
{
    bool ok;
    QSpinBox* edit = qobject_cast<QSpinBox*>(sender());
 
    QVariant var = edit->property("name");
    if (!var.isValid()) return;
    QString name = var.toString();
    m_parent->sendValueToDevice(ConfigMap::OUTPUT_REGISTER, name, v);
}

void CoolerStateWidget::onCoilChanged()
{
    QToolButton* button = qobject_cast<QToolButton*>(sender());
    if (!button) return;
    QVariant var = button->property("name");
    if (!var.isValid()) return;
    QString name = var.toString();
    int d = button->isChecked() ? 1 : 0;
    button->setText(d ? tr("On") : tr("Off"));
    m_parent->sendValueToDevice(ConfigMap::OUTPUT_REGISTER, name, d);
}

void CoolerStateWidget::onPlotCheckChanged()
{
    QCheckBox* check = qobject_cast<QCheckBox*>(sender());
    if (!check) return;
    QVariant var = check->property("name");
    if (!var.isValid()) return;
    QString name = var.toString();
    
    if (check->isChecked())
    {
        QCPGraph* graph = ui.plotView->addGraph();
        QColor color(Qt::GlobalColor(Qt::red + (m_colorEnumerator++ % (Qt::darkYellow - Qt::red))));
        graph->setPen(QPen(color,2));
        QCPScatterStyle scatter(QCPScatterStyle::ssCircle, color, 4);
        graph->setScatterStyle(scatter);
        m_plotList[name] = graph;
        graph->setName(QString::fromStdString(m_parent->getCurrentConfig()->getParameterDescription(name.toStdString())));    
    }
    else
    {
        ui.plotView->removeGraph(m_plotList[name]);
        m_plotList.erase(name);
    }
    ui.plotView->replot();
}

void CoolerStateWidget::initPlotter(void)
{
    ui.plotView->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes | QCP::iSelectLegend | QCP::iSelectPlottables);
    
    ui.plotView->yAxis->setRange(-20, 100);
    ui.plotView->axisRect()->setupFullAxesBox();

    ui.plotView->plotLayout()->insertRow(0);
    ui.plotView->plotLayout()->addElement(0, 0, new QCPPlotTitle(ui.plotView, tr("History")));

    ui.plotView->xAxis->setLabel(tr("Time"));
    ui.plotView->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui.plotView->yAxis->setLabel(tr("Value"));

    ui.plotView->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui.plotView->legend->setFont(legendFont);
    ui.plotView->legend->setSelectedFont(legendFont);
    ui.plotView->legend->setSelectableParts(QCPLegend::spItems);
}