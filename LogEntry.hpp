#ifndef LOGENTRY_HPP
#define LOGENTRY_HPP

#include <QDialog>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QDateEdit>
#include <QLabel>
#include <QToolButton>

class LogEntry : public QDialog
{
    Q_OBJECT

public:
    LogEntry(QWidget *parent = 0);

public slots:
    void Open(QString &title, QString &text, QString &name, QString &date, int id);

    void Done();        ///< Close the window and emit changed
    void Clear();       ///< Clears all of the edit boxes
    QString GetTitle(); ///< Get the title
    QString GetText();  ///< Get the text
    QString GetDate();  ///< Get the date as a string
    int GetID();

signals:
    void Changed();

private:
    QVBoxLayout* m_layout;
    QLabel* m_name;
    QTextEdit* m_maintext;
    QLineEdit* m_lineedit;
    QDateEdit* m_dateEdit;
    QToolButton* m_okbutton;
    int m_id;
};

#endif // LOGENTRY_HPP

