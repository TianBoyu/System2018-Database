//
// Created by 良觌 on 2018/6/27.
//

#include <string>
#include <sstream>
namespace Dytz
{
    std::string intToString(int val)
    {
        std::string s;
        std::stringstream ss;
        ss << val;
        ss >> s;
        return s;
    }

    int stringToInt(std::string s)
    {
        int val;
        std::stringstream ss;
        ss << s;
        ss >> val;
        return val;
    }

    std::string cutLogNumber(std::string logFile)
    {
        std::string ret = "";
        for(int i = 3; i < logFile.length(); ++i)
        {
            if(logFile[i] >= '0' && logFile[i] <= '9')
                ret += logFile[i];
            else break;
        }
        return ret;
    }
}

