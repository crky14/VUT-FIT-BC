/**
 * Project: Client-server programme for getting users information
 * Class: IPK
 * Year: 2017/2018
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */

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
#include <pwd.h>
#include <iostream>
#include <sstream>
#include <fstream>

#define FAIL 0;
#define SUCCESS 1;

void help() {
    std::cerr << "Usage: Scripts is executed from command line" <<std::endl;
    std::cerr << "Example: \'./ipk-server -p port" <<std::endl;
    std::cerr << "Needed cmd arguments:" <<std::endl;
    std::cerr << "  -p \'port\': port number" <<std::endl;
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
//parsing received message and getting needed response
void get_information(std::string &received, std::string &respond) {

    //check for protocol on the start of the string
    if(received.find("<<xcrkon00#IPK>>") == 0) {
        received = received.substr(16);
        //structure for getpwnam()
        struct passwd *user_info;
        std::string mode;
        //checking for mode selected
        mode = received.substr(0,2);
        received.erase(0,2);
        //getting needed info from /etc/passwd/
        if(mode.compare("N#") == 0) {
            //getting name full name from user database
            if((user_info = getpwnam(received.c_str())) == NULL) {
                respond = "<<xcrkon00#F>>Login not found\n";
                return;
            }
            respond = user_info->pw_gecos;
            respond += "\n";  
        }
        else if(mode.compare("F#") == 0){
            //getting home direcotry fro muser database
            if((user_info = getpwnam(received.c_str())) == NULL) {
                respond = "<<xcrkon00#F>>Login not found\n";
                return;
            }
            respond = user_info->pw_dir;  
            respond += "\n";  
        }
        else if(mode.compare("L#") == 0){
            //using ifstream to load /etc/passwd and get all users
            std::string line;
            std::ifstream file_desc("/etc/passwd");
            if(received.empty()) {
                while((std::getline(file_desc, line))) {
                    line = line.substr(0, line.find(":"));
                    respond += line + "\n";
                }
            }
            else {
                //getting all user which match given login
                while((std::getline(file_desc, line))) {
                    if((line.find(received.c_str()) == 0)) {
                        line = line.substr(0, line.find(":"));
                        respond += line + "\n";
                    }
                }
                //login match no user
                if(respond.empty()) {
                    respond = "<<xcrkon00#F>>No users found!\n";
                    return;
                }
            }
        }
        else{
            respond = "<<xcrkon00#F>>Unknown option\n";
            return;
        }
    }
    else{
        respond = "<<xcrkon00#F>>Unknown protocol\n";
        return;
    }
    respond = "<<xcrkon00#S>>" + respond;
}
//function providing all commucation with clients
void server_communication(std::string &port) {
    int status;
    //structures for getaddrinfo
    struct addrinfo settings;
    struct addrinfo *server_info;

    //incializng structure 
    memset(&settings, 0, sizeof(settings));
    settings.ai_family = AF_INET;  // use IPv6 , it accepts also IPv4
    settings.ai_socktype = SOCK_STREAM;
    settings.ai_flags = AI_PASSIVE;     // fill in my IP for me

    //service name lookup using getaddinfo()
    if ((status = getaddrinfo(NULL, port.c_str(), &settings, &server_info)) != 0) {
        std::cerr << "Error has occured while getting server informations\n";
        exit(EXIT_FAILURE);
    }
    
    //server socket descriptor for listening
    int server_socket;
    int confirm = 1;
    struct addrinfo *ptr_iterator;
    for(ptr_iterator = server_info; ptr_iterator != NULL; ptr_iterator = ptr_iterator->ai_next) {
        //getting socket descriptor
        if((server_socket = socket(ptr_iterator->ai_family, ptr_iterator->ai_socktype, ptr_iterator->ai_protocol)) == -1) {
            continue;
        }
        //clear port for reuse if needed
        if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &confirm, sizeof(int)) == -1) {
            std::cerr << "Socket cant be used - Address already in use\n";
            exit(EXIT_FAILURE);
        }
        //bind server socket to specific port
        if(bind(server_socket, ptr_iterator->ai_addr, ptr_iterator->ai_addrlen) == -1){
            close(server_socket);  
            continue;  
        }  
        break;     
    }
    //free structure because i dont need it anymore
    free(server_info);
    if(ptr_iterator == NULL) {
        std::cerr << "Failed to bind socket\n";
        exit(EXIT_FAILURE);
    }

    //listening on specified socket for connections , 10 peers can be in queue
    if(listen(server_socket, 10) == -1) {
        std::cerr << "Error has occured while client was waiting for connection\n";
        exit(EXIT_FAILURE);
    }
    //struct cointaning inforamtion about connected peer
    struct sockaddr_storage client_addr;
    uint addr_len = sizeof(client_addr);
    //socket descriptor for cummunication
    int cmn_socket;
    pid_t new_proc;
    //received string
    std::string received;
    //buffer and received bytes count
    char buffer[1024];
    int recv_count;
    //infinite loop , server responding to all peers
    while(1) {
        //accepting connection on specified port and creates new socket descriptor for communication with peer
        if((cmn_socket = accept(server_socket, (struct sockaddr *) &client_addr,  &addr_len)) < 0) {
            std::cerr << "Error has occured while connecting\n";
            exit(EXIT_FAILURE);
        }

        //creates new process which will answer peer while parent will be listening for other connections
        if((new_proc = fork()) == -1) {
            std::cerr << "Error has occured while creating process\n";
            exit(EXIT_FAILURE);
        }
        //parent closing socket for communicaiton
        else if(new_proc != 0) {
            close(cmn_socket);
        }
        //child closing socket for listening
        else if(new_proc == 0) {
            close(server_socket);
            //receiving until buffer is full or client close connection
            while((recv_count = recv(cmn_socket, buffer, sizeof(buffer), 0)) > 0) {
                if(recv_count == -1){
                    std::cerr << "Error has occured while receiving received\n";
                    exit(EXIT_FAILURE); 
                }
                received.append(buffer,recv_count);
                if(recv_count == 0) {
                    break;
                }
            }

            //checks protocol and get needed data for answer
            std::string respond;
            get_information(received, respond);
        
            //sending answer until its all send
            int sended;
            if((sended = send(cmn_socket,respond.c_str(), strlen(respond.c_str()), 0)) == -1) {
                std::cerr << "Error has occured while connection\n";
                exit(EXIT_FAILURE);
            }
            //child close socket for communication wit peer and exits
            close(cmn_socket);
            exit(EXIT_SUCCESS);
        }
    }
    close(server_socket);
}

int main(int argc, char *argv[])
{	
    std::string mess, port;
	int opt,port_num; 
    //parsing command line arguments
	while ((opt = getopt(argc, argv, ":p:")) != -1) {
        switch (opt) {
        case 'p':
            if(!port.empty()) {
                std::cerr << "Wrong usage of command line arguments\n";
                exit(EXIT_FAILURE);
            }
            port = optarg;
        	break;
        case '?':
            std::cerr << "Invalid argument\n";
            exit(EXIT_FAILURE);
            break;
        default:
        	help();
        	exit(EXIT_FAILURE);
        }
    }
    //checks if port is a number
    if(is_num(port, port_num) == false || argc > 3) {
            std::cerr << "Wrong usage of command line arguments\n";
            help();
            exit(EXIT_FAILURE);
    }
    //checks port value
    if(port_num < 1024 || port_num > 65535) {
            std::cerr << "Invalid port number\n";
            exit(EXIT_FAILURE);
    }
    //communication with clients
    server_communication(port);

    return EXIT_SUCCESS;
}
