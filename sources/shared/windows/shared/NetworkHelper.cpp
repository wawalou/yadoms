#include "stdafx.h"

#include "../../shared/Exceptions/Exception.hpp"
#include "../../shared/Log.h"
#include "NetworkHelper.h"

CNetworkHelper::CNetworkHelper()
{
}

CNetworkHelper::~CNetworkHelper()
{
}

std::vector<boost::asio::ip::address> getLocalIps()
{
   std::vector<boost::asio::ip::address> result = new std::vector<boost::asio::ip::address>();
   boost::asio::io_service io_service;
   boost::asio::ip::udp::resolver resolver(io_service);
   boost::asio::ip::udp::resolver::query query("localhost", "");
   boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
   boost::asio::ip::udp::resolver::iterator end; // End marker.
   
   //we push every ip in the vector
   while (iter != end)
   {
      boost::asio::ip::address addr = iter->endpoint().address();
      //we manage only ipv4
      if(addr.is_v4())
         result.push_back(addr);
   }

   return result;
}
