/**
 * Project: 
 * Class: ISA
 * Year: 2018/2019
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */

#ifndef FUNCTIONS_H 
#define FUNCTIONS_H

/* Libraries */
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cerrno>
#include <netdb.h>
#include <fstream>
#include <pcap.h>
#include <byteswap.h>

void print_help();

bool check_seconds(const std::string &string, int &int_value);

#endif