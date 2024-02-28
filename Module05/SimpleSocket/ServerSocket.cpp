//
// File:     ServerSocket.cpp
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Server file for the server portion of our C++ Sockets interface.   This code
// is based on code from an 2002 article in Linux Gazette
#include "ServerSocket.h"
#include "SocketException.h"
//
// ClientSocket::ClientSocket(std::string, int)
// Construct a new instance of the client side of our C++ socket interface.
// Pre-condition:
// Host and port passed from calling application.
//
// Post-condition:
// A new instance of our class is created on the heap.
//
ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }

}
//
// ServerSocket::~ServerSocket()
// Do nothing constructor for our socket.
//
ServerSocket::~ServerSocket()
{
}
//
// ServerSocket::operator <<(const std::string &)
// Put a piece of data to the socket.
// Pre-condition:
// String into which the data is to be dumped is passed into method.
//
// Post-condition:
// The information is read from the socket or an exception is thrown if the
// read failed.
//
const ServerSocket& ServerSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}
//
// ServerSocket::operator >>(const std::string &)
// Get a piece of data from the socket.
// Pre-condition:
// String into which the data is to be dumped is passed into method.
//
// Post-condition:
// The information is read from the socket or an exception is thrown if the
// read failed.
//
const ServerSocket& ServerSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}
//
// ServerSocket::accept( ServerSocket&)
// Accept connections from a client.
// Pre-condition:
// A socket defined in the passed parameter.
// Post-condition:
// Accept the connection from that socket or have thrown an exception.
//
void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}
