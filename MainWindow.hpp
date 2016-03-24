#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QHBoxLayout>
#include <QToolButton>
#include <QMenuBar>
#include <QMenu>

#include <QAction>

#include "Database.hpp"
#include "LogEntry.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void LookUpLogs(QListWidgetItem* item,QListWidgetItem* itemp);
    void AddUser();
    void OpenLogDialogForAdd(); ///< Opens up the dialog
    void OpenLogDialogForEdit(); ///< Open log dialog for editing
    void UpdateLog(); ///< Either update or add a log
    void DeleteLog(); ///< Delete the selected log
    void EditUser(); ///< Edit a user name
    void OpenDB();
    void NewDB();

signals:
    void OpenEntry(QString &title, QString &text, QString &name, QString &date, int id);

private:
    QListWidget* m_namelist; ///< List of names
    QListWidget* m_loglist; ///< List of logs titles and dates
    QHBoxLayout *m_cl; ///< The layout contaning the lists
    QVBoxLayout *m_mainLayout; ///< Layout containing everything

    QMenuBar *m_menuBar;
    QMenu *m_menu;
    QAction *m_newAction;
    QAction *m_openAction;


    QVBoxLayout *m_buttons;
    QToolButton *m_editlog;
    QToolButton *m_edituser;
    QToolButton *m_deletelog;
    //QToolButton *m_deleteuser;
    QToolButton *m_addlog;
    QToolButton *m_adduser;

    QWidget *m_buttonWidget;
    QWidget *m_window; ///< The mainwindow
    QWidget *m_listwindow; ///< The mainwindow

    Database* m_database;

    void RefreshNameList(); ///< Refresh the name list
    void RefreshLogList(QListWidgetItem* item); ///< Refresh log list for the name item

    std::map<QListWidgetItem*, int> m_itemToID;
    std::map<QListWidgetItem*, int> m_itemToIDLog;

    LogEntry m_logentry;

    bool m_editMode;

    QString m_currentDirectory;
    QLabel* m_fileName;
};

#endif // MAINWINDOW_H

