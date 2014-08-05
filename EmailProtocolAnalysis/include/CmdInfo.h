/**
 * @file 
 * EmailProtocolAnalysis<br/>
 * CmdInfo.h<br/>
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

#ifndef CMDINFO_H
#define	CMDINFO_H

#include<map>
#include"Common.h"

namespace 
{

    /**
     * 命令信息，抽象客户端到服务器的命令
     */
    class CmdInfo
    {
    public:
        /**
         * 构造函数
         */
        CmdInfo();

        /**拷贝构造函数
         * @param original 待拷贝的命令信息
         */
        CmdInfo(const CmdInfo& original);

        /**
         * 赋值操作符
         * @param original 赋值的右操作数，来源
         * @return SessionId&，被赋值的命令信息
         */
        CmdInfo& operator=(const CmdInfo& original);

        /**
         * 析构函数
         */
        virtual ~CmdInfo();

        /**
         * 获取命令开始时间<p/>
         * 客户端向服务端发送的时间
         * @return long，毫秒，命令开始时间
         */
        long getStartMillis() const
        {
            return startMillis;
        }

        /**
         * 设置命令开始时间
         * @param startMillis，毫秒
         */
        void setStartMillis(long startMillis)
        {
            this->startMillis = startMillis;
        }

        /**
         * 获取命令响应时间<p/>
         * 常为服务器响应命令的时间
         * @return long，毫秒，命令响应时间
         */
        long getResponseMillis() const
        {
            return responseMillis;
        }

        /**
         * 设置命令响应时间
         * @param responseMillis 命令响应时间，毫秒
         */
        void setResponseMillis(long responseMillis)
        {
            this->responseMillis = responseMillis;
        }

        /**
         * 获取命令ID
         * @return  命令ID
         */
        int getCmdId() const
        {
            return cmdId;
        }

        /**
         * 设置命令ID
         * @param cmdId 命令ID
         */
        void setCmdId(int cmdId)
        {
            this->cmdId = cmdId;
        }

        /**
         * 获取状态ID
         * @return 状态ID
         */
        int getStatusId() const
        {
            return statusId;
        }

        /**
         * 设置状态ID
         * @param statusId 状态ID
         */
        void setStatusId(int statusId)
        {
            this->statusId = statusId;
        }
        
        void addProperty(std::pair<int, void*>& property);
        
        void removeProperty(int propertyKey);
        
        void* getProperty(int propertyKey);

    private:
        /**
         * 开始时间
         */
        long startMillis;

        /**
         * 响应时间
         */
        long responseMillis;

        /**
         * 命令ID
         */
        int cmdId;

        /**
         * 状态ID
         */
        int statusId;
        
        /**
         * 属性集
         */
        std::map<int, void*>* propsP;

        /**
         * 未知状态，一个命令信息的各个数据成员缺省均为该值
         */
        const static int UNKONW = -1;

        /**
         * 日志对象
         */
        const static log4cplus::Logger LOGGER;
    };
}
#endif	/* CMDINFO_H */

