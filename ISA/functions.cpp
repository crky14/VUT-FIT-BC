/**
 * Project: 
 * Class: ISA
 * Year: 2018/2019
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */

#include "functions.h"

//Function with help message for script usage
void print_help() {
    //std::cerr << "Not created yet";
    /*
    std::cerr << "Usage: Scripts is executed from command line" <<std::endl;
    std::cerr << "Example: \'./ipk-client -h host -p port [-n|-f|-l] login" <<std::endl;
    std::cerr << "Needed cmd arguments:" <<std::endl;
    std::cerr << "  -h \'host\': IP adresse or  fully-qualified DNS name of server" <<std::endl;
    std::cerr << "  -p \'port\': port number" <<std::endl;
    std::cerr << "Options: only one can be used at same time, at least one is needed" <<std::endl;
    std::cerr << "  -n \'login\': User ID info" <<std::endl;
    std::cerr << "  -f \'login\': Users home directory" <<std::endl;
    std::cerr << "  -l \'login\': List of all users. If login was inserted, it will be used as prefix for users select " <<std::endl;
    */
}

bool check_seconds(const std::string &string, int &int_value)
{
    //check if string is not empty and first char is number
    if(string.empty() || (!isdigit(string[0]))){
        int_value = 60;
        return false;
    }

    //strtol to convert string with error checking
    char *endptr;
    int_value = strtol(string.c_str(), &endptr, 10);
    if(*endptr == 0)
        return true;
    else{
        int_value = 60;
        return false;
    }
}