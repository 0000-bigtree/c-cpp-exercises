/**
 * @file
 * EmailProtocolAnalysis<br/>
 * CmdInfo.cpp<br/>
 * Copyright (c) bigtree 2011-2014. All rights reserved.
 * @date 2011-07-05
 * @author bigtree
 * @version 1.0.0<p/>
 * <b>&nbsp;&nbsp;&nbsp;历史:</b>
 * <pre>
 *        ver    date       author        desc
 *        1.0.0  2011-07-05 bigtree           created
 * </pre>
 */

#include"CmdInfo.h"

using std::map;
using log4cplus::Logger;

namespace bigtree_analysis
{
    const Logger CmdInfo::LOGGER = Logger::getInstance(".CmdInfo");

    CmdInfo::CmdInfo() : startMillis(UNKONW), responseMillis(UNKONW), cmdId(UNKONW), statusId(UNKONW), propsP(0)
    {
        LOG4CPLUS_TRACE(LOGGER, "constructor enter");
        LOG4CPLUS_TRACE(LOGGER, "constructor leave");
    }

    CmdInfo::CmdInfo(const CmdInfo& original) : startMillis(original.startMillis), responseMillis(original.responseMillis), cmdId(original.cmdId), statusId(original.statusId), propsP(0)
    {
        LOG4CPLUS_TRACE(LOGGER, "copy constructor enter");
        if (0 != original.propsP) {
            this->propsP = new map<int, void*>(*original.propsP);
        }
        LOG4CPLUS_TRACE(LOGGER, "copy constructor leave");
    }

    CmdInfo& CmdInfo::operator=(const CmdInfo& original)
    {
        LOG4CPLUS_TRACE(LOGGER, "operator= enter");
        this->startMillis = original.startMillis;
        this->responseMillis = original.responseMillis;
        this->cmdId = original.cmdId;
        this->statusId = original.statusId;
        if (0 != original.propsP) {
            delete propsP;
            propsP = new map<int, void*>(*original.propsP);
        }
        else {
            if (0 != propsP) {
                delete propsP;
                propsP = 0;
            }
        }
        LOG4CPLUS_TRACE(LOGGER, "operator= leave");
        return *this;
    }

    /**
     * 析构函数
     */
    CmdInfo::~CmdInfo()
    {
        LOG4CPLUS_TRACE(LOGGER, "deconstructor enter");
        delete propsP;
        LOG4CPLUS_TRACE(LOGGER, "deconstructor leave");
    }

    void CmdInfo::addProperty(std::pair<int, void*>& property)
    {
        if (0 == propsP) {
            propsP = new map<int, void*>();
        }
        else {
            const map<int, void*>::iterator it = propsP->find(property.first);
            if (propsP->end() != it) {
                propsP->erase(it);
            }
        }
        propsP->insert(property);
    }

    void CmdInfo::removeProperty(int propertyKey)
    {
        if (0 != propsP) {
            const map<int, void*>::iterator it = propsP->find(propertyKey);
            if (propsP->end() != it) {
                propsP->erase(it);
            }
        }
    }

    void* CmdInfo::getProperty(int propertyKey)
    {
        if (0 != propsP) {
            const map<int, void*>::iterator it = propsP->find(propertyKey);
            if (propsP->end() != it) {
                return it->second;
            }
        }
        return 0;
    }
}
