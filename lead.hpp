#include <string>
#include <regex>
#include <map>
#include <fstream>
#include <iostream>

class Lead {
    private:
        std::string name;
        std::string number;
    public:
        Lead(std::string name, std::string number);


        friend std::ostream & operator<<(std::ostream &os,const Lead& lead);
        static void write(std::ostream &os,const std::map<std::string, Lead*>);
        static std::map<std::string, Lead*> read(std::string);
        operator std::string() const ;
        static std::string* phoneNormalize(std::string&);
};