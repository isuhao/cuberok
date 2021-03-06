/* Cuberok
 * Copyright (C) 2008 Vasiliy Makarov <drmoriarty.0@gmail.com>
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

#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include <QtCore>

class Player : public QObject
{
    Q_OBJECT
 public:
    Player(): QObject(0), manager(0) {};
    ~Player() {};

	virtual bool prepare() = 0;
	virtual bool ready() = 0;

	virtual bool canOpen(QString mime) = 0;
    virtual bool open(QUrl fname, long start = 0, long length = 0) = 0;
    virtual bool play() = 0;
    virtual bool stop() = 0;
    virtual bool setPause(bool p) = 0;
    virtual bool close() = 0;
    virtual bool setPosition(double pos) = 0;
    virtual double getPosition() = 0;
    virtual int  volume() = 0;
    virtual void setVolume(int v) = 0;
    virtual bool playing() = 0;

	virtual int  weight() = 0;
	virtual QString name() = 0;
	virtual void processErrorMessage(QString m) { if(manager) manager->processErrorMessage(m); }
	virtual void setManager(Player* m) { manager = m; }
	virtual QStringList hardcodedList() { return QStringList(); };
	virtual QStringList hardcodedBlacklist() { return QStringList(); };

    //int  repeat_mode;
    //int  shuffle_mode;

 signals:
    void position(double);
    void finish();

 protected:
	long _start;
	long _length;
	Player *manager;
};

Q_DECLARE_INTERFACE(Player, "Cuberok.Player/1.0")

#endif // PLAYER_H
