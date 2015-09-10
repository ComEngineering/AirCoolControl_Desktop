#include "coolerstatewidget.h"
#include <QMutexLocker>
#include "DeviceExplorer.h"
#include <QIntValidator>
#include "Configurator.h"

#include "InputValueFieldWidget.h"
#include "CoilValueFieldWidget.h"
#include "OutValueWidget.h"

CoolerStateWidget::CoolerStateWidget(DeviceExplorer *parent)
    : QWidget(NULL),
    m_parent(parent),
    m_needSetTimeRange(true),
    m_colorEnumerator(0)
{ 
    ui.setupUi(this);
 
    m_splitterSizes.push_back(size().width());
    m_splitterSizes.push_back(0);
    m_splitterSizes.push_back(0);
    ui.splitter->setSizes(m_splitterSizes);

    ui.inputParametersTable->setSortingEnabled(false);

    m_tables[ConfigMap::INPUT_REGISTER] = ui.inputParametersTable;
    m_tables[ConfigMap::OUTPUT_REGISTER] = ui.outputParametersTable;
    m_tables[ConfigMap::COIL] = ui.coilsTable;
    
    ui.frame->resize(500, ui.frame->height());
   // initPlotter();
    setUpdatesEnabled(true);
    startTimer(Configurator::getPullInterval());

    connect(&m_updateSplitterTimer, SIGNAL(timeout()), this, SLOT(updateSplitter()));
    m_updateSplitterTimer.start(k_splitterUpdateTime);
}

CoolerStateWidget::~CoolerStateWidget()
{
}

void CoolerStateWidget::updateSplitter()
{
    QList<int> sizes(ui.splitter->sizes());
    bool needToStopResize = true;
    if (sizes != m_splitterSizes)
    {
        for (int i = 0; i < 3; i++)
        {
            int delta = m_splitterSizes[i] - sizes[i];
            if (abs(delta) <= k_delta)
                sizes[i] = m_splitterSizes[i];
            else
            {
                sizes[i] += (delta < 0) ? -k_delta : k_delta;
                needToStopResize = false;
            }
        }
        ui.splitter->setSizes(sizes);
        update();
    }
    if (needToStopResize)
        m_updateSplitterTimer.stop();
}

void CoolerStateWidget::setNewSplitterMode(bool showPlotter)
{
    int splitterLength = size().width();

    if (showPlotter || false == ui.picture_widget->isValid())
    {
        int tabLength = splitterLength * .4;
        m_splitterSizes[0] = tabLength;
        m_splitterSizes[1] = splitterLength - tabLength;
        m_splitterSizes[2] = 0;
    }
    else
    {
        float as = ui.picture_widget->getAspectRatio();
        m_splitterSizes[2] = size().height() * as;
        m_splitterSizes[1] = 0;
        m_splitterSizes[0] = splitterLength - m_splitterSizes[2];
    }

    m_updateSplitterTimer.start(k_splitterUpdateTime);
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

void CoolerStateWidget::setParameterList(ConfigMapShared config)
{
    ui.picture_widget->setConfig(config, m_parent);
    setNewSplitterMode(!ui.picture_widget->isValid());

    int currentRow[ConfigMap::REGISTER_PULL_COUNT];

    for (ConfigMap::RegisterType i = ConfigMap::REGISTER_PULL_FIRST; i < ConfigMap::REGISTER_PULL_COUNT; ConfigMap::NEXT(i))
    {
        m_tables[i]->blockSignals(true);
        m_tables[i]->setRowCount(config->getParametersList(i).size());
        currentRow[i] = 0;
    }
    
    for (int i = 0; i < config->size(); i++)
    {
        const ConfigMap::Parameter& a_parameter = (*config)[i].second;
        std::string a_name = (*config)[i].first;
        ConfigMap::RegisterType a_type = a_parameter.m_type;

        /// Description
        QTableWidgetItem* newItem = new QTableWidgetItem(QString::fromStdString(a_parameter.m_description));
        newItem->setFlags(Qt::ItemIsEnabled);

        /// Plot check box
        if (a_type != ConfigMap::COIL)
        {
            QCheckBox* check = new QCheckBox(this);
            m_tables[a_type]->setCellWidget(currentRow[a_type], 1, check);
            check->setProperty("name", QVariant(QString::fromStdString(a_name)));
            connect(check, SIGNAL(clicked()), this, SLOT(onPlotCheckChanged()));
        }
        /// Value
        QWidget* valueWidget;
        if (a_type == ConfigMap::COIL)
        {
            valueWidget = new CoilValueFieldWidget(m_parent,a_name,this);
            m_tables[a_type]->setItem(currentRow[a_type], 1, newItem);
        }
        else if (a_type == ConfigMap::INPUT_REGISTER)
        {
            valueWidget = new InputValueFieldWidget(m_parent, a_name, this);
            m_tables[a_type]->setItem(currentRow[a_type], 2, newItem);
        }
        else if (a_type == ConfigMap::OUTPUT_REGISTER)
        {
            valueWidget = new OutValueWidget(m_parent,a_name,a_parameter,this);
            m_tables[a_type]->setItem(currentRow[a_type], 2, newItem);
        }
        else
        {
            assert(false);
        }
        m_tables[a_type]->setCellWidget(currentRow[a_type], 0, valueWidget);
        currentRow[a_type]++;
    }

    for (auto table : m_tables)
        table->blockSignals(false);
}

void CoolerStateWidget::updateParameter(int n, QVariant value, ConfigMap::RegisterType type)
{
    QMutexLocker locker(&m_updateMutex);

    ValueFieldWidget *aItem = qobject_cast<ValueFieldWidget*>(m_tables[type]->cellWidget(n, 0));
    if (aItem)
        aItem->setValue(value);
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
    setNewSplitterMode(!m_plotList.empty());
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