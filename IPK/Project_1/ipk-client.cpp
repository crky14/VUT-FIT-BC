/**
 * Project: Client-server programme for getting users information
 * Class: IPK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */
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


//Function with help message for script usage
void help() {
    std::cerr << "Usage: Scripts is executed from command line" <<std::endl;
    std::cerr << "Example: \'./ipk-client -h host -p port [-n|-f|-l] login" <<std::endl;
    std::cerr << "Needed cmd arguments:" <<std::endl;
    std::cerr << "  -h \'host\': IP adresse or  fully-qualified DNS name of server" <<std::endl;
    std::cerr << "  -p \'port\': port number" <<std::endl;
    std::cerr << "Options: only one can be used at same time, at least one is needed" <<std::endl;
    std::cerr << "  -n \'login\': User ID info" <<std::endl;
    std::cerr << "  -f \'login\': Users home directory" <<std::endl;
    std::cerr << "  -l \'login\': List of all users. If login was inserted, it will be used as prefix for users select " <<std::endl;
}

//function to check if string is unsigned integer and insert it into parameter int_value
bool is_num(const std::string &string, int &int_value)
{
    //check if string is not empty and first char is number
    if(string.empty() || (!isdigit(string[0]))) 
        return false;

    //strtol to convert string with error checking
    char * endptr;
    int_value = strtol(string.c_str(), &endptr, 10) ;
    if(*endptr == 0)
        return true;
    else
        return false;
}
// function checks prefix of received message and print it to STDERR or STDOUT
void print_information(std::string &message) {
    if(message.find("<<xcrkon00#S>>") == 0) {
        message.erase(0,14);
        std::cout << message;
    }
    else if(message.find("<<xcrkon00#F>>") == 0){
        message.erase(0,14);
        std::cerr << message;
    }
    else{
        std::cerr << "Uknown format of received message\n";
    }
}

//function doing all needed comunnication with server
void client_communication(std::string &message, std::string &host, std::string &port) {
   
    struct addrinfo settings; // struct for getaddrinfo() which will be filled with needed options
    struct addrinfo *server_info; //pointer to list of structures with service names and connection settings

    //setting structure settings
    memset(&settings, 0, sizeof(settings)); // make sure the struct is empty
    settings.ai_family = AF_INET;     // don't care IPv4 or IPv6
    settings.ai_socktype = SOCK_STREAM; // TCP stream sockets

    //service name lookup using getaddinfo()
    int status;
    if ((status = getaddrinfo(host.c_str(), port.c_str(), &settings, &server_info)) != 0) {
        std::cerr << "Error has occured while getting server informations\n";
        exit(EXIT_FAILURE);
    }

    //socket descriptor
    int client_socket;
    //pointer for iteration over looked up service names
    struct addrinfo *ptr_iterator;
    for(ptr_iterator = server_info; ptr_iterator != NULL; ptr_iterator = ptr_iterator->ai_next) {
        if((client_socket = socket(ptr_iterator->ai_family, ptr_iterator->ai_socktype, ptr_iterator->ai_protocol)) == -1) {
            continue;
        }
        //connecting to server on specified socket on which server is listtening
        if(connect(client_socket, server_info->ai_addr, server_info->ai_addrlen) < 0){
            close(client_socket);
            continue;
        }   
        break;
    }
    // freeing list server_info cause i dont need it anymore
    free(server_info);
    if(ptr_iterator == NULL) {
        std::cerr << "Failed to connect\n";
        exit(EXIT_FAILURE);
    }

    int sended;
    //sending message until all bytes are sent
    if((sended = send(client_socket,message.c_str(), strlen(message.c_str()), 0)) == -1) {
            std::cerr << "Error has occured while connection\n";
            exit(EXIT_FAILURE);
    }

    //asynchronious close of socket for write because nothing will be sent anymore
    shutdown(client_socket, SHUT_WR);

    //count of receved bytes
    int recv_count;
    //buffer for message -1kB
    char buffer[1024];
    //string for received message
    std::string received;
    //receiving message until buffer is full or server close connection
    while((recv_count = recv(client_socket, buffer, sizeof(buffer), 0)) > 0){
        if(recv_count < 0 ) {
            std::cerr << "Error has occured while receiving message\n";
            exit(EXIT_FAILURE); 
        }
        received.append(buffer,recv_count);
        if(recv_count == 0) {
            break;
        }
    }
    //closing socket
    close(client_socket);
    //printing received message to STDIN or STDERR
    print_information(received);
}

int main(int argc, char *argv[])
{	
    //protocol prefix
    std::string protocol = "<<xcrkon00#IPK>>";
	int opt;
	std::string message, host, port;
	int port_num;
    bool Lflag = false;
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-l") == 0) {
            if(Lflag == true) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);
            }
            Lflag = true;
        }
    }
    bool option = false;
    //parsing command line arguments
	while ((opt = getopt(argc, argv, ":h:p:n:f:l:")) != -1) {
        switch (opt) {
        case 'h': 
            if(!host.empty()) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);
            }
        	host = optarg;
        	break;
        case 'p':
            if(!port.empty()) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);
            }
            port = optarg;
        	break;
        case 'n':
            if(option == true) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);               
            }
            option = true;
            message = optarg;
            message = protocol + "N#" + message; 	
        	break;
        case 'f':
            if(option == true) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);               
            }
            option = true;
            message = optarg;
            message = protocol + "F#" + message;    
        	break;
        case 'l':
            if(option == true) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);               
            }
            option = true;
            if(optarg){
                message = optarg;
            }
            message = protocol + "L#" + message;    
        	break;
        case '?':
            std::cerr << "Invalid argument\n";
            exit(EXIT_FAILURE);
            break;
        default:
            if(optopt == 'l') {
            if(option == true) {
                    std::cerr << "Wrong usage of command line arguments\n";
                    exit(EXIT_FAILURE);               
                }
                option = true;
                message = protocol + "L#";
                break; 
            }
        	help();
        	exit(EXIT_FAILURE);
        }
    }
    //checks if all needed parameters were entered
    if((host.empty()) || (is_num(port, port_num)) == false || message.empty() || argc > 7) {
            std::cerr << "Wrong usage of command line arguments\n";
            help();
            exit(EXIT_FAILURE);
    }
    //checks port number, smaller than iteger and bigger than reserved ports
    if(port_num < 1024 || port_num > 65535) {
            std::cerr << "Invalid port number\n";
            exit(EXIT_FAILURE);
    }
    //communicationg with server
    client_communication(message, host, port);

    return EXIT_SUCCESS;
}
