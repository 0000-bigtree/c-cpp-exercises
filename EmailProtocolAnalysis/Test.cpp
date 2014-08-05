/**
 * @file
 * EmailProtocolAnalysis<br/>
 * test.cpp<br/>
 * Copyright (c) bigtree 2011-2014. All rights reserved.
 * @date 2011-07-02
 * @author bigtree
 * @version 1.0.0<p/>
 * <b>&nbsp;&nbsp;&nbsp;历史:</b>
 * <pre>
 *        ver    date       author        desc
 *        1.0.0  2011-07-02 bigtree           created
 * </pre>
 */

#include"Common.h"
#include"Imap4Analyser.h"
#include"SessionId.h"
#include"SessionInfo.h"
#include"CmdInfo.h"
#include"Analyser.h"
#include"Imap4Analyser.h"

using std::string;
using log4cplus::Logger;
using log4cplus::PropertyConfigurator;
using bigtree_analysis::SessionId;
using bigtree_analysis::Analyser;
using bigtree_analysis::Imap4Analyser;

struct TEST_DATA
{
    const char* data;
    size_t length;
    SessionId sessionId;
};

void test1(Analyser* analyser);

/**
 * 初始化log4cplus日志组件
 * @return 初始化成功，返回true，失败，返回false
 */
bool InitLog4cplus()
{
    PropertyConfigurator::doConfigure("config/traceConfig.properties");
    return true;
}

int main(int argc, char** argv)
{
    Logger LOGGER = Logger::getInstance(".main");
    InitLog4cplus();

    // 测试1
    Analyser* analyser = new Imap4Analyser();

    string a ("abc");
    string b ("abc");
    LOG4CPLUS_TRACE(LOGGER, "" << a << ((a == b) ? "==" : "!= ") << b);
    test1(analyser);
    delete analyser;
    return 0;
}

void test1(Analyser* analyser)
{
    TEST_DATA data[] = {
        {
            "C2 LOGIN wdx ddddafsdfasdf\r\n", 34, SessionId(0, 0, 0, 143, 5)
        },
        {
            "a", 1, SessionId(1, 0, 0, 0, 0)
        },
        {
            "C2OK LOGINX succeeded\r\n", 22, SessionId(0, 143, 0, 0, 5)
        },
        {
            "C2 BAD LOGIN succeeded\r\n", 22, SessionId(0, 143, 0, 0, 5)
        },
        {
            "C3 BAD dddddd\r\n", 16,  SessionId(0, 0, 0, 143, 5)
        },
        {
            "C3 SELECT dddddd\r\n", 19,  SessionId(0, 0, 0, 143, 5)
        },
        {
            "C6 LOGOUT\r\n", 9, SessionId(0, 0, 0, 143, 5)
        },
        {
            "* BYE Nice talking to you\r\nC6 OK LOGOUT successful\r\n", 53, SessionId(0, 143, 0, 0, 5)
        }
//        {
//            "C6 OK LOGOUT successful\r\n", 28, SessionId(0, 143, 0, 0, 5)
//        }
    };

    for (size_t i = 0; i < sizeof (data) / sizeof(data[0]); i++) {
        // FIXME
        TEST_DATA& element = data[i];
        analyser->analyse(element.data, element.length, element.sessionId);
    }
}
