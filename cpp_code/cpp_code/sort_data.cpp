#include <fstream>
#include "include/csv.h"
#include <iostream>
#include <string>
#include <sstream>
#include "include/data_structures.hpp"
#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include "include/json.hpp"
#include <set>
#include <algorithm>
int no_chunks = 16;
int total_size=0;
//int total_rows=138581151;
int total_rows=18208;
int chunk_size = total_rows/no_chunks+100;

void split_into_chunks()
{
  /** input clients **/
  io::CSVReader<7> transaction_in("../big_data/transactions.csv");
  transaction_in.read_header(io::ignore_extra_column, "id", "source", "target","date","time","amount","currency");
  std::string id,source,target,date,time,currency;
  double amount;
  std::ofstream files[no_chunks];
  for ( int i=0;i<no_chunks;i++)
  {
    int a = i;
    std::stringstream ss;
    ss << a;
    std::string str = ss.str();
    files[i]=std::ofstream(std::string("../big_data/chunk_0_")+str+".csv");
    files[i] << "id,source,target,date,time,amount,currency" << std::endl;
  }

  std::vector <raw_transaction> sorted_transactions;
  int cnt=0;
  int total_size=0;
  while(transaction_in.read_row(id,source,target,date,time,amount,currency))
  {
    raw_transaction t; 
    t.id=id;
    t.source = source;
    t.target = target;
    t.amount = amount;
    t.timestamp = getEpochTime(date,time);
    t.time=time;
    t.date=date;
    sorted_transactions.push_back(t);
    total_size++;
    if ( total_size == chunk_size)
    {
      std::sort(sorted_transactions.begin(),sorted_transactions.end());
      for (auto it = sorted_transactions.begin();it<sorted_transactions.end();it++)
          files[cnt] << it->id << "," << it->source << "," << it->target << "," << it->date << "," << it->time << "," << it->amount << "," << it->currency << std::endl;
      cnt++;
      sorted_transactions.clear();
      total_size=0;
    }
  }
  std::sort(sorted_transactions.begin(),sorted_transactions.end());
  for (auto it = sorted_transactions.begin();it<sorted_transactions.end();it++)
      files[cnt] << it->id << "," << it->source << "," << it->target << "," << it->date << "," << it->time << "," << it->amount << "," << it->currency << std::endl;
  cnt++;
  sorted_transactions.clear();
  for ( int i=0;i<no_chunks;i++)
  {
    files[i].close();
  }
}

void merge_chunks(int rbr, int first, int second,int res)
{

  std::stringstream ss1;
  ss1 << "../big_data/chunk_" << rbr-1 << "_" << first <<".csv";
  std::string s1 = ss1.str();
  io::CSVReader<7> t1(s1);
  t1.read_header(io::ignore_extra_column, "id", "source", "target","date","time","amount","currency");

   
  std::stringstream ss2;
  ss2 << "../big_data/chunk_" << rbr-1 << "_" << second <<".csv";
  std::string s2 = ss2.str();
  io::CSVReader<7> t2(s2);
  t2.read_header(io::ignore_extra_column, "id", "source", "target","date","time","amount","currency");
  

  int a = res;
  std::stringstream ss;
  ss << "../big_data/chunk_"  << rbr << "_" << a <<".csv";
  std::ofstream f=std::ofstream(ss.str());
  f << "id,source,target,date,time,amount,currency" << std::endl;


  std::string id1,source1,target1,date1,time1,currency1;
  std::string id2,source2,target2,date2,time2,currency2;
  double amount1, amount2;
  std::ofstream files[no_chunks];

  
  t1.read_row(id1,source1,target1,date1,time1,amount1,currency1);
  t2.read_row(id2,source2,target2,date2,time2,amount2,currency2);


  bool both_not_full=true;
  while ( both_not_full)
  {
    
    std::time_t timestamp1= getEpochTime(date1,time1);
    std::time_t timestamp2= getEpochTime(date2,time2);
    if ( timestamp1 < timestamp2)
    {
      f << id1 << "," << source1 << "," << target1 << "," << date1 << "," << time1 << "," << amount1 << "," << currency1 << std::endl;
      if (!t1.read_row(id1,source1,target1,date1,time1,amount1,currency1))
      {
        while (t2.read_row(id2,source2,target2,date2,time2,amount2,currency2))
          f << id2 << "," << source2 << "," << target2 << "," << date2 << "," << time2 << "," << amount2 << "," << currency2 << std::endl;
        both_not_full=false;
      }
    }
    else
    {
      f << id2 << "," << source2 << "," << target2 << "," << date2 << "," << time2 << "," << amount2 << "," << currency2 << std::endl;
      if (!t2.read_row(id2,source2,target2,date2,time2,amount2,currency2))
      {
        while (t1.read_row(id1,source1,target1,date1,time1,amount1,currency1))
          f << id1 << "," << source1 << "," << target1 << "," << date1 << "," << time1 << "," << amount1 << "," << currency1 << std::endl;
        both_not_full=false;
      }
    }
  }
  f.close();
  // delete both files
  remove(s1.c_str());
  remove(s2.c_str());
}
int main() 
{
  split_into_chunks();
  int no_ch=16;
  for ( int rbr=1;no_ch>1;rbr++)
  {
    for ( int i=0;i<no_ch;i+=2)
    merge_chunks(rbr,i,i+1,i/2); 
    no_ch/=2;
  }

  return 0;
}

