//
// Created by wanton-wind on 2018/5/6.
//

#ifndef SYSTEM2018_DATABASE_DATABASE_H
#define SYSTEM2018_DATABASE_DATABASE_H

#include "DataSet.h"
#include "Parser.h"
#include "Error.h"

namespace sample {

class Database {

private:
    DataSet data;
    std::stringstream ss;

private:
    // process single command
    void processSingle(Command c, std::string k, std::string v) {
        switch (c) {
            case Command::PUT :
                data.put(k, v);
                break;
            case Command::GET :
                try {
                    ss << data.get(k) << ' ';
                }
                catch (...){
                    std::cerr << "key \"" << k << "\" does not exist.\n";
                }
                break;
            case Command::DEL :
                data.del(k);
                break;
        }
    }

public:
    Database(std::string name) : data(name) {

    }

    Database& process(std::stringstream &ss) {
        std::string command;
        while (std::getline(ss, command)) {
            Command c;
            std::string k;
            std::string v;
            try{
                std::tie(c, k, v) = Parser::decode(command);
                processSingle(c, k, v);
            }
            catch (UnknownCommandType){
                std::cerr << "UnknownCommandType in \"" << command << "\".\n";
            }
        }
        return *this;
    }

    // s means a transaction
    Database& process(std::string s) {
        std::stringstream ss{s};
        process(ss);
        return *this;
    }

    std::stringstream& getOutput() {
        return ss;
    }
};

}

#endif //SYSTEM2018_DATABASE_DATABASE_H
