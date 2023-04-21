#include <algorithm>
#include <fstream>
#include <iostream>
#include "lead.hpp"
#include <string>
//#include <map>
#include <unistd.h>
#include <iomanip>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>

#include <limits.h>

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

bool write(std::string filename, LeadSet leads) {
    std::string filepath = _getpath(filename);
    std::ofstream out(filepath);
    if(out) {
    out<<"name,phone\n";
    Lead::write(out, leads);
    out.close();
    return true;
    }
    return false;
}

void print(LeadSet leads) {
    int i = 0;
    for(auto item = leads.cbegin() ; item!=leads.cend();item++) {
    std::cout << '(' << ++i << ") |"
              << std::setiosflags(std::ios::left)
              << std::setw(20) << std::setfill('.')
              << std::string(*(item->second)).substr(0, std::string(*(item->second)).find(','))
              << std::string(*(item->second)).substr( std::string(*(item->second)).find(',')+1,  std::string(*(item->second)).size())
              << " |\n";
    }
}

std::string getNumber() {
    std::string number;
    std::getline(std::cin, number);
    return number;
}

std::string getFileName(int argc, char**argv) {
    if(argc==3&&std::string(argv[1]).find("-f" == 0 && std::string(argv[2]).size())) {
        return argv[2];
    } else {
        std::cout<<"\nPut filename. It will created in current work directory.\n";
        std::string fname;
        std::cin>>fname;
        return fname;
    }
}

int main(int argc, char**argv) {
    std::string fname = getFileName(argc,argv);
    LeadSet leadsSet;
    try {
    leadsSet = Lead::read(_getpath(fname));
    } catch(Lead::InvalidColumnName &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }

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
        //number = getNumber();
        std::getline(std::cin, number);

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
