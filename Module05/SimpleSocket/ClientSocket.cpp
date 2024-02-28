//
// File:     ClientSocket.h
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Header file for the client portion of our C++ Sockets interface.   This code
// is based on code from an 2002 article in Linux Gazette
#include "ClientSocket.h"
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
ClientSocket::ClientSocket ( std::string host, int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create client socket." );
    }

  if ( ! Socket::connect ( host, port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }
}
//
// ClientSocket::operator <<(const std::string &)
// Put a piece of data to the socket
// Pre-condition:
// String to be output is passed into method.
//
// Post-condition:
// The information is sent to the socket or an exception is thrown if the
// send fails.
//
const ClientSocket& ClientSocket::operator << ( const std::string& s ) const
{
  if ( ! Socket::send ( s ) )
    {
      throw SocketException ( "Could not write to socket." );
    }

  return *this;

}
//
// ClientSocket::operator >>(const std::string &)
// Get a piece of data from the socket.
// Pre-condition:
// String into which the data is to be dumped is passed into method.
//
// Post-condition:
// The information is read from the socket or an exception is thrown if the
// read failed.
//
const ClientSocket& ClientSocket::operator >> ( std::string& s ) const
{
  if ( ! Socket::recv ( s ) )
    {
      throw SocketException ( "Could not read from socket." );
    }

  return *this;
}
