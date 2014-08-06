/**
 * @file
 * EmailProtocolAnalysis<br/>
 * Analyser.h<br/>
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

#ifndef ANALYSER_H
#define	ANALYSER_H

#include<map>
#include<cstddef>
#include"Common.h"
#include"SessionId.h"
#include"SessionInfo.h"

namespace bigtree_analysis
{

    /**
     * 分析器
     */
    class Analyser
    {
    public:
        /**
         * 构造函数
         */
        Analyser();

        /**
         * 析构函数
         */
        virtual ~Analyser();

        /**
         * 分析内容
         * @param content 包中包含的内容，已经除去包头
         * @param length 内容长度
         * @param sessionId 会话ID，由五元组确定
         */
        virtual void analyse(const char* content, size_t length, const SessionId& sessionId) = 0;

        /**
         * 根据会话ID获取会话信息
         * @param sessionId 会话ID
         * @return SessionInfo*，如果不存在，返回一个值为0的会话信息指针
         */
         SessionInfo* getSessionInfo(const SessionId& sessionId);

        /**
         * 将一个键=>值关联放入Map中
         * @param sessionId 会话ID，作为Map的Key
         * @param sessionInfo，会话信息，作为Map的Value
         */
        void putSessionInfo(const SessionId& sessionId, SessionInfo& sessionInfo);

        /**
         * 在Map中是否包含指定的会话ID关联的会话信息
         * @param sessionId 会话ID，将作为Map的Key
         * @return 如果Map中存在该键值，返回true；不存在，返回false
         */
        bool containsSessionInfo(const SessionId& sessionId) const;

        /**
         * 删除会话信息
         * @param sessionId 会话ID，作为Map的Key，关联一个为Map的Value的会话信息
         */
        void removeSessionInfo(const SessionId& sessionId);
    private:
        /**
         * 保存会话ID和会话信息的Map，以会话ID为Key，会话信息为Value
         */
        std::map<SessionId, SessionInfo*>* sessionInfosMapP;

        /**
         * 日志对象
         */
        const static log4cplus::Logger LOGGER;
    };
}
#endif	/* ANALYSER_H */
