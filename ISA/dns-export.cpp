/**
 * Project: 
 * Class: ISA
 * Year: 2018/2019
 * University: VUTBR FIT
 * Login: xcrkon00
 * Author: Jakub Crkoň
 */

//includes
#include "functions.h"
#include "structures.h"

int main(int argc, char *argv[])
{
    //variables
    int opt;
    bool flagR, flagI, flagS, flagT = false;
    flagI = false;
    std::string fileName, interface, server, secondsStr;
    int seconds;

    //parsing command line arguments
    while ((opt = getopt(argc, argv, ":r:i:s:t:")) != -1) {
        switch (opt) {
            case 'r':
                if(!fileName.empty()) {
                    std::cerr << "Wrong usage of command line arguments\n";
                    exit(EXIT_FAILURE);
                }
                flagR = true;
        	    fileName = optarg;
                break;
            case 'i':
                if(!interface.empty()) {
                    std::cerr << "Wrong usage of command line arguments\n";
                    exit(EXIT_FAILURE);
                }
                flagI = true;
        	    interface = optarg;
                break;
            case 's':
                if(!server.empty()) {
                    std::cerr << "Wrong usage of command line arguments\n";
                    exit(EXIT_FAILURE);
                }
                flagS = true;
        	    server = optarg;
                break;
            case 't':
                if(!secondsStr.empty()) {
                    std::cerr << "Wrong usage of command line arguments\n";
                    exit(EXIT_FAILURE);
                }
                flagT = true;
        	    secondsStr = optarg;
                break;
            case '?':
                std::cerr << "Invalid argument\n";
                exit(EXIT_FAILURE);
            default:
        	    print_help();
        	    exit(EXIT_FAILURE);
        }
    }
    //checks if all needed parameters were inserted
    if(argc > 6 || (flagI && flagR)) {// || (check_seconds(secondsStr, seconds))) {
            std::cerr << "Wrong usage of command line arguments\n";
            print_help();
            exit(EXIT_FAILURE);
    }

    if(flagR) {
        char errbuf[1000];
        pcap_t *handle = pcap_open_offline(fileName.c_str(), errbuf);
        struct pcap_pkthdr header;	/* The header that pcap gives us */
		const u_char *packet;		/* The actual packet */
        packet = pcap_next(handle, &header);

	    const struct sniff_ethernet *ethernet; /* The ethernet header */
	    const struct sniff_ip *ip; /* The IP header */
	    const struct sniff_tcp *tcp; /* The TCP header */
        const struct sniff_udp *udp;
	    const char *payload; /* Packet payload */

	    u_int size_ip;
    	u_int size_tcp;

        ethernet = (struct sniff_ethernet*)(packet);
        ip = (struct sniff_ip*)(packet + SIZE_ETHERNET);
        size_ip = IP_HL(ip)*4;
        udp = (struct sniff_udp*)((unsigned char *)ip+size_ip);

        printf("%d\n", udp->th_sport);
        for (int i = 0; i < 8; i++) {
            printf("%d", !!((udp->th_sport << i) & 0x80));
        } 
        printf("\n");

        //printf("%s\n", inet_ntoa(ip->ip_dst));
    }

    return EXIT_SUCCESS;
}
