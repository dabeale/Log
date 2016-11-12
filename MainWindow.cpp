#include "MainWindow.hpp"

#include <QLabel>
#include <QInputDialog>
#include <QFileDialog>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_namelist(new QListWidget(this)),
    m_loglist(new QListWidget(this)),
    m_cl(new QHBoxLayout()),
    m_mainLayout(new QVBoxLayout()),
    m_menuBar(new QMenuBar(this)),
    m_menu(new QMenu(this)),
    m_newAction(new QAction(tr("New"),this)),
    m_openAction(new QAction(tr("Open"), this)),
    m_buttons(new QVBoxLayout()),
    m_editlog(new QToolButton(this)),
    m_edituser(new QToolButton(this)),
    m_deletelog(new QToolButton(this)),
    //m_deleteuser(new QToolButton(this)),
    m_addlog(new QToolButton(this)),
    m_adduser(new QToolButton(this)),
    m_mergeLogs( new QToolButton(this)),
    m_buttonWidget(new QWidget(this)),
    m_window(new QWidget(this)),
    m_listwindow(new QWidget(this)),
    m_database(new Database(this)),
    m_mergeWindow( new MergeWindow(m_database, this, this) ),
    m_logentry(new LogEntry(this)),
    m_editMode(false),
    m_currentDirectory(QDir::homePath()),
    m_fileName(new QLabel(this))
{
    m_fileName->setText("Please select a file before proceeding");

    m_cl->addWidget(m_namelist);
    m_cl->addWidget(m_loglist);
    m_namelist->setMaximumWidth(150);

    m_menu = this->menuBar()->addMenu(tr("&File"));
    m_menu->addAction(m_newAction);
    m_menu->addAction(m_openAction);

    m_editlog->setText(tr("Edit"));
    m_addlog->setText(tr("Add"));
    m_deletelog->setText(tr("Delete"));
    m_editlog->setMinimumWidth(80);
    m_addlog->setMinimumWidth(80);
    m_deletelog->setMinimumWidth(80);

    m_edituser->setText(tr("Edit"));
    m_adduser->setText(tr("Add"));
    m_mergeLogs->setText(tr("Merge Logs"));
    //m_deleteuser->setText(tr("Delete"));
    m_edituser->setMinimumWidth(80);
    m_adduser->setMinimumWidth(80);
    m_mergeLogs->setMinimumWidth(80);
    //m_deleteuser->setMinimumWidth(80);

    m_buttons->addWidget(new QLabel(tr("User:"),this));
    m_buttons->addWidget(m_edituser);
    m_buttons->addWidget(m_adduser);
    //m_buttons->addWidget(m_deleteuser);
    m_buttons->addSpacing(20);
    m_buttons->addWidget(new QLabel(tr("Log:"),this));
    m_buttons->addWidget(m_editlog);
    m_buttons->addWidget(m_addlog);
    m_buttons->addWidget(m_deletelog);
    m_buttons->addSpacing(20);
    m_buttons->addWidget(m_mergeLogs);
    m_buttons->addStretch();

    m_buttonWidget->setLayout( m_buttons );

    m_cl->addWidget(m_buttonWidget);
    m_listwindow->setLayout(m_cl);

    m_mainLayout->addWidget(m_fileName);
    m_mainLayout->addWidget(m_listwindow);

    m_window->setLayout(m_mainLayout);

    setCentralWidget(m_window);
    setMinimumWidth(800);
    setMinimumHeight(600);
    setContentsMargins(20, 20, 20, 20);

    QObject::connect( m_namelist, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                      this, SLOT(LookUpLogs(QListWidgetItem*,QListWidgetItem*)));

    QObject::connect( m_adduser, SIGNAL(clicked()),this, SLOT(AddUser()));
    QObject::connect( m_edituser, SIGNAL(clicked()),this, SLOT(EditUser()));
    QObject::connect( m_addlog, SIGNAL(clicked()),this, SLOT(OpenLogDialogForAdd()));
    QObject::connect( m_editlog, SIGNAL(clicked()),this, SLOT(OpenLogDialogForEdit()));
    QObject::connect( m_deletelog, SIGNAL(clicked()),this, SLOT(DeleteLog()));
    QObject::connect(this, SIGNAL(OpenEntry(QString&,QString&,QString&,QString&,int)),
                     &m_logentry, SLOT(Open(QString&,QString&,QString&,QString&,int)));
    QObject::connect(&m_logentry, SIGNAL(Changed()), this, SLOT(UpdateLog()));
    QObject::connect(m_openAction, SIGNAL(triggered()), this, SLOT(OpenDB()));
    QObject::connect(m_newAction, SIGNAL(triggered()), this, SLOT(NewDB()));
    QObject::connect(m_mergeLogs, SIGNAL(clicked()), m_mergeWindow, SLOT(show()));
    QObject::connect(m_mergeLogs, SIGNAL(clicked()), m_mergeWindow, SLOT(RefreshNameList()));
    OpenDB();
    RefreshNameList();
}

