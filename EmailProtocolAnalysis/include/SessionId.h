/**
 * @file 
 * EmailProtocolAnalysis<br/>
 * SessionId.h<br/>
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

#ifndef SESSIONID_H
#define	SESSIONID_H

#include<iostream>
#include<sstream>
#include<string>

#include"Common.h"

namespace 
{

    /**
     * 会话标识，每个会话的唯一标识
     */
    class SessionId
    {
    public:
        /**
         * 构造函数
         * @param sourceIp 源IP
         * @param sourcePort 源端口号
         * @param destIp 目的IP
         * @param destPort 目的端口号
         * @param protocolId 协议ID
         */
        SessionId(const unsigned int& sourceIp, const unsigned int& sourcePort,
                  const unsigned int& destIp, const unsigned int& destPort,
                  const int& protocolId);

        /**
         * 构造拷贝函数
         * @param original 待拷贝的会话ID对象
         */
        SessionId(const SessionId& original);

        /**
         * 赋值操作符
         * @param original 赋值的右操作数，来源
         * @return SessionId&，被赋值的会话ID 
         */
        SessionId& operator=(const SessionId& original);

        /**
         * 小于操作符
         * @param another 与其比较的另一个会话ID对象
         * @return 是否小于另一个对象
         */
        bool operator<(const SessionId& another) const;

        /**
         * 等于操作符
         * @param another 与其比较的另一个会话ID对象
         * @return 是否等于另一个对象
         */
        bool operator ==(const SessionId& another) const;

        /**
         * 输出操作符
         * @param out 输出流
         * @param sessionId 待输出的会话ID对象
         * @return std::ostream&，输出流
         */
        friend std::ostream& operator<<(std::ostream& out, const SessionId& sessionId)
        {
            out << "[sourceIp=" << sessionId._sourceIp << ", sourcePort=" << sessionId._sourcePort
                    << ", destIp=" << sessionId._destIp << ", destPort=" << sessionId._destPort
                    << ", protocolId=" << sessionId._protocolId << "]";
            return out;
        }

        /**
         * 析构函数
         */
        virtual ~SessionId();

        /**
         * 获取源IP(客户端IP)
         * @return  int, 源IP地址
         */
        unsigned int getSourceIp() const
        {
            return _sourceIp;
        }

        /**
         * 获取源端口
         * @return int，源端口
         */
        unsigned int getSourcePort() const
        {
            return _sourcePort;
        }

        /**
         * 获取目的IP(服务器端IP)
         * @return unsigned int, 目的IP
         */
        unsigned int getDestIp() const
        {
            return _destIp;
        }

        /**
         * 获取目的端口
         * @return int，目的端口
         */
        unsigned int getDestPort() const
        {
            return _destPort;
        }

        /**
         * 获取协议ID
         */
        int getProtocolId() const
        {
            return _protocolId;
        }

    private:
        /**
         * 源IP
         */
        unsigned int _sourceIp;

        /**
         * 源端口
         */
        unsigned int _sourcePort;

        /**
         * 目的IP
         */
        unsigned int _destIp;

        /**
         * 目的端口
         */
        unsigned int _destPort;

        /**
         * 协议ID
         */
        int _protocolId;

        // 键值
        std::string* keyP;

        /**
         * 作为key的分隔符
         */
        const static std::string SEPARATOR;

        /**
         * 日志对象
         */
        const static log4cplus::Logger LOGGER;

    private:

        void assignKey()
        {
            std::ostringstream oss;
            oss << _sourceIp << SEPARATOR << _sourcePort << SEPARATOR << _destIp << SEPARATOR << _destPort << SEPARATOR << _protocolId;
            // 生成字符串，用来作KEY
            keyP = new std::string(oss.str());
        }
    };
}

#endif	/* SESSIONID_H */

