#ifndef DATABASE_H
#define DATABASE_H

#include <deque>
#include <tuple>

#include <QWidget>
#include <QListWidgetItem>

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

class Database : public QWidget
{
    Q_OBJECT
public:
    Database(QWidget *parent = 0);

    std::deque<std::pair<int, std::string> > GetNames() const; ///< Get the current (id,names) in the database
    int AddName(const std::string& name); ///< Add a name to the data
    void EditName(const int id, const std::string& newname);
    void DeleteName(const int id);

    std::deque< std::tuple<int, std::string, std::string> > GetLogsFor(const int id);
    std::tuple<int, std::string, std::string, std::string> GetLog(const int id);
    void AddLog( const std::string& logtitle,
                 const std::string& date,
                 const std::string& logcontent,
                 const int personid);
    void EditLog( const std::string& logtitle,
                 const std::string& date,
                 const std::string& logcontent,
                 const int personid,
                 const int logid);

    void DeleteLog(const int id);

    void OpenFile(const std::string& filename);
    void NewDatabase(const std::string& filename);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H

