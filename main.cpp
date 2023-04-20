#include <fstream>
#include <iostream>
#include "lead.hpp"
#include <string>
#include <map>
#include <unistd.h>

#include <limits.h>

using setT = std::map<std::string, Lead*>;

std::string cwd() {
    char cwdp[PATH_MAX];
   if (getcwd(cwdp, sizeof(cwdp)) != NULL) {
       return std::string(cwdp);
   } else {
       std::cerr<<("getcwd() error\n");
       return "";
   }
}

std::string _getpath(std::string filename) {
    return cwd()+"/"+filename+".csv";
}

bool checkIsEmpty(std::string filename) {
    
    std::fstream in;
    in.open(_getpath(filename).c_str(), std::ios::in);
    if(in) {
        std::string firstline;
        std::getline(in, firstline);
        in.close();
        if(firstline=="name,phone");
        return false;
    }
    return true;
}

bool write(std::string filename, setT leads) {
    std::string filepath = _getpath(filename);
    bool isnewfile = checkIsEmpty(filename);
    std::ofstream out(filepath);
    //if(isnewfile) {
        out<<"name,phone\n";
    //}
    Lead::write(out, leads);
    out.close();
    return true;
}

void print(setT leads) {
    int i = 0;
    for(auto item = leads.cbegin() ; item!=leads.cend();item++) {
        std::cout << '(' << ++i << ") " << *(item->second);
    }
}

std::string getNumber() {
    std::string number;
    std::getline(std::cin, number);
    return number;
}

int main(int argc, char**argv) {

    std::cout<<"\nPut filename. It will created in current work directory.\n";
    std::string fname;
    std::cin>>fname;
    std::map<std::string, Lead*> leadsSet = Lead::read(_getpath(fname));

    bool isWork = true;
    
    while(isWork) {
        std::cout << "Put lead data or command: ";
        std::string nameOrCommand;
        std::string number;
        std::cin >> nameOrCommand;
        if(nameOrCommand.find("\\")==0&&nameOrCommand.size()>1) {
            char* command = &nameOrCommand[1];
            while(std::string(command).size()) {
                if(std::string(command).find("w")==0) {
                    write(fname, leadsSet);
                } else if(std::string(command).find("s")==0) {
                    print(leadsSet);
                } else if(std::string(command).find("r")==0) {
                    leadsSet.erase(getNumber());
                } else if(std::string(command).find("q")==0) {
                    isWork = false; 
                }
                command+= (std::string(command).size()>0) ? 1 : 0;
            }
            continue;
        }
        number = getNumber();

        if(Lead::phoneNormalize(number)==nullptr) {
            std::cout << "\e[0;31mInvalid number.\e[0;0m\n";
            continue;
        }
        int size = leadsSet.size();
        leadsSet.insert({number, new Lead(nameOrCommand, number)});
        if(size==leadsSet.size())
            std::cout << "\033[1;33m(Already exist)\033[0m\n";
    }

    return 0;
}