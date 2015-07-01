//Copyright 2015 Ryan Wick

//This file is part of Bandage

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.


#include "../ui/mainwindow.h"
#include <QApplication>
#include <QStringList>
#include <QString>
#include <QCommandLineParser>
#include <QTextStream>
#include "../command_line/load.h"
#include "../command_line/image.h"
#include "../command_line/contiguous.h"
#include "../command_line/commoncommandlinefunctions.h"

void printUsage(QTextStream * out)
{
    *out << "" << endl;
    *out << "Usage:   Bandage <command> [options]" << endl << endl;
    *out << "Command: <blank>      launch Bandage GUI" << endl;
    *out << "         load         launch Bandage GUI and load a graph file" << endl;
    *out << "         image        generate a PNG image of a graph" << endl;
    *out << "         contiguous   extract all sequences contiguous with a target sequence" << endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setApplicationName("Bandage");
    QApplication::setApplicationVersion("0.6.0");

    QTextStream out(stdout);
    QTextStream err(stdout);

    QStringList arguments = QCoreApplication::arguments();
    arguments.pop_front();

    //If the user simply called Bandage without any arguments, just run the GUI.
    if (arguments.size() == 0)
    {
        MainWindow w;
        w.show();
        return a.exec();
    }

    QString first = arguments.at(0);
    arguments.pop_front();

    if (first == "-h" || first == "-help" || first == "--help")
    {
        out << "" << endl;
        out << "Program: Bandage" << endl;
        out << "Version: " << QApplication::applicationVersion() << endl;
        printUsage(&out);
        out << "" << endl;
        return 0;
    }

    else if (first == "-v" || first == "-version" || first == "--version")
    {
        out << "Version: " << QApplication::applicationVersion() << endl;
        return 0;
    }

    else if (first == "load")
        return bandageLoad(&a, arguments);

    else if (first == "image")
        return bandageImage(arguments);

    else if (first == "contiguous")
        return bandageContiguous(arguments);

    else
    {
        err << "" << endl << "Invalid command: " << first << endl;
        printUsage(&err);
        err << "" << endl;
        return 1;
    }
}
