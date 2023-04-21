#include <string>
#include <regex>
#include <map>
#include <fstream>
#include <iostream>
#include <exception>

class Lead;
using LeadSet = std::map<std::string, Lead*>;

class Lead {
    private:
        std::string name;
        std::string number;
    public:
        Lead(std::string name, std::string number);
        class InvalidColumnName : std::exception {
        public:
             const char* what() const noexcept override {return "Invalid column name."; }
        };

        friend std::ostream & operator<<(std::ostream &os,const Lead& lead);
        static void write(std::ostream &os,const LeadSet);
        static std::map<std::string, Lead*> read(std::string);
        operator std::string() const ;
        inline static std::string* phoneNormalize(std::string&);
};
