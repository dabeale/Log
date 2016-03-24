#include "LogEntry.hpp"

LogEntry::LogEntry(QWidget *parent) :
    QDialog(parent),
    m_layout(new QVBoxLayout(this)),
    m_name(new QLabel(this)),
    m_maintext(new QTextEdit(this)),
    m_lineedit(new QLineEdit(this)),
    m_dateEdit(new QDateEdit(this)),
    m_okbutton(new QToolButton(this)),
    m_id(0)
{
    m_layout->addWidget(m_name);
    m_layout->addWidget(new QLabel(tr("Date:"),this));
    m_layout->addWidget(m_dateEdit);
    m_layout->addWidget(new QLabel(tr("Title:"),this));
    m_layout->addWidget(m_lineedit);
    m_layout->addWidget(new QLabel(tr("Main text:"),this));
    m_layout->addWidget(m_maintext);
    m_layout->addWidget(m_okbutton);

    m_okbutton->setText("Done");

    setLayout(m_layout);
    setWindowTitle(tr("New Log"));
    setMinimumWidth(500);
    setMinimumHeight(600);

    QObject::connect(m_okbutton, SIGNAL(clicked()), this, SLOT(Done()));
}

void LogEntry::Open(QString& title, QString& text, QString& name, QString& date, int id)
{
    m_name->setText(name);
    m_maintext->setText(text);
    m_lineedit->setText(title);
    m_dateEdit->setDate(QDate::fromString(date,Qt::ISODate));
    m_id = id;
    this->show();
}

void LogEntry::Clear()
{
    m_lineedit->clear();
    m_maintext->clear();
    m_name->clear();
    m_dateEdit->clear();
}

QString LogEntry::GetTitle()
{
    return m_lineedit->text();
}

QString LogEntry::GetText()
{
    return m_maintext->toPlainText();
}

QString LogEntry::GetDate()
{
    return m_dateEdit->date().toString(Qt::ISODate);
}

int LogEntry::GetID()
{
    return m_id;
}

void LogEntry::Done()
{
    this->hide();
    emit Changed();
}
