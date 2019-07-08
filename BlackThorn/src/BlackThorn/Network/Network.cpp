#include "btpch.h"
#include "Network.h"

namespace BlackThorn {

	void InitializeTCP(const char* address, int port);

	Network::Network()
	{

	}

	Network::~Network()
	{

	}

	bool Network::Initialize()
	{
		// Initialze winsock
		WSADATA winsockData;
		WORD winsockVersion = MAKEWORD(2, 2); //0x202;
		if (WSAStartup(winsockVersion, &winsockData))
		{
			BT_CORE_ERROR("NETWORK: WSAStartup failed: {0}", WSAGetLastError());
			return false;
		}

		BT_CORE_INFO("NETWORK: WSAStartup Successfull.");

		//char localIP[10] = "127.0.0.1";
		//Network::TCPListenerThread = std::thread(Network::InitializeTCP, localIP, PORT);

		return true;
	}

	void Network::InitializeTCP(const char* address, int port)
	{
		BT_CORE_WARN("NETWORK: Establishing TCP Listener...");
		SOCKET listeningSocket = CreateListenSocket(address, port);
		if (listeningSocket == INVALID_SOCKET)
		{
			BT_CORE_ERROR("NETWORK: ListeningSocket - INVALID_SOCKET.");
			system("pause");
			//return false;
		}
		BT_CORE_INFO("NETWORK: TCP Listener Established.");

		BT_CORE_WARN("NETWORK: Waiting for connections...");
		while (1) {
			AcceptConnections(listeningSocket);
			BT_CORE_INFO("Acceptor restarting...");
		}

		//return true;
	}

	bool Network::InitializeUDP()
	{
		return false;
	}

	void Network::CleanUp()
	{
	}

	SOCKET Network::CreateListenSocket(const char* address, int port)
	{
		u_long interfaceAddr = inet_addr(address);
		if (interfaceAddr != INADDR_NONE)
		{
			SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
			if (listeningSocket != INVALID_SOCKET)
			{
				sockaddr_in hint;
				hint.sin_family = AF_INET;
				hint.sin_addr.s_addr = interfaceAddr;
				hint.sin_port = htons(port);

				if (bind(listeningSocket, (sockaddr*)&hint, sizeof(hint)) != SOCKET_ERROR)
				{
					if (listen(listeningSocket, SOMAXCONN) == SOCKET_ERROR)
					{
						return INVALID_SOCKET;
					}
				}
				else
				{
					// Unable to bind socket
					BT_CORE_ERROR("NETWORK: Unable To Bind Listen Socket.");
					CleanUp();
					return INVALID_SOCKET;
				}
			}

			return listeningSocket;
		}
		return INVALID_SOCKET;
	}

	void Network::AcceptConnections(SOCKET ListeningSocket)
	{
		sockaddr_in sinRemote;
		int addrSize = sizeof(sinRemote);

		while (true) {
			SOCKET sd = accept(ListeningSocket, (sockaddr*)&sinRemote,
				&addrSize);
			if (sd != INVALID_SOCKET) {
				BT_CORE_INFO("NETWORK: Accepted Connection From {0} : {1}", inet_ntoa(sinRemote.sin_addr), ntohs(sinRemote.sin_port));

				DWORD threadID;
				CreateThread(0, 0, TCPConsumer, (void*)sd, 0, &threadID);
			}
			else {
				BT_CORE_WARN("NETWORK: Accept() Failed.");
				return;
			}
		}
	}

	DWORD WINAPI Network::TCPConsumer(void * sd_)
	{
		int retval = 0;
		SOCKET sd = (SOCKET)sd_;

		if (!EchoIncomingPackets(sd)) {
			BT_CORE_ERROR("NETWORK: Echo incoming packets failed");
			retval = 3;
		}

		BT_CORE_WARN("NETWORK: Shutting connection down...");
		if (ShutdownConnection(sd) != SOCKET_ERROR) {
			BT_CORE_INFO("NETWORK: Connection is down");
		}
		else {
			BT_CORE_ERROR("NETWORK: Connection shutdown failed");
			retval = 3;
		}

		return retval;
	}

	bool Network::EchoIncomingPackets(SOCKET sd)
	{
		// Read data from client
		char acReadBuffer[BUFFER_SIZE];
		int readBytes;
		do {
			readBytes = recv(sd, acReadBuffer, BUFFER_SIZE, 0);
			if (readBytes > 0) {
				BT_CORE_TRACE("NETWORK: Received {0} bytes from client.", readBytes);
				int sentBytes = 0;
				while (sentBytes < readBytes) {
					int temp = send(sd, acReadBuffer + sentBytes,
						readBytes - sentBytes, 0);
					if (temp > 0) {
						BT_CORE_TRACE("NETWORK: Sent {0} bytes back to client.", temp);
						sentBytes += temp;
					}
					else if (temp == SOCKET_ERROR) {
						return false;
					}
					else {
						// Client closed connection before we could reply to
						// all the data it sent, so bomb out early.
						BT_CORE_TRACE("NETWORK: Peer unexpectedly dropped connection!", temp);
						return true;
					}
				}
			}
			else if (readBytes == SOCKET_ERROR) {
				return false;
			}
		} while (readBytes != 0);

		BT_CORE_WARN("Connection closed by peer.");
		return true;
	}

	bool Network::ShutdownConnection(SOCKET sd)
	{
		return closesocket(sd);
	}

}