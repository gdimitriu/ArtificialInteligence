/*
 * numbereditorplugin.cpp
 *
 *  @Copyright 2023 Gabriel Dimitriu
 * All rights reserved.
 *
 * This file is part of Neural Network Simulator project.

 * Neural Network Simulator is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * Neural Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with Neural Network Simulator; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "numbereditor.h"
#include "numbereditorplugin.h"

#include <QtPlugin>

NumberEditorPlugin::NumberEditorPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void NumberEditorPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool NumberEditorPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *NumberEditorPlugin::createWidget(QWidget *parent)
{
    return new NumberEditor(parent);
}

QString NumberEditorPlugin::name() const
{
    return QLatin1String("NumberEditor");
}

QString NumberEditorPlugin::group() const
{
    return tr("Image Manipulation Widgets");
}

QIcon NumberEditorPlugin::icon() const
{
    return QIcon();
}

QString NumberEditorPlugin::toolTip() const
{
    return tr("A graphical number editor widget");
}

QString NumberEditorPlugin::whatsThis() const
{
    return tr("A graphical number editor widget for NSS");
}

bool NumberEditorPlugin::isContainer() const
{
    return false;
}

QString NumberEditorPlugin::domXml() const
{
    return QLatin1String("<widget class=\"NumberEditor\" name=\"numberEditor\">\n</widget>\n");
}

QString NumberEditorPlugin::includeFile() const
{
    return QLatin1String("<numbereditor.h>");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(NumberEditorPlugin, NumberEditorPlugin)
#endif // QT_VERSION < 0x050000
