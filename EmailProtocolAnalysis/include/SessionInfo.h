/**
 * @file 
 * EmailProtocolAnalysis<br/>
 * SessionInfo.h<br/>
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

#ifndef SMTPINFO_H
#define	SMTPINFO_H

#include<string>
#include<vector>
#include"Common.h"
#include"CmdInfo.h"

namespace 
{

    /**
     * SMTP信息类<p/>
     * 包含了一个SMTP会话的基本信息
     */
    class SessionInfo
    {
    public:
        /**
         * 构造函数
         */
        SessionInfo();

        /**
         * 拷贝构造函数
         * @param original 待拷贝的会话信息对象
         */
        SessionInfo(const SessionInfo& original);

        /**
         * 析构函数
         */
        virtual ~SessionInfo();

        unsigned int getServerIp() const
        {
            return serverIp;
        }

        void setServerIp(const unsigned int& serverIp)
        {
            this->serverIp = serverIp;
        }

        unsigned int getServerPort() const
        {
            return serverPort;
        }

        void setServerPort(const unsigned int& serverPort)
        {
            this->serverPort = serverPort;
        }

        unsigned int getClientIp() const
        {
            return clientIp;
        }

        void setClientIp(const unsigned int& clientIp)
        {
            this->clientIp = clientIp;
        }
        
        unsigned int getClientPort() const
        {
            return clientPort;
        }

        void setClientPort(const unsigned int& clientPort)
        {
            this->clientPort = clientPort;
        }

        /**
         * 获取收到数据包数
         * @return  unsigned long, 收到数据包个数
         */
        unsigned long getPacketsCount() const
        {
            return packetsCount;
        }

        /**
         * 设置收到数据包个数
         * @param packetsCount 数据包个数
         */
        void setPacketsCount(const unsigned long &packetsCount)
        {
            this->packetsCount = packetsCount;
        }

        /**
         * 获取收到字节数
         * @return  unsigned long, 收到字节数
         */
        unsigned long getBytesCount()
        {
            return bytesCount;
        }

        /**
         * 设置收到字节数
         * @param bytesCount 收到字节数
         */
        void setBytesCount(const unsigned long &bytesCount)
        {
            this->bytesCount = bytesCount;
        }

        /**
         * 获取命令序列
         * @return std::vector<::CmdInfo*>，命令序列
         */
        const std::vector<::CmdInfo*>* getCmds() const;

        /**
         * 设置命令序列
         * @param cmds，命令序列
         */
        void setCmds(const std::vector<::CmdInfo*> &cmds);

        /**
         * 向命令序列添加命令
         * @param cmd 命令
         */
        void addCmd(::CmdInfo* cmd);

    private:

        unsigned int serverIp;

        unsigned int serverPort;

        unsigned int clientIp;

        unsigned int clientPort;

        /**
         * 收到数据包数
         */
        unsigned long packetsCount;

        /**
         * 收到字节数
         */
        unsigned long bytesCount;

        /**
         * 命令序列
         */
        std::vector<::CmdInfo*>* cmdsP;

        /**
         * 日志对象
         */
        const static log4cplus::Logger LOGGER;

    };
}
#endif	/* SMTPINFO_H */
