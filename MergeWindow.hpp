#ifndef MERGEWINDOW_H
#define MERGEWINDOW_H

#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QWidget>
#include <QList>
#include <QInputDialog>

#include <QMainWindow>

#include "Database.hpp"

#include <sstream>

class MainWindow;

class MergeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MergeWindow(Database* database, MainWindow* mainwindow, QWidget *parent = 0);
    ~MergeWindow() {}

    QList<uint32_t> GetSelectedItems();

public slots :
    void RefreshNameList();
    void MergeItems(); ///< Merge the items in the database


private:
    Database* m_database;
    MainWindow* m_mainwindow;

    QWidget *m_window;
    QVBoxLayout *m_mainLayout; ///< Layout containing everything

    QListWidget *m_namelist; ///< List of names

    QHBoxLayout *m_buttons;
    QWidget *m_buttonWidget;

    QToolButton *m_close;

    std::map<QListWidgetItem*, int> m_itemToID;
};
#endif
