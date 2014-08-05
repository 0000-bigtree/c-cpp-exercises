/**
 * @file
 * EmailProtocolAnalysis<br/>
 * Imap4Analyser.cpp<br/>
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
#include<ctime>
#include"Imap4Analyser.h"

using std::string;
using std::vector;
using std::pair;
using log4cplus::Logger;

namespace bigtree_analysis
{
    // 初始化静态变量
    const Logger Imap4Analyser::LOGGER = Logger::getInstance(".Imap4Analyser");

    const string Imap4Analyser::GENERAL_TAG = "*";

    const string Imap4Analyser::COMMAND_LOGIN = "LOGIN ";

    const string Imap4Analyser::RESPONSE_LOGIN_OK = "OK ";

    const string Imap4Analyser::RESPONSE_LOGIN_NO = "NO ";

    const string Imap4Analyser::RESPONSE_LOGIN_BAD = "BAD ";

    const string Imap4Analyser::COMMAND_LOGOUT = "LOGOUT";

    const string Imap4Analyser::RESPONSE_LOGOUT_OK = "OK ";

    const string Imap4Analyser::RESPONSE_LOGOUT_BAD = "BAD ";

    Imap4Analyser::Imap4Analyser()
    {
        LOG4CPLUS_TRACE(LOGGER, "constructor enter");
        LOG4CPLUS_TRACE(LOGGER, "constructor leave");
    }

    void Imap4Analyser::analyse(const char* content, size_t length, const SessionId& sessionId)
    {
        LOG4CPLUS_TRACE(LOGGER, "********************analyse enter, content=" << content << ", length=" << length << ", sessionId=" << sessionId);

        // 长度至少要为最小可分析长度
        if (0 == content || MIN_LENGTH > length) {
            LOG4CPLUS_WARN(LOGGER, "invalid packet, content error or length error");
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        string str(content);

        // 查找第一个空格，将之前的内容视为标签
        size_t tagEndPos = str.find_first_of(' ');

        // 未找到空格，视为无效数据
        if (string::npos == tagEndPos) {
            LOG4CPLUS_WARN(LOGGER, "invalid packet, can't find blank character");
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        // 标签长度为零
        if (0 == tagEndPos) {
            LOG4CPLUS_WARN(LOGGER, "invalid packet, tag length is zero");
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        // 得到标签
        string tag = str.substr(0, tagEndPos);
        LOG4CPLUS_TRACE(LOGGER, "get tag, tag=" << tag);

        // 测试长度是否足够继续分析
        if (tagEndPos + 1 >= length) {
            LOG4CPLUS_WARN(LOGGER, "invalid packet, content deficient");
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        // 查找LOGIN命令
        size_t cmdOrResponseBeginPos = str.find(COMMAND_LOGIN, tagEndPos + 1);

        // 找到LOGIN命令
        if (string::npos != cmdOrResponseBeginPos && tagEndPos + 1 == cmdOrResponseBeginPos) {
            LOG4CPLUS_TRACE(LOGGER, "get LOGIN cmd");

            // 测试长度是否足够继续分析
            size_t cmdEndPos = cmdOrResponseBeginPos + COMMAND_LOGIN.size();
            if (cmdOrResponseBeginPos + 2 >= length) {
                LOG4CPLUS_WARN(LOGGER, "invalid packet, content deficient");
                LOG4CPLUS_TRACE(LOGGER, "analyse leave");
                return;
            }

            // 查找登录用户名
            size_t userNameEndPos = str.find_first_of(' ', cmdEndPos + 2);
            if (string::npos == userNameEndPos) {
                LOG4CPLUS_WARN(LOGGER, "invalid login cmd, can't find user name");
                LOG4CPLUS_TRACE(LOGGER, "analyse leave");
                return;
            }

            // 获取登录用户名
            string userName = str.substr(cmdEndPos, userNameEndPos - cmdEndPos);
            LOG4CPLUS_TRACE(LOGGER, "get LOGIN user name, userName=" << userName);

            // 查看是否已经有相关信息
            const bool contained = containsSessionInfo(sessionId);
            if (contained) {
                // 删除原来的信息
                removeSessionInfo(sessionId);
                LOG4CPLUS_WARN(LOGGER, "removed existing sessionInfo");
            }

            // 创建新的会话信息
            SessionInfo * const sessionInfoP = new SessionInfo();
            putSessionInfo(sessionId, *sessionInfoP);

            // 放入登录信息
            CmdInfo* cmdInfoP = new CmdInfo();
            cmdInfoP->setCmdId(ID_COMMAND_LOGIN);
            const time_t now = time(0);
            cmdInfoP->setStartMillis(now * 1000);

            // 放入命令标签
            int key = PROPS_KEY_TAG;
            pair<int, void*> propTag(key, new string(tag));
            cmdInfoP->addProperty(propTag);

            // 放入用户名
            key = PROPS_KEY_USER_NAME;
            pair<int, void*> propUserName(key, new string(userName));
            cmdInfoP->addProperty(propUserName);
            sessionInfoP->addCmd(cmdInfoP);

            // 设置服务端和客户端IP
            sessionInfoP->setServerIp(sessionId.getDestIp());
            sessionInfoP->setServerPort(sessionId.getDestPort());
            sessionInfoP->setClientIp(sessionId.getSourceIp());
            sessionInfoP->setClientPort(sessionId.getSourcePort());

            // 计算包数量和大小
            sessionInfoP->setPacketsCount(1);
            sessionInfoP->setBytesCount(54 + length);
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        LOG4CPLUS_TRACE(LOGGER, "can't find LOGIN cmd");

        // 查看是否已经有相关信息，是否已经LOGIN
        bool contained = containsSessionInfo(sessionId);

        // 当前包是否为响应命令
        bool isResponse = !contained;
        const SessionId* reverseIdP = 0;

        // 测试是否为响应包
        if (!contained) {
            reverseIdP = new SessionId(sessionId.getDestIp(), sessionId.getDestPort(), sessionId.getSourceIp(), sessionId.getSourcePort(), sessionId.getProtocolId());
            contained = containsSessionInfo(*reverseIdP);
            isResponse = contained;
        }

        // 非已知的会话包，丢弃
        if (!contained) {
            LOG4CPLUS_WARN(LOGGER, "ignored packed, can't find sessionInfo");
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
            return;
        }

        // 是已知的会话包，计算大小及包数
        SessionInfo * const sessionInfoP = getSessionInfo(isResponse ? *reverseIdP : sessionId);

        sessionInfoP->setPacketsCount((sessionInfoP->getPacketsCount() + 1));
        sessionInfoP->setBytesCount(sessionInfoP->getBytesCount() + 54 + length);

        // 非响应包，由客户端发向服务器
        if (!isResponse) {

            // 查找LOGINOUT命令
            cmdOrResponseBeginPos = str.find(COMMAND_LOGOUT, tagEndPos + 1);

            // 找到登出命令
            if (string::npos != cmdOrResponseBeginPos && tagEndPos + 1 == cmdOrResponseBeginPos) {
                LOG4CPLUS_TRACE(LOGGER, "get LOGOUT cmd");

                // 放入登出信息
                CmdInfo* cmdInfoP = new CmdInfo();
                cmdInfoP->setCmdId(ID_COMMAND_LOGOUT);
                const time_t now = time(0);
                cmdInfoP->setStartMillis(now * 1000);

                // 放入命令标签
                int key = PROPS_KEY_TAG;
                pair<int, void*> propTag(key, new string(tag));
                cmdInfoP->addProperty(propTag);

                sessionInfoP->addCmd(cmdInfoP);
            }
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
        }

            // 响应包，由服务器发向客户端
        else {
            // 先查找LOGIN的返回状态，先查看标签是否一致
            vector<CmdInfo*> cmds = (*(sessionInfoP->getCmds()));
            string* tagP = static_cast<string*> (cmds[0]->getProperty(PROPS_KEY_TAG));
            bool isSameTag = *tagP == tag;

            // 标签一致，是期待的LOGIN的响应
            if (isSameTag) {

                // 查找登录的命令响应
                const size_t responseBeginPos = tagEndPos + 1;
                int statusId = ID_RESPONSE_LOGIN_UNKNOW;

                // 尝试OK响应
                cmdOrResponseBeginPos = str.find(RESPONSE_LOGIN_OK, responseBeginPos);
                if (string::npos != cmdOrResponseBeginPos && cmdOrResponseBeginPos == responseBeginPos) {
                    statusId = ID_RESPONSE_LOGIN_OK;
                }
                else {
                    // 尝试NO响应
                    cmdOrResponseBeginPos = str.find(RESPONSE_LOGIN_NO, responseBeginPos);
                    if (string::npos != cmdOrResponseBeginPos && cmdOrResponseBeginPos == responseBeginPos) {
                        statusId = ID_RESPONSE_LOGIN_NO;
                    }
                    else {
                        // 尝试BAD响应
                        cmdOrResponseBeginPos = str.find(RESPONSE_LOGIN_BAD, responseBeginPos);
                        if (string::npos != cmdOrResponseBeginPos && cmdOrResponseBeginPos == responseBeginPos) {
                            statusId = ID_RESPONSE_LOGIN_BAD;
                        }
                    }
                }
                const time_t now = time(0);
                cmds[0]->setResponseMillis(now * 1000);
                cmds[0]->setStatusId(statusId);
                LOG4CPLUS_TRACE(LOGGER, "got LOGIN cmd response, statusId=" << statusId);
            }
                // 查找 LOGOUT 命令的响应
            else {

                // 是否已经收到LOGOUT命令
                if (2 == cmds.size()) {
                    tagP = static_cast<string*> (cmds[1]->getProperty(PROPS_KEY_TAG));

                    // 查看标签是否一致
                    isSameTag = *tagP == tag;

                    size_t responseBeginPos = tagEndPos + 1;

                    if (!isSameTag && GENERAL_TAG == tag && responseBeginPos < length) {
                        string newTag("\r\n");
                        newTag.append(*tagP);
                        newTag.append(" ");
                        newTag.append(RESPONSE_LOGOUT_OK);
                        size_t tmp = str.find(newTag, responseBeginPos);
                        isSameTag = string::npos != tmp;
                        if (isSameTag) {
                            responseBeginPos = tmp + 3 + tagP->size();
                        }
                        // 再尝试BAD
                        if (!isSameTag) {
                            string newTag("\r\n");
                            newTag.append(*tagP);
                            newTag.append(" ");
                            newTag.append(RESPONSE_LOGOUT_BAD);
                            size_t tmp = str.find(newTag, responseBeginPos);
                            isSameTag = string::npos != tmp;
                            if (isSameTag) {
                                responseBeginPos = tmp + 3 + tagP->size();
                            }
                        }
                    }

                    // 标签一致，是期待的LOGOUT的响应
                    if (isSameTag) {

                        // 查找的登出命令响应
                        int statusId = ID_RESPONSE_LOGOUT_BAD;

                        // 尝试OK响应
                        cmdOrResponseBeginPos = str.find(RESPONSE_LOGOUT_OK, responseBeginPos);
                        if (string::npos != cmdOrResponseBeginPos && responseBeginPos == cmdOrResponseBeginPos) {
                            statusId = ID_RESPONSE_LOGOUT_OK;
                        }
                        const time_t now = time(0);
                        cmds[1]->setResponseMillis(now * 1000);
                        cmds[1]->setStatusId(statusId);
                        LOG4CPLUS_TRACE(LOGGER, "got LOGOUT cmd response, statusId=" << statusId);
                        log(sessionInfoP);
                        removeSessionInfo(*reverseIdP);
                    }
                }
            }
            delete reverseIdP;
            LOG4CPLUS_TRACE(LOGGER, "analyse leave");
        }
    }

    void Imap4Analyser::log(SessionInfo* sessionInfoP)
    {
        vector<CmdInfo*> cmds = (*(sessionInfoP->getCmds()));
        const string& userName = *(static_cast<string*> (cmds[0]->getProperty(PROPS_KEY_USER_NAME)));
        const int statusId = cmds[0]->getStatusId();
        int status;
        if (ID_RESPONSE_LOGIN_OK == statusId) {
            status = STATUS_RESPONSE_LOGIN_OK;
        }
        else if (ID_RESPONSE_LOGIN_NO == statusId) {
            status = STATUS_RESPONSE_LOGIN_NO;
        }
        else if (ID_RESPONSE_LOGIN_BAD == statusId) {
            status = STATUS_RESPONSE_LOGIN_BAD;
        }
        else {
            status = STATUS_RESPONSE_LOGIN_UNKNOW;
        }
        LOG4CPLUS_TRACE(LOGGER, "***********************" << ID_PROTOCOL << "|" << userName << "|" << status);
        LOG4CPLUS_TRACE(LOGGER, "***********************" << sessionInfoP->getPacketsCount() << "|" << sessionInfoP->getBytesCount());
    }

    Imap4Analyser::~Imap4Analyser()
    {
        LOG4CPLUS_TRACE(LOGGER, "deconstructor enter");
        LOG4CPLUS_TRACE(LOGGER, "deconstructor leave");
    }
}
