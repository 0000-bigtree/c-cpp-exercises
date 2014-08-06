/**
 * @file
 * EmailProtocolAnalysis<br/>
 * Imap4Analyser.h<br/>
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

#ifndef SMTPANALYSER_H
#define	SMTPANALYSER_H

#include<string>
#include"Analyser.h"
#include"Common.h"

namespace bigtree_analysis
{

    /**
     * IMAP4协议分析器
     */
    class Imap4Analyser : public Analyser
    {
    public:
        /**
         * 构造函数
         */
        Imap4Analyser();

        void analyse(const char* content, size_t length, const SessionId& sessionId);

        /**
         * 析构函数
         */
        virtual ~Imap4Analyser();

    private:
        /**
         * 记录日志
         * @param sessionInfoP 向待记录的会话信息的指针
         */
        void log(SessionInfo* sessionInfoP);

        /**
         * 协议ID
         */
        static const int ID_PROTOCOL = 5;

        /**
         * 标准端口
         */
        static const int STANDARD_PORT = 143;

        /**
         * SSH/TLS端口(安全端口)
         */
        static const int SSH_PORT = 993;

        /**
         * 日志对象
         */
        const static log4cplus::Logger LOGGER;
        /**************************LOGIN COMMAND*******************************/
        /**
         * LOGIN 命令ID
         */
        const static int ID_COMMAND_LOGIN = 500100;

        /**
         * LOGIN 命令
         */
        const static std::string COMMAND_LOGIN;

        const static int ID_RESPONSE_LOGIN_OK = 500101;

        /**
         * LOGIN OK
         */
        const static std::string RESPONSE_LOGIN_OK;

        const static int STATUS_RESPONSE_LOGIN_OK = 1;

        const static int ID_RESPONSE_LOGIN_NO = 500102;

        /**
         * LOGIN NO
         */
        const static std::string RESPONSE_LOGIN_NO;


        const static int STATUS_RESPONSE_LOGIN_NO = 2;

        const static int ID_RESPONSE_LOGIN_BAD = 500103;

        /**
         * LOGIN BAD
         */
        const static std::string RESPONSE_LOGIN_BAD;

        const static int STATUS_RESPONSE_LOGIN_BAD = 4;

        const static int ID_RESPONSE_LOGIN_UNKNOW = 500199;

        const static int STATUS_RESPONSE_LOGIN_UNKNOW = -2;

        const static int STATUS_RESPONSE_LOGIN_LOGOUT = 3;

        const static int PROPS_KEY_USER_NAME = 1;

        const static int PROPS_KEY_TAG = 2;

        const static std::string GENERAL_TAG;

        /*************************LOGOUT COMMAND*******************************/
        /**
         * LOGOUT 命令ID
         */
        const static int ID_COMMAND_LOGOUT = 500200;

        /**
         * LOGOUT 命令
         */
        const static std::string COMMAND_LOGOUT;

        const static int ID_RESPONSE_LOGOUT_OK = 500201;

        const static std::string RESPONSE_LOGOUT_OK;

        const static int ID_RESPONSE_LOGOUT_BAD = 500202;

        const static std::string RESPONSE_LOGOUT_BAD;

        /**
         * 最小可分析的长度
         */
        const static size_t MIN_LENGTH = 6;
    };
}

#endif	/* SNMPANALYSER_H */


