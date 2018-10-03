#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <bits/socket.h>
#include <bits/ioctls.h>
#include <linux/can.h>
#include <stdio.h>
#include <string.h>

#include "C_SocketCAN.h"

int CAN_Open(const char *if_name)
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
	strncpy(ifr.ifr_name, if_name, IFNAMSIZ - 1);
	ifr.ifr_name[IFNAMSIZ - 1] = '\0';
	ifr.ifr_ifindex = if_nametoindex(ifr.ifr_name);
	if( !ifr.ifr_ifindex )
	{
		perror("if_nametoindex");
		return -1;
	}
	
	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if( bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) == -1 )
	{
		perror("binding");
		return -1;
	}
	return sockfd;
}

int CAN_ReadFrame(int sockfd, struct can_frame *RecvFrame)
{
	ssize_t recvBytes = read(sockfd, RecvFrame, sizeof(struct can_frame));
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
}

int CAN_WriteFrame(int sockfd, struct can_frame *SendFrame)
{
	ssize_t sendBytes = write(sockfd, SendFrame, sizeof(struct can_frame));
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
	return (int) sendBytes;
}

int CAN_IsAvailable(int sockfd)
{
	int availableBytes = 0;
	ioctl(sockfd, FIONREAD, &availableBytes);
	
	if( availableBytes < sizeof(struct can_frame) )
		return 0;
	
	return (availableBytes / sizeof(struct can_frame));
}

void CAN_Close(int sockfd)
{
	close(sockfd);
}