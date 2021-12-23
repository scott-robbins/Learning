/* Example of working with network interfaces in C */
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


//This is the structure the linux kernel uses for network interface properties
// struct iface{		
//     char ifr_name[IFNAMSIZ]; /* Interface name */
//     union {
//         struct sockaddr ifr_addr;
//         struct sockaddr ifr_dstaddr;
//         struct sockaddr ifr_broadaddr;
//         struct sockaddr ifr_netmask;
//         struct sockaddr ifr_hwaddr;
//         short           ifr_flags;
//         int             ifr_ifindex;
//         int             ifr_metric;
//         int             ifr_mtu;
//         struct ifmap    ifr_map;
//         char            ifr_slave[IFNAMSIZ];
//         char            ifr_newname[IFNAMSIZ];
//         char           *ifr_data;
//     };
// };

int usage(char * s){
	printf("Usage: %s <interface>\n", s);
	return 1;
}

int main(int argc, char**argv){
	int sock;
	struct ifreq ethernet;
	struct hostent *host_entry;

	// check input args
	if (argc < 2) return usage(argv[0]);

	// create socket 
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0)	return -1;

	// Try to get information for nx interface at argv[1]
	memset(&ethernet, 0, sizeof(struct ifreq));
	memcpy(ethernet.ifr_name, argv[1], IFNAMSIZ - 1);
	// if err < 0 the ioctl failed
	if (ioctl(sock, SIOCGIFFLAGS, &ethernet)<0){
		printf("Cannot find interface %s\n",argv[1]);
		return 1;
	}else{ printf("%s: flags=%hu <", argv[1],ethernet.ifr_flags); }  
	
	int n_channels = 1;
	// Check if interface is up/broadcasting/multicasting
	if (ethernet.ifr_flags & IFF_UP) { printf(" UP"); n_channels++;}

	if (ethernet.ifr_flags & IFF_BROADCAST) { printf(" BROADCAST"); n_channels++;}

	if (ethernet.ifr_flags & IFF_UP) { printf(" MULTICAST"); n_channels++;}
	printf("> ");

	// Show Maximum Transmission Unit
	printf(" mtu %i\n", ethernet.ifr_mtu);

	// show inet address
	/* I want IP address attached to "eth0" */
	int fd;
	strncpy(ethernet.ifr_name, argv[1], IFNAMSIZ-1);
	ioctl(sock, SIOCGIFADDR, &ethernet);
	char *ip = inet_ntoa(((struct sockaddr_in *)&ethernet.ifr_addr)->sin_addr);
	printf("\tinet %s ", ip);

	ioctl(sock, SIOCGIFNETMASK, &ethernet);
	char *netmask = inet_ntoa(((struct sockaddr_in *)&ethernet.ifr_netmask)->sin_addr);
	printf("\tnetmask %s", netmask);

	ioctl(sock, SIOCGIFDSTADDR, &ethernet);
	char *dest = inet_ntoa(((struct sockaddr_in *)&ethernet.ifr_dstaddr)->sin_addr);
	printf("\tdestination %s\n", dest);

	printf("\n");
	// Close Socket 
	close(sock);

	// Exit 
	return 0;
}
