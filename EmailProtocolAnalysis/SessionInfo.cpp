/**
 * @file
 * EmailProtocolAnalysis<br/>
 * SessionInfo.cpp<br/>
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

#include"SessionInfo.h"

using std::string;
using std::vector;

using log4cplus::Logger;
using bigtree_analysis::CmdInfo;

namespace bigtree_analysis
{
    const Logger SessionInfo::LOGGER = Logger::getInstance(".SessionInfo");

    SessionInfo::SessionInfo() : packetsCount(0), bytesCount(0), cmdsP(0)
    {
        LOG4CPLUS_TRACE(LOGGER, "constructor enter");
        LOG4CPLUS_TRACE(LOGGER, "constructor leave");
    }

    SessionInfo::SessionInfo(const SessionInfo& original) : packetsCount(original.packetsCount), bytesCount(original.bytesCount), cmdsP(0)
    {
        LOG4CPLUS_TRACE(LOGGER, "copy constructor enter");
        if (0 != original.cmdsP) {
            cmdsP = new vector<CmdInfo*>(*original.cmdsP);
        }
        LOG4CPLUS_TRACE(LOGGER, "copy constructor leave");
    }

    SessionInfo::~SessionInfo()
    {
        LOG4CPLUS_TRACE(LOGGER, "deconstructor enter");

        if (0 != cmdsP && !cmdsP->empty()) {
            for (vector<CmdInfo*>::const_iterator it = cmdsP->begin(); it < cmdsP->end(); ++it) {
                delete *it;
            }
        }
        LOG4CPLUS_TRACE(LOGGER, "deconstructor leave");
    }

    void SessionInfo::addCmd(CmdInfo* cmd)
    {
        if (0 == cmdsP) {
            cmdsP = new vector<CmdInfo*>;
        }
        cmdsP->push_back(cmd);
    }

    const std::vector<CmdInfo*>* SessionInfo::getCmds() const
    {
        return cmdsP;
    }

    void SessionInfo::setCmds(const vector<CmdInfo*> &cmds)
    {
        // 先释放原来的内存
        if (0 != cmdsP) {
            // 释放其中元素的内存
            if (!cmdsP->empty()) {
                for (vector<CmdInfo*>::const_iterator it = cmdsP->begin();
                        it < cmdsP->end(); ++it) {
                    //释放该元素的内存
                    delete *it;
                }
            }
            delete cmdsP;
        }

        // 置为无效指针
        if (cmds.empty()) {
            cmdsP = 0;
        }
        // 拷贝其中的元素
        else {
            cmdsP = new vector<CmdInfo*>();
            for (vector<CmdInfo*>::const_iterator it = cmds.begin();
                    it < cmds.end(); ++it) {
                cmdsP->push_back(*it);
            }
        }
    }
}
