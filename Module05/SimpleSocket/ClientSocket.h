//
// File:     ClientSocket.h
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Header file for the client portion of our C++ Sockets interface.   This code
// is based on code from an 2002 article in Linux Gazette
#ifndef ClientSocket_class
#define ClientSocket_class
#include "Socket.h"
class ClientSocket : private Socket
{
 public:

  ClientSocket ( std::string host, int port );
  virtual ~ClientSocket(){};

  const ClientSocket& operator << ( const std::string& ) const;
  const ClientSocket& operator >> ( std::string& ) const;
};

#endif
