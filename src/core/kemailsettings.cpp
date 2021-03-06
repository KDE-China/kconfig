/*
 * Copyright (c) 2000 Alex Zepeda <zipzippy@sonic.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "kemailsettings.h"

#include <kconfig.h>
#include <kconfiggroup.h>

class KEMailSettingsPrivate
{
public:
    KEMailSettingsPrivate() : m_pConfig(nullptr) {}
    ~KEMailSettingsPrivate()
    {
        delete m_pConfig;
    }
    KConfig *m_pConfig;
    QStringList profiles;
    QString m_sDefaultProfile, m_sCurrentProfile;
};

QString KEMailSettings::defaultProfileName() const
{
    return p->m_sDefaultProfile;
}

QString KEMailSettings::getSetting(KEMailSettings::Setting s) const
{
    KConfigGroup cg(p->m_pConfig, QStringLiteral("PROFILE_") + p->m_sCurrentProfile);
    switch (s) {
    case ClientProgram: {
        return cg.readEntry("EmailClient");
    }
    case ClientTerminal: {
        return cg.readEntry("TerminalClient", QVariant(false)).toString();
    }
    case RealName: {
        return cg.readEntry("FullName");
    }
    case EmailAddress: {
        return cg.readEntry("EmailAddress");
    }
    case ReplyToAddress: {
        return cg.readEntry("ReplyAddr");
    }
    case Organization: {
        return cg.readEntry("Organization");
    }
    case OutServer: {
        return cg.readEntry("OutgoingServer");
    }
    case OutServerLogin: {
        return cg.readEntry("OutgoingUserName");
    }
    case OutServerPass: {
        return cg.readEntry("OutgoingPassword");
    }
    case OutServerType: {
        return cg.readEntry("OutgoingServerType");
    }
    case OutServerCommand: {
        return cg.readEntry("OutgoingCommand");
    }
    case OutServerTLS: {
        return cg.readEntry("OutgoingServerTLS", QVariant(false)).toString();
    }
    case InServer: {
        return cg.readEntry("IncomingServer");
    }
    case InServerLogin: {
        return cg.readEntry("IncomingUserName");
    }
    case InServerPass: {
        return cg.readEntry("IncomingPassword");
    }
    case InServerType: {
        return cg.readEntry("IncomingServerType");
    }
    case InServerMBXType: {
        return cg.readEntry("IncomingServerMBXType");
    }
    case InServerTLS: {
        return cg.readEntry("IncomingServerTLS", QVariant(false)).toString();
    }
    };
    return QString();
}
void KEMailSettings::setSetting(KEMailSettings::Setting s, const QString  &v)
{
    KConfigGroup cg(p->m_pConfig, QStringLiteral("PROFILE_") + p->m_sCurrentProfile);
    switch (s) {
    case ClientProgram: {
        cg.writePathEntry("EmailClient", v);
        break;
    }
    case ClientTerminal: {
        cg.writeEntry("TerminalClient", (v == QLatin1String("true")));
        break;
    }
    case RealName: {
        cg.writeEntry("FullName", v);
        break;
    }
    case EmailAddress: {
        cg.writeEntry("EmailAddress", v);
        break;
    }
    case ReplyToAddress: {
        cg.writeEntry("ReplyAddr", v);
        break;
    }
    case Organization: {
        cg.writeEntry("Organization", v);
        break;
    }
    case OutServer: {
        cg.writeEntry("OutgoingServer", v);
        break;
    }
    case OutServerLogin: {
        cg.writeEntry("OutgoingUserName", v);
        break;
    }
    case OutServerPass: {
        cg.writeEntry("OutgoingPassword", v);
        break;
    }
    case OutServerType: {
        cg.writeEntry("OutgoingServerType", v);
        break;
    }
    case OutServerCommand: {
        cg.writeEntry("OutgoingCommand", v);
        break;
    }
    case OutServerTLS: {
        cg.writeEntry("OutgoingServerTLS", (v == QLatin1String("true")));
        break;
    }
    case InServer: {
        cg.writeEntry("IncomingServer", v);
        break;
    }
    case InServerLogin: {
        cg.writeEntry("IncomingUserName", v);
        break;
    }
    case InServerPass: {
        cg.writeEntry("IncomingPassword", v);
        break;
    }
    case InServerType: {
        cg.writeEntry("IncomingServerType", v);
        break;
    }
    case InServerMBXType: {
        cg.writeEntry("IncomingServerMBXType", v);
        break;
    }
    case InServerTLS: {
        cg.writeEntry("IncomingServerTLS", (v == QLatin1String("true")));
        break;
    }
    };
    cg.sync();
}

void KEMailSettings::setDefault(const QString &s)
{
    p->m_pConfig->group("Defaults").writeEntry("Profile", s);
    p->m_pConfig->sync();
    p->m_sDefaultProfile = s;

}

void KEMailSettings::setProfile(const QString &s)
{
    QString groupname = QStringLiteral("PROFILE_");
    groupname.append(s);
    p->m_sCurrentProfile = s;
    if (!p->m_pConfig->hasGroup(groupname)) { // Create a group if it doesn't exist
        KConfigGroup cg(p->m_pConfig, groupname);
        cg.writeEntry("ServerType", QString());
        p->profiles += s;
    }
}

#ifndef KDE_NO_DEPRECATED
QString KEMailSettings::currentProfileName() const
{
    return p->m_sCurrentProfile;
}
#endif

QStringList KEMailSettings::profiles() const
{
    return p->profiles;
}

KEMailSettings::KEMailSettings()
    : p(new KEMailSettingsPrivate())
{
    p->m_sCurrentProfile.clear();

    p->m_pConfig = new KConfig(QStringLiteral("emaildefaults"));

    const QStringList groups = p->m_pConfig->groupList();
    for (QStringList::ConstIterator it = groups.begin(); it != groups.end(); ++it) {
        if ((*it).startsWith(QLatin1String("PROFILE_"))) {
            p->profiles += (*it).mid(8, (*it).length());
        }
    }

    KConfigGroup cg(p->m_pConfig, "Defaults");
    p->m_sDefaultProfile = cg.readEntry("Profile", tr("Default"));
    if (!p->m_sDefaultProfile.isNull()) {
        if (!p->m_pConfig->hasGroup(QStringLiteral("PROFILE_") + p->m_sDefaultProfile)) {
            setDefault(tr("Default"));
        } else {
            setDefault(p->m_sDefaultProfile);
        }
    } else {
        if (!p->profiles.isEmpty()) {
            setDefault(p->profiles[0]);
        } else {
            setDefault(tr("Default"));
        }
    }
    setProfile(defaultProfileName());
}

KEMailSettings::~KEMailSettings()
{
    delete p;
}
