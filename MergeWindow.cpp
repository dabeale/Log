#include "MergeWindow.hpp"
#include "MainWindow.hpp"

MergeWindow::MergeWindow(Database *database,  MainWindow* mainwindow, QWidget *parent) :
    QMainWindow(parent),
    m_database( database ),
    m_mainwindow( mainwindow ),
    m_window(new QWidget(this)),
    m_mainLayout(new QVBoxLayout()),
    m_namelist(new QListWidget(this)),
    m_buttons(new QHBoxLayout()),
    m_buttonWidget(new QWidget(this)),
    m_close(new QToolButton(this))
{
    m_close->setText("Close");
    m_buttons->addStretch();
    m_buttons->addWidget( m_close );
    m_buttonWidget->setLayout( m_buttons );

    m_mainLayout->addWidget(m_namelist);
    m_mainLayout->addWidget( m_buttonWidget );
    m_namelist->setSelectionMode( QAbstractItemView::MultiSelection );

    m_window->setLayout(m_mainLayout);

    setCentralWidget(m_window);
    setMinimumWidth(100);
    setMinimumHeight(400);
    setContentsMargins(20, 20, 20, 20);

    QObject::connect(m_close, SIGNAL(clicked()), this, SLOT(MergeItems()));
    QObject::connect(m_close, SIGNAL(clicked()), m_mainwindow, SLOT(RefreshNameList()));
    QObject::connect(m_close, SIGNAL(clicked()), this, SLOT(hide()));


    hide();
}

QList<uint32_t> MergeWindow::GetSelectedItems()
{
    QList<uint32_t> SelectedItems;
    for(const auto& n : m_namelist->selectedItems())
    {
        SelectedItems.push_back( m_itemToID[n] );
    }
    return SelectedItems;
}

void MergeWindow::MergeItems()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("New label name"),
                                     tr("New label:"), QLineEdit::Normal,
                                     "", &ok);
    if (ok && !text.isEmpty())
    {
        auto SelectedItems = GetSelectedItems();
        uint32_t firstSelectedItem = *SelectedItems.begin();
        for(const auto& s : SelectedItems)
        {
            auto logs =  m_database->GetLogsFor(s);
            std::string selectedname = m_database->GetName(s);
            for(const auto log : logs)
            {
                auto specificLog = m_database->GetLog(std::get<0>(log));
                std::string title =std::get<1>(specificLog);
                m_database->EditLog( (QString(selectedname.c_str()) + tr(" : ") + QString(title.c_str())).toStdString(),
                                     std::get<2>(specificLog), std::get<3>(specificLog), firstSelectedItem, std::get<0>(specificLog) );
            }

            if(s!= firstSelectedItem)
            {
                m_database->DeleteName(s);
            }
            else
            {
                m_database->EditName(s, text.toStdString());
            }
        }
    }

}


void MergeWindow::RefreshNameList()
{
    auto namelist = m_database->GetNames();
    m_namelist->clear();
    m_itemToID.clear();
    //new QListWidgetItem(tr(""), m_namelist);
    for(const auto& n : namelist)
    {
        m_itemToID[new QListWidgetItem(tr(n.second.c_str()), m_namelist)] = n.first;
    }
}
