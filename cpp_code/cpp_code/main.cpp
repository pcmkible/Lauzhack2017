#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include "include/json.hpp"
#include "include/csv.h"
#include <set>
#include "include/data_structures.hpp"
#include <algorithm>
/** to do -> include atms and companies in your analysis */

using json = nlohmann::json;
std::set<client> str_to_id_map;

std::vector <std::vector<client_involved>> trans_history;

int no_clients;

void stream();
void process_transaction(transaction &trans);
int main()
{
  stream();
  return 0;
}


bool* used;
int* circ;

int no_days=5;
int no_secs=60*60*24*no_days;
int n=0;

void process_transaction(transaction &trans)
{
  {

    /* check if there is anything suspicious */
    std::vector<client_involved> filtered_history;

    for ( auto cl_hist_it = trans_history[trans.source].begin();cl_hist_it<trans_history[trans.source].end();cl_hist_it++)
    {
      if (cl_hist_it->involved_at + no_secs < trans.timestamp )
      {
        continue;
      }
      else
      {
        filtered_history.push_back(*cl_hist_it);
      }
    }
    bool is_suspicious_circle=false;
    for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
    {
      if ( cl_hist_it->id == trans.target)
      {
        is_suspicious_circle=true;
      }
      else
      {
        trans_history[trans.target].push_back(*cl_hist_it);
      }
    }
    if (is_suspicious_circle)
    {
      printf("Suspicious circle %d\n\n",trans.target);
      for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
      {
        cl_hist_it->trans.print();
        n+=1;
      }
    }
    // filter trans_history of target
   

    trans_history[trans.source]=filtered_history;
    trans_history[trans.target].push_back(client_involved(trans.timestamp,trans.source,trans));
  }
}


int trans_id_n;

void stream()
{

  /** input clients **/
  
  io::CSVReader<1> client_in("../data/clients.small.csv");
  client_in.read_header(io::ignore_extra_column, "id");
  std::string client_id;
  int client_count =0;
  while(client_in.read_row(client_id))
  {
    str_to_id_map.insert(client(client_count,client_id));
    client_count++;
  }
  no_clients=client_count;
  for ( int i = 0;i<client_count;i++)
  {
    trans_history.push_back(std::vector<client_involved>());
  }

  //std::ifstream transactions_file("transactions.small.csv");
  io::CSVReader<7> transaction_in("../data/transactions.small.csv");
  transaction_in.read_header(io::ignore_extra_column, "id", "source", "target","date","time","amount","currency");
  std::string id,source,target,date,time,currency;
  double amount;
  while(transaction_in.read_row(id,source,target,date,time,amount,currency))
  {
      auto source_it = str_to_id_map.find(client(-1,source));
      auto target_it = str_to_id_map.find(client(-1,target));
      if ( source_it == str_to_id_map.end() || target_it == str_to_id_map.end())
      {
      }
      else
      {
        transaction t; 
        t.s_source=source;
        t.s_target=target;
        int source = source_it->int_id;
        int target= target_it->int_id;
        //if ( target!=1000)
        t.id=id;
        t.source = source;
        t.target = target;
        t.amount = amount;
        t.timestamp = getEpochTime(date,time);
        t.time=time;
        t.date=date;

        //transactions[source].push_back(t);
        process_transaction(t);
      }
  }


}

/** some more pattern detection **/


