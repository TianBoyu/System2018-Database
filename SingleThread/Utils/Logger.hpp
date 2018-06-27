//
// Created by wanton-wind on 2018/5/20.
//

#ifndef SYSTEM2018_DATABASE_LOGGER_H
#define SYSTEM2018_DATABASE_LOGGER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <io.h>
#include "Parser.hpp"
#include "../Database.hpp"
#include "Tools.cpp"


namespace Dytz {

    class Database;

    class Logger {
    private:
        const std::string LOG_RECORD_PATH = "LOG_RECORD.txt";
        const int MAX_LOG_FILE_SIZE = 1000; //the maximum number of log items that a log file can hold
        static int CURRENT_LOG_NUMBER = 0;

    public:
        Logger() {
            if (_access(LOG_RECORD_PATH.c_str(), 0) == -1) {
                std::ofstream logRecordOut(LOG_RECORD_PATH);
                logRecordOut << "LOG-" << 0 << ".txt";
                logRecordOut.close();
            }
            logPath = getLastLogPath();
            if(logPath == "")
                logPath = getNewLogName();
            else CURRENT_LOG_NUMBER = stringToInt(cutLogNumber(logPath));
        };


        std::string splitLog(const std::string oldLogFile)
        {
            //return the name of the new Logfile
            std::ifstream oldLogStream = std::ifstream(oldLogFile);
            if(!oldLogStream.is_open()) {
                throw OpenFileError();
            }
            std::vector<std::string> old_log = std::vector<std::string>();
            std::string cur_line;
            int i = 0;
            std::vector<std::string> cur_log = old_log;
            while(std::getline(oldLogStream, cur_line)){
                if(i < MAX_LOG_FILE_SIZE)
                    cur_log.push_back(cur_line);
                else
                {
                    i = 0;
                    writeWholeLog(cur_log);
                    cur_log.clear();
                    logPath = getNewLogName();
                }
                ++i;
            }

        }

        // create log
        void createLog(const std::string &transaction) {
            std::ifstream inLog(logPath);
            std::ofstream logFile;
            if(!inLog) {
                logFile.open(logPath, std::ios::out);
            } else {
                logFile.open(logPath, std::ios::app);
            }
            if (!logFile) {
                throw OpenFileError();
            }
            logFile << transaction << '\n';
            logFile.close();
        }

        // get the exactly context useful for database restore
        std::vector<LogPiece> getRestoreContext() { // TODO endline
            std::vector<std::string> whole_log = getWholeLog();
            std::vector<LogPiece> restoreContext;
            if (whole_log.empty()) {
                return restoreContext;
            }
            for (auto citer = whole_log.cend() - 1; citer != whole_log.cbegin(); --citer) {
                if ((*citer)[0] == '@')
                    break;
                restoreContext.push_back(LogPiece(LogParser::decode(*citer)));
            }
            return std::move(restoreContext);
        };

        std::vector<LogPiece> getRollBackContext(int numberOfTransaction){
            for(int i = 1; i , numberOfTransaction; ++i)
            {

            }
        }


    private:
        // logging path
        std::string logPath;

        std::string getLastLogPath(){
            std::ifstream logRecord(LOG_RECORD_PATH);
            if (!logRecord.is_open()) {
                throw OpenFileError();
            }
            std::string cur_line;
            while (std::getline(logRecord, cur_line)) {}
            if (_access(cur_line.c_str(), 0) == -1) {
                std::ofstream logFile(cur_line);
                logFile.close();
            }
            return cur_line;
        };

        std::vector<std::string> getWholeLog() {
            logPath = getLastLogPath();
            std::ifstream logFile(logPath);
            if (!logFile.is_open()) {
                throw OpenFileError();
            }
            std::vector<std::string> whole_file = std::vector<std::string>();
            std::string cur_line;
            while (std::getline(logFile, cur_line)) {
                whole_file.push_back(cur_line);
            }
            return whole_file;
        };

        std::string getNewLogName()
        {
            std::string ret = "Log" + intToString(CURRENT_LOG_NUMBER);
            addToLogRecord(ret);
            return ret;
        }

        void addToLogRecord(std::string logName)
        {
            std::ofstream logFile;
            logFile.open(LOG_RECORD_PATH, std::ios::app);
            logFile << logName << '\n';
        }

        void writeWholeLog(std::vector<std::string> whole_log)
        {
            //used when split log
            std::ofstream outLog = std::ofstream(logPath);
            for(std::string log : whole_log)
                outLog << log << '\n';
        }

    };


}


#endif //SYSTEM2018_DATABASE_LOGGER_H
