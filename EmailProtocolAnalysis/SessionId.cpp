/**
 * @file
 * EmailProtocolAnalysis<br/>
 * SessionId.cpp<br/>
 * Copyright (c) bigtree 2011-2014. All rights reserved.
 * @date 2011-07-06
 * @author bigtree
 * @version 1.0.0<p/>
 * <b>&nbsp;&nbsp;&nbsp;历史:</b>
 * <pre>
 *        ver    date       author        desc
 *        1.0.0  2011-07-06 bigtree           created
 * </pre>
 */

#include"SessionId.h"

using std::string;
using log4cplus::Logger;

namespace bigtree_analysis
{
    // 初始化静态变量
    const string SessionId::SEPARATOR = "_";

    const Logger SessionId::LOGGER = Logger::getInstance(".SessionId");

    SessionId::SessionId(const unsigned int& sourceIp, const unsigned int& sourcePort,
                         const unsigned int& destIp, const unsigned int& destPort,
                         const int& protocolId) : _sourceIp(sourceIp), _sourcePort(sourcePort), _destIp(destIp), _destPort(destPort), _protocolId(protocolId)
    {
        LOG4CPLUS_TRACE(LOGGER, "constructor enter, sourceIp=" << sourceIp << ", sourcePort=" << sourcePort << ", destIp=" << destIp << ", destPort=" << destPort << ", protocolId=" << protocolId);
        assignKey();
        LOG4CPLUS_TRACE(LOGGER, "key=" << *keyP);
        LOG4CPLUS_TRACE(LOGGER, "constructor leave");
    }

    /**
     * 构造拷贝函数
     * @param original 待拷贝的会话ID对象
     */
    SessionId::SessionId(const SessionId& original) : _sourceIp(original._sourceIp), _sourcePort(original._sourcePort), _destIp(original._destIp), _destPort(original._destPort), _protocolId(original._protocolId)
    {
        LOG4CPLUS_TRACE(LOGGER, "copy constructor enter, original.sourceIp=" << original._sourceIp << ", original.sourcePort=" << original._sourcePort << ", original.destIp=" << original._destIp << ", original.destPort=" << original._destPort << ", original.protocolId=" << original._protocolId);
        assignKey();
        LOG4CPLUS_TRACE(LOGGER, "key=" << *keyP);
        LOG4CPLUS_TRACE(LOGGER, "copy constructor leave");
    }

    /**
     * 赋值操作符
     * @param original 赋值的右操作数，来源
     * @return SessionId&，被赋值的会话ID
     */
    SessionId& SessionId::operator=(const SessionId & original)
    {
        LOG4CPLUS_TRACE(LOGGER, "operator= enter, original.sourceIp=" << original._sourceIp << ", original.sourcePort=" << original._sourcePort << ", original.destIp=" << original._destIp << ", original.destPort=" << original._destPort << ", original.protocolId=" << original._protocolId);
        if (*keyP != *original.keyP) {
            LOG4CPLUS_TRACE(LOGGER, "operator= do copy");
            this->_sourceIp = original._sourceIp;
            this->_sourcePort = original._sourcePort;
            this->_destIp = original._destIp;
            this->_destPort = original._destPort;
            this->_protocolId = original._protocolId;
            if (0 != keyP) {
                delete keyP;
            }
            assignKey();
        }
        LOG4CPLUS_TRACE(LOGGER, "key=" << *keyP);
        LOG4CPLUS_TRACE(LOGGER, "operator= leave");
        return *this;
    }

    SessionId::~SessionId()
    {
        LOG4CPLUS_TRACE(LOGGER, "deconstructor enter");
        // 释放作为KEY的字符串
        delete keyP;
        LOG4CPLUS_TRACE(LOGGER, "deconstructor leave");
    }

    bool SessionId::operator<(const SessionId& another) const
    {
        return *this->keyP < *another.keyP;
    }

    /**
     * 等于操作符
     * @param another 与其比较的另一个会话ID对象
     * @return 是否等于另一个对象
     */
    bool SessionId::operator ==(const SessionId& another) const
    {
        return *this->keyP == *another.keyP;
    }
}
