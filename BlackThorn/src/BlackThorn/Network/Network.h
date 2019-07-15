#pragma once

#include "Socket.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <thread>

#define PORT 5555
#define BUFFER_SIZE 1024

namespace BlackThorn {

	class BLACKTHORN_API Network
	{
	public:
		Network();
		~Network();

		static bool Initialize();
		static void InitializeTCP(const char* address, int port);
		static void InitializeUDP(const char* address, int port);

		static void CleanUp();

		// TCP

		static SOCKET CreateListenSocket(const char* address, int port);
		static void AcceptConnections(SOCKET ListeningSocket);
		static DWORD TCPConsumer(void* sd_);

		static bool EchoIncomingPackets(SOCKET sd);
		static bool ShutdownConnection(SOCKET sd);

		// UDP

		static SOCKET CreateUDPSocket(const char* address, int port, sockaddr_in outhint);
		static DWORD UDPConsumer(SOCKET udpsocket, sockaddr_in hint);
		static bool EchoIncomingUDPPackets(SOCKET udpsocket, sockaddr_in hint);

	private:
		static std::thread TCPListenerThread;
	};

}
