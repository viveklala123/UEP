//Server
#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
SOCKET Connections[100];
int ConnectionCounter=0;
void connectionThread(int index)
{
	char buffer[256];
	while(true)
	{
		recv(Connections[index],buffer,sizeof(buffer),NULL);
		for(int j=0;j<ConnectionCounter;j++)
		{
			if(j==index)
				continue;
			send(Connections[j],buffer,sizeof(buffer),NULL);
		}

	}
}

int main()
{
	//Winsock Startup
	WSADATA wsaData;
	WORD DLLVersion MAKEWORD(2,1);
	if(WSAStartup(DLLVersion,&wsaData)!=0)//if WSAStartup returns anything other than 0, then that means an error has occured in WinSock error
	{
		MessageBox(NULL,L"Winsock startup failed",L"Server", MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN addr;//Address that we will bind out listening socket to    

	int addrlen = sizeof(addr);//length of the address(required for accept call)
	addr.sin_addr.s_addr= inet_addr("127.0.0.1");//Address = localhost(this pc)
	addr.sin_port = htons(1111);//Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen =socket(AF_INET, SOCK_STREAM, NULL);//Create Socket to listen for new connections
	bind(sListen ,(SOCKADDR*)&addr,sizeof(addr));///Bind the address to the socket
	listen(sListen,SOMAXCONN);//Places Slisten socket in a state in which it is listening for an incoming connectioin. Note:SOMAXCONN=socket Outstanding
	//SOMAXCONN : The maximum length of the queue of pending connections. 

	SOCKET newConnection;//Socket to hold the client's connection
	for(int i=0;i<100;i++)
	{
		newConnection = accept(sListen,(SOCKADDR*)&addr,&addrlen);//Accept new connection

		if(newConnection == 0)
		{
			std::cout<<"Failed to accept client's connection."<<std::endl;

		}
		else{//if client connection properly accepted 

			std::cout<<"Client connected!"<<std::endl;
			char MOTD[256] = "Welcome! This is a Message of the day";//Create buuffer with msg of the day
			send(newConnection,MOTD,sizeof(MOTD),NULL);//Send MOTD Buffer
			Connections[i]=newConnection;
			ConnectionCounter+=1;
			CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)connectionThread,(LPVOID)(i),NULL,NULL);//Create thread to handle this client. The index in the socket array for this thread is the value(i).
		}
	}
	system("pause");

	return 0;
}