void MainWindow::ShowMergeWindow()
{

}

void MainWindow::OpenDB()
{
    m_loglist->clear();
    m_itemToIDLog.clear();
    QString file = QFileDialog::getOpenFileName(this, tr("Choose file to open"), m_currentDirectory, tr("Database (*.db)"));
    if(file.length() > 0)
    {
        m_currentDirectory = QFileInfo(file).path();
        m_fileName->setText( QFileInfo(file).fileName());
        m_database->OpenFile(file.toStdString());
        RefreshNameList();
    }
}

void MainWindow::NewDB()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Filename"), m_currentDirectory, tr("Database (*.db)"));
    if(file.length() > 0)
    {
        m_currentDirectory = QFileInfo(file).path();
        m_fileName->setText( QFileInfo(file).fileName());
        m_database->NewDatabase(file.toStdString());
        m_loglist->clear();
        m_itemToIDLog.clear();
        m_namelist->clear();
    }
}

void MainWindow::RefreshNameList()
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

void MainWindow::RefreshLogList(QListWidgetItem* item)
{
    m_loglist->clear();
    m_itemToIDLog.clear();
    auto logs = m_database->GetLogsFor(m_itemToID[item]);
    for( const auto& l : logs)
    {
        std::stringstream ss;
        ss << std::get<2>(l) << " \t|  " << std::get<1>(l);
        m_itemToIDLog[new QListWidgetItem(tr(ss.str().c_str()), m_loglist)] = std::get<0>(l);
    }
}

void MainWindow::LookUpLogs(QListWidgetItem* item,QListWidgetItem* itemp)
{
    (void) itemp;
    RefreshLogList(item);
}

void MainWindow::AddUser()
{
    bool ok;
    QString out =QInputDialog::getText(this, tr("Enter a name"), tr("Enter a name"),
                          QLineEdit::Normal, QString(),  &ok);
    if( ok )
    {
        int id = m_database->AddName(out.toStdString());
        m_itemToID[new QListWidgetItem(out, m_namelist)] = id;
    }
}

void MainWindow::OpenLogDialogForAdd()
{
    if(m_namelist->currentItem()!=0)
    {
        QString Empty;
        QString Title = m_namelist->currentItem()->text();
        m_editMode = false;
        QString date = QDate::currentDate().toString(Qt::ISODate);
        emit OpenEntry(Empty,
                        Empty,
                        Title,
                        date,
                        m_itemToID[m_namelist->currentItem()]);
    }
}

void MainWindow::OpenLogDialogForEdit()
{
    if(m_loglist->currentItem()!=0)
    {
        auto log = m_database->GetLog(m_itemToIDLog[m_loglist->currentItem()]);
        int id = std::get<0>(log);
        QString title(std::get<1>(log).c_str());
        QString date(std::get<2>(log).c_str());
        QString logg(std::get<3>(log).c_str());
        QString name(m_loglist->currentItem()->text());
        m_editMode = true;
        emit OpenEntry(title,
                        logg,
                        name,
                        date,
                        id);
    }
}

void MainWindow::UpdateLog()
{
    if(m_editMode)
    {
        m_database->EditLog(m_logentry.GetTitle().toStdString(),
                            m_logentry.GetDate().toStdString(),
                            m_logentry.GetText().toStdString(),
                            m_itemToID[m_namelist->currentItem()],
                            m_logentry.GetID());
        RefreshLogList(m_namelist->currentItem());
    }
    else
    {
        m_database->AddLog(m_logentry.GetTitle().toStdString(),
                           m_logentry.GetDate().toStdString(),
                           m_logentry.GetText().toStdString(),
                           m_logentry.GetID());
        RefreshLogList(m_namelist->currentItem());
    }
}

void MainWindow::DeleteLog()
{
    if(m_loglist->currentItem()!=0)
    {
        std::stringstream ss;
        ss << "You have asked to delete log id " <<m_itemToIDLog[m_loglist->currentItem()];
        QMessageBox msgBox;
        msgBox.setText(ss.str().c_str());
        msgBox.setInformativeText("Are you sure?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            m_database->DeleteLog(m_itemToIDLog[m_loglist->currentItem()]);
            RefreshLogList(m_namelist->currentItem());
        }
    }
}

void MainWindow::EditUser()
{
    bool ok;
    QString out =QInputDialog::getText(this, tr("Enter a name"), tr("Enter a name"),
                          QLineEdit::Normal, m_namelist->currentItem()->text(),  &ok);
    if( ok )
    {
        m_database->EditName(m_itemToID[m_namelist->currentItem()], out.toStdString());
        RefreshNameList();
    }
}

MainWindow::~MainWindow()
{

}
