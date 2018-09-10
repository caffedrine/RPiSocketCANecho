#include <cstdio>
#include <iostream>
#include <cstring>

#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>
#include <bits/ioctls.h>
#include <unistd.h>

#include <linux/can.h>
#include <linux/spi/spidev.h>
#include <linux/can/raw.h>
#include <thread>

#define CAN_INTERFACE   "can0"

using namespace std;

int main()
{
	int sockfd;
	struct sockaddr_can addr;
	struct ifreq ifr;
	
	if( (sockfd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0 )
	{
		perror("socket");
		return -1;
	}
	
	/* Check whether interface exists or not */
	strncpy(ifr.ifr_name, CAN_INTERFACE, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if (!ifr.ifr_ifindex)
	{
		perror("if_nametoindex");
		return 1;
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if( bind(sockfd, (struct sockaddr *) &addr, sizeof(addr))  == -1)
	{
		perror("binding");
		return -1;
	}
	
	cout << "CAN initialized! Waiting for recv()..." << endl;
	
	struct can_frame recvFrame, sendFrame;
	ssize_t recvBytes, sendBytes;
	int i = 0;
	
	while( true )
	{
		recvBytes = read(sockfd, &recvFrame, sizeof(struct can_frame));
		if( recvBytes < 0 )
		{
			perror("can raw socket read");
			return -1;
		}
		if( recvBytes < sizeof(struct can_frame) )
		{
			fprintf(stderr, "read: incomplete CAN frame\n");
			return -1;
		}
		cout <<  i++ << ". RECV: 0x" << std::hex << recvFrame.can_id << " # 0x" <<  std::hex << ((int)recvFrame.can_dlc) << " # " << std::hex << recvFrame.data << endl;

		/** ************************************** **/

		sendFrame = recvFrame;
		sendBytes = write(sockfd, &sendFrame, sizeof(struct can_frame));
		if( sendBytes < 0 )
		{
			perror("can raw socket send");
			return -1;
		}
		if( sendBytes < sizeof(struct can_frame) )
		{
			fprintf(stderr, "send: incomplete can frame\n");
			return -1;
		}
		cout <<  i++ << ". SEND: 0x" << std::hex << recvFrame.can_id << " # 0x" << std::hex << ((int)recvFrame.can_dlc) << " # " << std::hex << recvFrame.data << endl;

		
		this_thread::sleep_for(chrono::milliseconds(200));
	}
	
	return 0;
}