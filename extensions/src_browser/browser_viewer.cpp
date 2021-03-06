/* Cuberok
 * Copyright (C) 2008-2009 Vasiliy Makarov <drmoriarty.0@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "browser_viewer.h"
#include "magnatune_browser.h"
#include "jamendo_browser.h"

BrowserList::BrowserList(QWidget*parent): QListWidget(parent), browser(0)
{
}

QStringList BrowserList::mimeTypes () const
{
	QStringList list;
	list.append("text/uri-list");
	list << CUBEROK_MIME_TYPE;
	return list;
}

QMimeData * BrowserList::mimeData ( const QList<QListWidgetItem *> items ) const
{
	QMimeData *mime = new QMimeData();
	if(browser && browser->tagsAvailable()) {
		QVector<STags> tags;
		foreach(QListWidgetItem *it, items) {
			QStringList list;
			list << it->text();
			list << it->data(Qt::ToolTipRole).toString();
			list << it->data(Qt::StatusTipRole).toString();
			list << it->data(Qt::UserRole).toString();
			STags tag = browser->getTags(list);
			tags << tag;
		}
		QByteArray array;
		QDataStream stream(&array, QIODevice::WriteOnly);
		stream << tags;
		mime->setData(CUBEROK_MIME_TYPE, array);
	} else {
		QList<QUrl> urls;
		foreach(QListWidgetItem *it, items) {
			QString url;
			url = it->data(Qt::UserRole).toString();
			if(url.size())
				urls << QUrl(url);
		}
		mime->setUrls(urls);
	}
	return mime;
}

BrowserViewer::BrowserViewer(Proxy *pr, QWidget * parent, Qt::WindowFlags f)
	: QWidget(parent, f), proxy(pr), browser(0)
{
 	ui.setupUi(this);
	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(itemActivated(QListWidgetItem*)));
	browserChanged(ui.comboBox->currentIndex());
}

BrowserViewer::~BrowserViewer()
{
	if(browser) {
		browser->disconnect();
		delete browser;
		browser = 0;
	}
}

void BrowserViewer::browserChanged(int i)
{
	if(browser) {
		ui.listWidget->browser = 0;
		browser->disconnect();
		delete browser;
		browser = 0;
	}
	ui.listWidget->clear();
	switch(i) {
	case 0:  // empty
		return;
	case 1:  // MagnaTune
		browser = new MagnatuneBrowser(proxy);
		break;
	case 2:  // Jamendo
		browser = new JamendoBrowser(proxy);
		break;
	default:
		return;
	}
	if(!connect(browser, SIGNAL(list(QList< QStringList >)), this, SLOT(putList(QList< QStringList >))))
		proxy->error("Can't connect to browser.list");
	ui.listWidget->browser = browser;
	home();
}

void BrowserViewer::putList(QList< QStringList > list)
{
	proxy->log("Put "+QString::number(list.size())+" items into browser viewer");
	ui.listWidget->clear();
	foreach(QStringList item, list) {
		if(item.size() >= 4 && item[0].size()) {
			QListWidgetItem *lwi = new QListWidgetItem(item[0]);
			if(item[3].size()) { // has URL
				lwi->setIcon(QIcon(":/icons/def_song.png"));
			} else {
				lwi->setIcon(QIcon(":/icons/forward.png"));
			}
			lwi->setData(Qt::ToolTipRole, item[1]);
			lwi->setData(Qt::StatusTipRole, item[2]);
			lwi->setData(Qt::UserRole, item[3]);
			ui.listWidget->addItem(lwi);
		}
	}
}

void BrowserViewer::itemActivated(QListWidgetItem* it)
{
	QString id, url;
	id = it->data(Qt::StatusTipRole).toString();
	url = it->data(Qt::UserRole).toString();
	if(id.size()) {
		pathItems << it->data(Qt::DisplayRole).toString();
		history << current;
		goTo(id, it->text());
	} else if(url.size()) {
		/*if(url.toLower().indexOf("xspf") >= 0) { // need to download a playlist
			dl = new Downloader();
			connect(dl, SIGNAL(complete(QString)), this, SLOT(dlComplete(QString)));
			connect(dl, SIGNAL(cancel(QString)), this, SLOT(dlCancel(QString)));
			if(!dl->download(QUrl(url))) {
				delete dl;
			}
			} else*/ {  // this is a direct link to stream
			addUrl(url);
		}
	}
}

void BrowserViewer::back()
{
	QString last;
	if(history.size() && browser) {
		last = history[history.size()-1];
		history.removeLast();
		current = last;
		pathItems.removeLast();
		updatePath();
		browser->GetList(last);
	}
}

/*void BrowserViewer::forward()
  {
  QString last;
  if(fhistory.size() && browser) {
  last = fhistory[0];
  fhistory.removeFirst();
  history.push_back(current);
  current = last;
  pathItems.push_back(fpathItems.first());
  fpathItems.removeFirst();
  updatePath();
  browser->GetList(last);
  }
  }*/

void BrowserViewer::home()
{
	pathItems.clear();
	history.clear();
	goTo("", "");
}

void BrowserViewer::reload()
{
	goTo(current, "");
}

void BrowserViewer::goTo(QString s, QString text)
{
	if(!browser) return;
	current = s;
	updatePath();
	browser->GetList(current, text);
}

void BrowserViewer::updatePath()
{
	QString path;
	foreach(QString s, pathItems) {
		path += "/" + s;
	}
	ui.label->setText(path);
	//ui.toolButton_forward->setDisabled(!fhistory.size());
	ui.toolButton_back->setDisabled(!history.size());
}
/*
void BrowserViewer::dlComplete(QString file)
{
	emit openUrl(QUrl::fromLocalFile(file));
	dl->disconnect();
	//delete dl;
	//dl = 0;
}

void BrowserViewer::dlCancel(QString err)
{
	Console::Self().warning("Cancel: " + err);
	dl->disconnect();
	//delete dl;
	//dl = 0;
}
*/
void BrowserViewer::storeState()
{
}

void BrowserViewer::addUrl(QString url)
{
	qDebug() << "Append url " << url;
	proxy->setPlControl(SPlControl(SPlControl::Append, url));
}
