#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <time.h>

static int *glob_var, *am;
static float *om;
int count;

int createsock(char add[13],int port)
{
	int clientSocket;
	char buffer[1024];
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(add);
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
	addr_size = sizeof serverAddr;
	connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size);
	return clientSocket;
}

int main()
{  
	//sleep(30);                 
	clock_t ti, et;
	time_t myt;
	double li=0, t, tp=2.0*22.0/7.0;
	char str[100];
	int clientSocket;
	cpu_set_t mask;
	CPU_ZERO(&mask);
	glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, 
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	om = mmap(NULL, sizeof *om, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	am = mmap(NULL, sizeof *am, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*glob_var=1;
	*om=1;
	*am=1;
	if(fork())
	{
		ti=clock();
		CPU_SET(0, &mask);
		sched_setaffinity(0, sizeof(mask), &mask);
		clientSocket=createsock("127.0.0.1",10005);
		str[0]='\0';
		sprintf(str, "xaxis,yaxis");
		str[11]='\n';
		str[12]='\0';
		send(clientSocket, str, 14, 0);
		while(*glob_var)
        	{
			et=clock()-ti;
			t=((double)et)/CLOCKS_PER_SEC;
			if(t>li)
			{
				myt = time(NULL);	
				str[0]='\0';
				sprintf(str, "%f,%f\n",t, (*am)*sin(tp*(*om)*li));
				send(clientSocket, str, strlen(str), 0);
				li+=0.01;
			}
		}
	}
	else if(fork())
	{
		ti=clock();	
		clientSocket=createsock("127.0.0.1",10002);
		CPU_SET(1, &mask);
		sched_setaffinity(0, sizeof(mask), &mask);
		str[0]='\0';
		//sprintf(str, "Core %d", sched_getcpu());
		sprintf(str, "xaxis,yaxis");
		str[11]='\n';
		str[12]='\0';
		send(clientSocket, str, 14, 0);
		while(*glob_var)
		{
			et=clock()-ti;
			t=((double)et)/CLOCKS_PER_SEC;
			if(t>li)
			{
				myt = time(NULL);
				sprintf(str, "%f,%f\n",t, (*am)*sin(tp/4-tp*(*om)*li));
				send(clientSocket,str,strlen(str),0);
				li+=0.01;
			}
		}
	}
	else if(fork())
	{
		ti=clock();
		CPU_SET(2, &mask);
		sched_setaffinity(0, sizeof(mask), &mask);
		clientSocket=createsock("127.0.0.1",10003);
		str[0]='\0';
		//sprintf(str, "Core %d", sched_getcpu());
		sprintf(str, "xaxis,yaxis");
		str[11]='\n';
		str[12]='\0';
		send(clientSocket, str, 14, 0);
		float i=0;
		float lu;
		while(*glob_var)
		{
			et=clock()-ti;
			t=((double)et)/CLOCKS_PER_SEC;
			if(t>li)
			{
				i=(*am)*(*om)*lu;
				if(i>(*am))
				{
					lu=0;
				}			
				str[0]='\0';
				myt = time(NULL);
				sprintf(str,"%f,%f\n", t,i);
				send(clientSocket,str,strlen(str),0);
				li+=0.01;
				lu+=0.01;
			}
		}
	}
	else if(fork())
	{
		ti=clock();
		CPU_SET(3, &mask);
		sched_setaffinity(0, sizeof(mask), &mask);
		clientSocket=createsock("127.0.0.1",10004);
		int i=1;
		char str[15];
		str[0]='\0';
		//sprintf(str, "Core %d", sched_getcpu());
		sprintf(str, "xaxis,yaxis");
		str[11]='\n';
		str[12]='\0';
		send(clientSocket, str, 14, 0);
		int j=0;
		while(*glob_var)
		{
			et=clock()-ti;
			t=((double)et)/CLOCKS_PER_SEC;
			if(t>li)
			{
				j++;
				myt = time(NULL);
				sprintf(str,"%f,%d\n",t,i*(*am));
				send(clientSocket, str, strlen(str), 0);
				if(j==50)
				{
					i*=-1;
					j=0;
				}
				li+=0.01;
			}
		}
	}
	else
	{
		char ch[2];
		float val;
		clientSocket = createsock("127.0.0.1",10010);
		printf("aaa");
		while(*glob_var)
		{
			recv(clientSocket, ch, 1, 0);
			printf("aaa");
			ch[1]='\0'; 
			if(strcmp(ch,"f")==0)
			{
				recv(clientSocket, ch, 2, 0);
				*om=atof(ch);
			}
			else if(strcmp(ch,"a")==0)
			{
				recv(clientSocket, ch, 2, 0);
				ch[1]='\0';
				*am=atoi(ch);
			}
			else
				*glob_var=0;
		}
	}
 	return 0;
}
