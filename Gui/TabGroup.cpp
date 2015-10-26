/* ***** BEGIN LICENSE BLOCK *****
 * This file is part of Natron <http://www.natron.fr/>,
 * Copyright (C) 2015 INRIA and Alexandre Gauthier-Foichat
 *
 * Natron is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Natron is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Natron.  If not, see <http://www.gnu.org/licenses/gpl-2.0.html>
 * ***** END LICENSE BLOCK ***** */

// ***** BEGIN PYTHON BLOCK *****
// from <https://docs.python.org/3/c-api/intro.html#include-files>:
// "Since Python may define some pre-processor definitions which affect the standard headers on some systems, you must include Python.h before any standard headers are included."
#include <Python.h>
// ***** END PYTHON BLOCK *****

#include "TabGroup.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "Engine/KnobTypes.h"

#define NATRON_FORM_LAYOUT_LINES_SPACING 0


using std::make_pair;
using namespace Natron;


TabGroup::TabGroup(QWidget* parent)
: QFrame(parent)
{
    setFrameShadow(QFrame::Raised);
    setFrameShape(QFrame::Box);
    QHBoxLayout* frameLayout = new QHBoxLayout(this);
    _tabWidget = new QTabWidget(this);
    frameLayout->addWidget(_tabWidget);
    setVisible(false);
}

void
TabGroup::onGroupSecretChanged()
{
    KnobSignalSlotHandler* handler = qobject_cast<KnobSignalSlotHandler*>(sender());
    if (!handler) {
        return;
    }
    KnobGroup* knob = dynamic_cast<KnobGroup*>(handler->getKnob().get());
    if (!knob) {
        return;
    }
    int found = -1;
    for (U32 i = 0 ; i < _tabs.size(); ++i) {
        if (_tabs[i].first.lock().get() == knob) {
            found = (int)i;
            break;
        }
    }
    if (found == -1) {
        return;
    }
    
    bool secret = handler->getKnob()->getIsSecret();
    if (secret) {
        _tabWidget->removeTab(found);
        if (_tabWidget->count() == 0) {
            hide();
        }
    } else {
        _tabWidget->insertTab(found, _tabs[found].second.tabContainer, _tabs[found].second.tabContainer->objectName());
        if (!isVisible()) {
            setVisible(true);
        }
    }
}

QGridLayout*
TabGroup::addTab(const boost::shared_ptr<KnobGroup>& group,const QString& name)
{
    
    QWidget* tab = 0;
    QGridLayout* tabLayout = 0;
    for (std::size_t i = 0; i < _tabs.size(); ++i) {
        boost::shared_ptr<KnobGroup> k = _tabs[i].first.lock();
        if (!k) {
            continue;
        }
        if (k == group) {
            tab = _tabs[i].second.tabContainer;
            tabLayout = _tabs[i].second.gridLayout;
            break;
        }
    }
   
    if (!tab) {
        QObject::connect(group->getSignalSlotHandler().get(), SIGNAL(secretChanged()), this, SLOT(onGroupSecretChanged()));
        tab = new QWidget(_tabWidget);
        tab->setObjectName(name);
        tabLayout = new QGridLayout(tab);
        tabLayout->setColumnStretch(1, 1);
        //tabLayout->setContentsMargins(0, 0, 0, 0);
        tabLayout->setSpacing(NATRON_FORM_LAYOUT_LINES_SPACING); // unfortunately, this leaves extra space when parameters are hidden
        if (!group->getIsSecret()) {
            _tabWidget->addTab(tab,name);
            if (!isVisible()) {
                setVisible(true);
            }
        }
        TabData d;
        d.gridLayout = tabLayout;
        d.tabContainer = tab;
        _tabs.push_back(std::make_pair(group, d));
    }
    assert(tabLayout);
    return tabLayout;
}

void
TabGroup::removeTab(KnobGroup* group)
{
    int i = 0;
    for (std::vector<std::pair< boost::weak_ptr<KnobGroup>,TabData > >::iterator it = _tabs.begin();
         it != _tabs.end(); ++it) {
        boost::shared_ptr<KnobGroup> k = it->first.lock();
        if (!k) {
            continue;
        }
        if (k.get() == group) {
            _tabWidget->removeTab(i);
            _tabs.erase(it);
            break;
        }
    }

}
