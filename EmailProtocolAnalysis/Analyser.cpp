/**
 * @file
 * EmailProtocolAnalysis<br/>
 * Analyser.cpp<br/>
 * Copyright (c) bigtree 2011-2014. All rights reserved.
 * @date 2011-07-04
 * @author bigtree
 * @version 1.0.0<p/>
 * <b>&nbsp;&nbsp;&nbsp;历史:</b>
 * <pre>
 *        ver    date       author        desc
 *        1.0.0  2011-07-04 bigtree           created
 * </pre>
 */

#include"Analyser.h"

using std::map;
using std::pair;
using log4cplus::Logger;

namespace bigtree_analysis
{
    const Logger Analyser::LOGGER = Logger::getInstance("bigtree_analysis.Analyser");

    Analyser::Analyser() : sessionInfosMapP(0)
    {
        LOG4CPLUS_TRACE(LOGGER, "constructor enter");
        LOG4CPLUS_TRACE(LOGGER, "constructor leave");
    }

    Analyser::~Analyser()
    {
        LOG4CPLUS_TRACE(LOGGER, "deconstructor enter");
        if (0 != sessionInfosMapP && !sessionInfosMapP->empty()) {
            for (map<SessionId, SessionInfo*>::iterator it = sessionInfosMapP->begin();
                    it != sessionInfosMapP->end(); it++) {
                delete it->second;
            }
        }
        delete sessionInfosMapP;
        LOG4CPLUS_TRACE(LOGGER, "deconstructor leave");
    }

    SessionInfo* Analyser::getSessionInfo(const SessionId& sessionId)
    {
        LOG4CPLUS_TRACE(LOGGER, "getSessionInfo enter, sessionId=" << sessionId);
        if (0 == sessionInfosMapP) {
            LOG4CPLUS_TRACE(LOGGER, "sessionInfosMap uninitialized");
            LOG4CPLUS_TRACE(LOGGER, "getSessionInfo leave");
            return 0;
        }
        // 查找元素
        map<SessionId, SessionInfo*>::iterator it = sessionInfosMapP->find(sessionId);

        // 未找到元素
        if (it == sessionInfosMapP->end()) {
            LOG4CPLUS_TRACE(LOGGER, "can't find element");
            LOG4CPLUS_TRACE(LOGGER, "getSessionInfo leave");
            return 0;
        }
       // 找到元素
        else {
            LOG4CPLUS_TRACE(LOGGER, "found element");
            LOG4CPLUS_TRACE(LOGGER, "getSessionInfo leave");
            return it->second;
        }
    }

    void Analyser::putSessionInfo(const SessionId& sessionId, SessionInfo& sessionInfo)
    {
        LOG4CPLUS_TRACE(LOGGER, "putSessionInfo enter, sessionId=" << sessionId);
        // 先分配内存
        if (0 == sessionInfosMapP) {
            LOG4CPLUS_TRACE(LOGGER, "sessionInfosMap do initialized");
            sessionInfosMapP = new map<SessionId, SessionInfo*> ();
        }

        // 放入Map中
        sessionInfosMapP->insert(pair<SessionId, SessionInfo*> (sessionId, &sessionInfo));
        LOG4CPLUS_TRACE(LOGGER, "putSessionInfo leave");
    }

    bool Analyser::containsSessionInfo(const SessionId& sessionId) const
    {
        LOG4CPLUS_TRACE(LOGGER, "containsSessionInfo enter, sessionId=" << sessionId);
        if (0 == sessionInfosMapP) {
            LOG4CPLUS_TRACE(LOGGER, "sessionInfosMap uninitialized");
            LOG4CPLUS_TRACE(LOGGER, "containsSessionInfo leave");
            return false;
        }
        else {
            LOG4CPLUS_TRACE(LOGGER, "containsSessionInfo leave");
            return 0 < sessionInfosMapP->count(sessionId);
        }
    }

    void Analyser::removeSessionInfo(const SessionId& sessionId)
    {
        LOG4CPLUS_TRACE(LOGGER, "removeSessionInfo enter, sessionId=" << sessionId);
        if (0 == sessionInfosMapP) {
            LOG4CPLUS_TRACE(LOGGER, "sessionInfosMap uninitialized");
        }
        else {
            // 查找元素
            map<SessionId, SessionInfo*>::iterator it = sessionInfosMapP->find(sessionId);

            // 未找到元素
            if (it == sessionInfosMapP->end()) {
                LOG4CPLUS_TRACE(LOGGER, "can't find element");
            }
            // 找到元素
            else {
                LOG4CPLUS_TRACE(LOGGER, "found element");
                delete it->second;
                sessionInfosMapP->erase(it);
            }
        }
        LOG4CPLUS_TRACE(LOGGER, "removeSessionInfo leave");
    }
}
