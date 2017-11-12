#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>
struct transaction
{
  std::string id;
  int source,target;
  std::string s_source,s_target;
  double amount;
  std::string currency;
  std::time_t timestamp;
  std::string date,time;
  void print()
  {
    std::cout << id <<"," << s_source << "," << s_target<< "," << date<< "," << time << "," <<amount << "," << currency <<std::endl;
  }
};

struct raw_transaction
{
  std::string id;
  std::string source,target;
  double amount;
  std::string currency;
  std::string date,time;
  std::time_t timestamp;
};

struct client_involved
{
  client_involved(std::time_t involved_at,int id,transaction trans): involved_at(involved_at),id(id),trans(trans){}
  std::time_t involved_at;
  int id;
  transaction trans;
};

struct client
{
  client( int int_id, std::string id):int_id(int_id),str_id(id){}
  int int_id;
  std::string str_id;
};

bool operator < ( client i1, client i2)
{
  return i1.str_id < i2.str_id;
}

bool operator < ( raw_transaction t1, raw_transaction t2)
{
  return t1.timestamp < t2.timestamp;
}

bool operator < ( transaction t1, transaction t2)
{
  return t1.timestamp < t2.timestamp;
}

std::time_t getEpochTime(std::string& date,std::string& time)
{
  std::string dateTime = date+std::string("T")+time;
   // Let's consider we are getting all the input in
   // this format: '2014-07-25T20:17:22Z' (T denotes
   // start of Time part, Z denotes UTC zone).
   // A better approach would be to pass in the format as well.
   static const std::string dateTimeFormat("%m/%d/%YT%H:%M:%S");

   // Create a stream which we will use to parse the string,
   // which we provide to constructor of stream to fill the buffer.
   std::istringstream ss{ dateTime };

   // Create a tm object to store the parsed date and time.
   std::tm dt;

   // Now we read from buffer using get_time manipulator
   // and formatting the input appropriately.
   ss >> std::get_time(&dt, dateTimeFormat.c_str());

   // Convert the tm structure to time_t value and return.
   return std::mktime(&dt);
}
