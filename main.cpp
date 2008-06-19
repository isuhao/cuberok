#include <stdlib.h>
#include "cuberok.h"
#include "database.h"

#include <QtGui>
#include <QApplication>

QString style_name;

const int ver = 1;

void storeState(Cuberok *win)
{
	QSettings set;
	set.setValue("style", style_name);
	set.setValue("palette", QApplication::palette());
	set.setValue("winstate", win->saveState(ver));
	set.setValue("wingeometry", win->saveGeometry());
	/*QFile file(QDir::homePath()+"/.cuberock/state.dat");
	file.open(QIODevice::WriteOnly);
	QDataStream out(&file);   // we will serialize the data into the file
	out << style_name;   
	out << QApplication::palette(); 
	out << win->saveState(ver); 
	out << win->saveGeometry();
	out << win->splitter();
	*/
}

void restoreState(Cuberok *win)
{
	QSettings set;
	style_name = set.value("style").toString();
	QPalette p = set.value("palette").value<QPalette>();
	win->restoreState(set.value("winstate").toByteArray(), ver);
	win->restoreGeometry(set.value("wingeometry").toByteArray());
	QApplication::setStyle(style_name);
	QApplication::setPalette(p);
	/*QFile file(QDir::homePath()+"/.cuberock/state.dat");
	if(!file.exists()) return;
	file.open(QIODevice::ReadOnly);
	QDataStream in(&file);    // read the data serialized from the file
	QPalette p;
	QByteArray s, g;
	int splitter;
	in >> style_name >> p >> s >> g >> splitter;
	QApplication::setStyle(style_name);
	QApplication::setPalette(p);
	win->restoreState(s, ver);
	win->restoreGeometry(g);
	win->setSplitter(splitter);
	*/
}

int main(int argc, char *argv[])
{
	srand(0);
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("DrMoriarty");
	//QCoreApplication::setOrganizationDomain("");
	QCoreApplication::setApplicationName("Cuberok");	
	//QDir::home().mkdir(".cuberock");
    Cuberok w;
#ifndef DEBUG
    restoreState(&w);
#endif
    w.show();
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    //Database::Self();
    int res = a.exec();
#ifndef DEBUG
    storeState(&w);
#endif
    return res;
}
