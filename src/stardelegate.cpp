 /****************************************************************************
 **
 ** Copyright (C) 2007-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://trolltech.com/products/qt/licenses/licensing/opensource/
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
 ** or contact the sales department at sales@trolltech.com.
 **
 ** In addition, as a special exception, Trolltech gives you certain
 ** additional rights. These rights are described in the Trolltech GPL
 ** Exception version 1.0, which can be found at
 ** http://www.trolltech.com/products/qt/gplexception/ and in the file
 ** GPL_EXCEPTION.txt in this package.
 **
 ** In addition, as a special exception, Trolltech, as the sole copyright
 ** holder for Qt Designer, grants users of the Qt/Eclipse Integration
 ** plug-in the right for the Qt/Eclipse Integration to link to
 ** functionality provided by Qt Designer and its related libraries.
 **
 ** Trolltech reserves all rights not expressly granted herein.
 **
 ** Trolltech ASA (c) 2007
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

 #include <QtGui>

 #include "stardelegate.h"
 #include "stareditor.h"
 #include "starrating.h"

 void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
 {
     if (qVariantCanConvert<StarRating>(index.data())) {
         StarRating starRating = qVariantValue<StarRating>(index.data());

		 if (option.state & QStyle::State_Selected) {
			 if(option.state & QStyle::State_Active)
				 painter->fillRect(option.rect, option.palette.highlight());
			 else 
				 painter->fillRect(option.rect, option.palette.brush(QPalette::Inactive, QPalette::Highlight));
		 }

         starRating.paint(painter, option.rect, option.palette,
                          StarRating::ReadOnly);
     } else {
         QItemDelegate::paint(painter, option, index);
     }
 }

 QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                              const QModelIndex &index) const
 {
     if (qVariantCanConvert<StarRating>(index.data())) {
         StarRating starRating = qVariantValue<StarRating>(index.data());
         return starRating.sizeHint();
     } else {
         return QItemDelegate::sizeHint(option, index);
     }
 }

 QWidget *StarDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const

 {
     if (qVariantCanConvert<StarRating>(index.data())) {
         StarEditor *editor = new StarEditor(parent);
         connect(editor, SIGNAL(editingFinished()),
                 this, SLOT(commitAndCloseEditor()));
         return editor;
     } else {
         return QItemDelegate::createEditor(parent, option, index);
     }
 }

 void StarDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
 {
     if (qVariantCanConvert<StarRating>(index.data())) {
         StarRating starRating = qVariantValue<StarRating>(index.data());
         StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
         starEditor->setStarRating(starRating);
     } else {
         QItemDelegate::setEditorData(editor, index);
     }
 }

 void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
 {
     if (qVariantCanConvert<StarRating>(index.data())) {
         StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
         model->setData(index, qVariantFromValue(starEditor->starRating()));
     } else {
         QItemDelegate::setModelData(editor, model, index);
     }
 }

 void StarDelegate::commitAndCloseEditor()
 {
     StarEditor *editor = qobject_cast<StarEditor *>(sender());
     emit commitData(editor);
     emit closeEditor(editor);
 } 
