#include "Database.hpp"

#include <iostream>
#include <sstream>
#include <tuple>
#include <set>

std::string AddSlashes( const std::string& str)
{
    std::map<char, std::string> reps;
    reps['\0'] = "\\\0";
    reps['\''] = "\'\'";
    reps['\"'] = "\"\"";
    reps['\\'] = "\\\\";

    decltype(reps.begin()) pos;

    std::stringstream ss;
    for(auto& a : str)
    {
        pos = reps.find(a);
        if (pos!=reps.end())
            ss << pos->second;
        else
            ss << a;
    }
    return ss.str();
}

std::string StripSlashes( const std::string& str)
{
    std::set<char> reps;
    reps.insert('\0');
    reps.insert('\\');

    std::stringstream ss;
    for(auto ait=str.begin(); ait!=str.end(); ++ait)
    {
        if(*ait=='\\' && ait+1 !=str.end() && reps.find(*(ait+1)) != reps.end())
            continue;
        ss << *ait;
    }
    return ss.str();
}

Database::Database(QWidget *parent):
    QWidget(parent)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

std::deque<std::pair<int, std::string>> Database::GetNames() const
{
    std::deque<std::pair<int, std::string>> ret;
    QSqlQuery query("SELECT * from Person");
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
    while (query.next())
    {
        ret.push_back(std::make_pair(query.value(0).toInt(), StripSlashes(query.value(1).toString().toStdString())));
    }
    return ret;
}

void Database::OpenFile(const std::string& filename)
{
    m_db.setDatabaseName(filename.c_str());
    if(!m_db.open())
    {
        std::cerr << "error: Could not find database" << std::endl;
        QMessageBox::information(0,"Ok","Database not found");
    }
    QSqlQuery query("SELECT id from FileInfo");
    query.next();
    if(query.value(0).toInt() != 1)
    {
        QMessageBox::information(0,"Ok","Incorrect database type");
    }
}

void Database::NewDatabase(const std::string& filename)
{
    m_db.setDatabaseName(filename.c_str());
    if(!m_db.open())
    {
        std::cerr << "error: Unknown error" << std::endl;
        QMessageBox::information(0,"Ok","Unknown error");
    }
    QSqlQuery query1("CREATE TABLE FileInfo ( id int )");
    QSqlQuery query2("insert into FileInfo (id) values (1)");
    QSqlQuery query3("CREATE TABLE Log(ID INT PRIMARY KEY NOT NULL, \
                                       Log TEXT, \
                                       Date DATE, \
                                       PersonID INT, Title TEXT);");
    QSqlQuery query4("CREATE TABLE Person( \
                                     ID INT PRIMARY KEY NOT NULL, \
                                     Name   TEXT \
                                     );");
}

int Database::AddName(const std::string& name)
{
    QSqlQuery query("select MAX(id) from Person");
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
    query.next();
    int id = query.value(0).toInt()+1;
    std::stringstream ss;
    ss << "insert into Person (id, name) values ("<<id << ",\"" << AddSlashes(name) << "\")";
    QSqlQuery query2(tr(ss.str().c_str()));
    if (query2.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query2.lastError().text() );
    }
    return id;
}

std::deque< std::tuple<int, std::string, std::string> > Database::GetLogsFor(const int id)
{
    std::stringstream ss;
    ss << "select  id, Title, Date from Log where PersonId="<< id << " ORDER BY Date DESC";
    QSqlQuery query(tr(ss.str().c_str()));
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }

    std::deque< std::tuple<int, std::string, std::string> > ret;
    while (query.next())
    {
        ret.push_back(std::make_tuple(query.value(0).toInt(),
                                      StripSlashes(query.value(1).toString().toStdString()),
                                      query.value(2).toString().toStdString()));
    }
    return ret;
}

std::tuple<int, std::string, std::string, std::string> Database::GetLog(const int id)
{
    std::stringstream ss;
    ss << "select  id, Title, Date, Log from Log where id="<< id;
    QSqlQuery query(tr(ss.str().c_str()));
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
    query.next();

    return std::make_tuple(query.value(0).toInt(),
                           StripSlashes(query.value(1).toString().toStdString()),
                           query.value(2).toString().toStdString(),
                           StripSlashes(query.value(3).toString().toStdString()));
}

void Database::AddLog(const std::string& logtitle,
                       const std::string& date,
                       const std::string& logcontent,
                       const int logid)
{
    QSqlQuery query("select MAX(id) from Log");
    query.next();
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
    int id = query.value(0).toInt()+1;
    std::stringstream ss;
    ss << "insert into Log (ID, Log, Date, PersonID, Title) values ("
       << id << "," << "\"" << AddSlashes(logcontent) << "\",\"" << date << "\"," <<
          logid << ",\"" << AddSlashes(logtitle) << "\")";
    QSqlQuery query2(ss.str().c_str());
    if (query2.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query2.lastError().text() );
    }
}

void Database::EditLog( const std::string& logtitle,
             const std::string& date,
             const std::string& logcontent,
             const int personid,
             const int logid)
{
    std::stringstream ss;
    ss << "update Log set Log = \"" << AddSlashes(logcontent) << "\","
       << "Date = \"" << date << "\","
       << "PersonID = " << personid << ","
       << "Title = \"" << AddSlashes(logtitle) << "\" where ID=" << logid;
    QSqlQuery query(ss.str().c_str());
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
}

void Database::EditName(const int id, const std::string& newname)
{
    std::stringstream ss;
    ss << "update Person set Name=\"" << AddSlashes(newname) << "\" where id="<<id;
    QSqlQuery query(ss.str().c_str());
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
}

void Database::DeleteLog(const int id)
{
    std::stringstream ss;
    ss << "delete from Log where id="<<id;
    QSqlQuery query(ss.str().c_str());
    if (query.lastError().isValid())
    {
        QMessageBox::critical(0,"Error",query.lastError().text() );
    }
}
