//Client //https://www.youtube.com/watch?v=AjG4jcVn6QE
#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <iostream>

SOCKET Connection;
void ClientThread()
{
	char buffer[256];
	while(true)
	{
		recv(Connection,buffer,sizeof(buffer),NULL);
		std::cout<<buffer<<std::endl;
	}
}


int main()
{
	//Winsock Startup
	WSADATA wsaData;
	WORD DLLVersion MAKEWORD(2,1);
	if(WSAStartup(DLLVersion,&wsaData)!=0)//if WSAStartup returns anything other than 0, then that means an error has occured in WinSock error
	{
		//MessageBoxA(NULL,LPCSTR("Winsock startup failed"), MB_OK | MB_ICONERROR);
		exit(1);
	}
	SOCKADDR_IN addr;//Address that we will bind out listening socket to    

	int addrlen = sizeof(addr);//length of the address(required for accept call)
	addr.sin_addr.s_addr= inet_addr("127.0.0.1");//Address = localhost(this pc)
	addr.sin_port = htons(1111);//Port
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection =socket(AF_INET, SOCK_STREAM, NULL);//Set Connection Socket
	if(connect(Connection,(SOCKADDR*)&addr,sizeof(addr))!=0)//if we are unable to connect
	{
		//MessageBoxA(NULL,(LPCWSTR)L("Failed to connect"), MB_OK | MB_ICONERROR);
		//MessageBox( NULL, L"Failed to connect", L"Client", MB_OK | MB_ICONERROR);		
		return 0;//Failed to Connect
	}

	std::cout<<"Connected!"<<std::endl;
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ClientThread,NULL,NULL,NULL);//Create the client thread.

	char buffer[256];
	while(true)
	{
		std::cin.getline(buffer,sizeof(buffer));
		send(Connection,buffer,sizeof(buffer),NULL);
		Sleep(10);
	}
	system("pause");
	
	return 0;
}