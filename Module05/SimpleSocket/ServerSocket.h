//
// File:     ServerSocket.h
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Header file for the client portion of our C++ Sockets interface.   This code
// is based on code from an 2002 article in Linux Gazette
#ifndef ServerSocket_class
#define ServerSocket_class
#include "Socket.h"
class ServerSocket : private Socket
{
 public:
  ServerSocket ( int port );
  ServerSocket (){};
  virtual ~ServerSocket();
  const ServerSocket& operator << ( const std::string& ) const;
  const ServerSocket& operator >> ( std::string& ) const;
  void accept ( ServerSocket& );
};
#endif
