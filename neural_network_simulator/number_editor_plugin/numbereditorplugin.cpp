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
