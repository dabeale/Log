#include <QApplication>
#include "MainWindow.hpp"

#include <stdlib.h>
#include <iostream>
#include <string>


int main(int argc, char *argvv[])
{
    std::cout << "Management Log" << std::endl << std::endl;

    QApplication a(argc, argvv);
    MainWindow w;
    w.show();
    return a.exec();
}

