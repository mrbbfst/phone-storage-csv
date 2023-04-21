#include "lead.hpp"

Lead::Lead(std::string name, std::string number) {
    this->name=name;
    this->number= *Lead::phoneNormalize(number);
}

std::ostream & operator<<(std::ostream &os,const Lead& lead) {
    return os << std::string(lead);//lead.name << "," << lead.number + "\n";
}

void Lead::write(std::ostream &os,const LeadSet leads) {
    std::string buffer = "";
    for(auto item = leads.cbegin();item!=leads.cend();item++) {
        buffer+= std::string(*(item->second))+std::string("\n");
    }
    os<<buffer;
}

std::map<std::string, Lead*> Lead::read(std::string filename) {
    std::fstream is;
    std::map<std::string, Lead*> result;
    is.open(filename, std::ios::in);
    if(is) {
        std::string line;
        std::getline(is, line);
        if(line!="name,phone")
            throw Lead::InvalidColumnName();
        while(std::getline(is,line)) {
            result.insert({line.substr(line.find(",")+1, line.size()), 
                new Lead(
                    line.substr(0, line.find(",")), 
                    line.substr(
                        line.find(",")+1, 
                        line.size()
                        ) 
                    )
                    });  
        }
        is.close();
    }
    return result;
}

Lead::operator std::string() const {
    return this->name+","+this->number;//+"\n";
}

inline std::string* Lead::phoneNormalize(std::string& number) {
    number.erase(std::remove_if(number.begin(), number.end(), [](int ch) {return ch==(int)'-';} ), number.end());
    number.erase(std::remove_if(number.begin(), number.end(), [](int ch) {return ch==(int)'(';} ), number.end());
    number.erase(std::remove_if(number.begin(), number.end(), [](int ch) {return ch==(int)')';} ), number.end());
    number.erase(std::remove_if(number.begin(), number.end(), [](int ch) {return ch==(int)'+';} ), number.end());
    number.erase(std::remove_if(number.begin(), number.end(), isspace), number.end());
    std::string prefix ="";
    std::regex patternfull("^(380[1-9]{2}[0-9]{7})$");
    std::regex pattern2("80[1-9]{2}[0-9]{7}");
    std::regex pattern3("0[1-9]{2}[0-9]{7}");
    std::smatch smatch;

    if(std::regex_match(number, smatch, patternfull)) {
        ;
    } else if(std::regex_match(number, smatch, pattern2)) {
        number="8"+number;
    } else if(std::regex_match(number, smatch, pattern3)) {
        number = "38"+number;
    }
    else return nullptr;
    return &number;
}
