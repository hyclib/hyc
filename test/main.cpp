
#include "../test/hycTest.h"
#include "../include/template/geom/geom.hpp"
#include "../include/template/statechart/state_mechine.hpp"

#include "../demo/net/simpleServer_tcp.h"
using namespace hyc;
#include "../demo/net/promisc.hpp"

#include "../include/hycLeakDetector.h"

void exp(String dir)
{
    String d = dir;

    Array<String> ds;
    Array<String> fs;
    FileUtils::DirExplorer(d.c_str(), &fs, &ds);

    if (ds.empty())
    {
        if (fs.size() == 3)
        {
            if (fs[0].find_last_of("@3x.png") == String::npos
                && fs[1].find_last_of("@3x.png") == String::npos
                && fs[2].find_last_of("@3x.png") == String::npos)
            {
                if (fs[0].find_last_of("@2x.png") != String::npos)
                {
                    String temp = fs[0];
                    temp[temp.size()-6] = '3';
                    FileUtils::FileCopy(fs[0].c_str(), temp.c_str());
                }
                else if (fs[1].find_last_of("@2x.png") != String::npos)
                {
                    String temp = fs[1];
                    temp[temp.size()-6] = '3';
                    FileUtils::FileCopy(fs[1].c_str(), temp.c_str());
                }
                else if (fs[2].find_last_of("@2x.png") != String::npos)
                {
                    String temp = fs[2];
                    temp[temp.size()-6] = '3';
                    FileUtils::FileCopy(fs[2].c_str(), temp.c_str());
                }
            }
        }
    }
    else
    {
        for (u32 i = 0; i < ds.size(); i++)
        {
            exp(ds[i]);
        }
    }
}

int main()
{
    IPv6 vv = SocketUtils::AddrLocal_v6();

    char buf[255] = { 0 };
    inet_ntop(AF_INET6, &vv, buf, 255);

    TcpTransceiver conn;
    ConnectResult cr = conn.connect("mango-server1.citongs.com", 3562);

    String dir = "Images.xcassets";
    exp(dir);



    testPromisc();
    system("pause");
    CN_LEAK_DETECTOR;

    hyc_test();



    system("pause");

	return 0;

}

