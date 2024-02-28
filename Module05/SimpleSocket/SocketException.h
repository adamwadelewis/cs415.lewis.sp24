// SocketException class

//
// File:     SocketException.h
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Define an exception class for our socket interface.  Note that all code
// for this class has been in-lined into this header file.  The code in this
// class is based on code from a 2002 article in Linux Gazette.
//
#ifndef SocketException_class
#define SocketException_class
#include <string>
class SocketException
{
 public:
  SocketException ( std::string s ) : m_s ( s ) {};
  ~SocketException (){};

  std::string description() { return m_s; }

 private:

  std::string m_s;

};

#endif
