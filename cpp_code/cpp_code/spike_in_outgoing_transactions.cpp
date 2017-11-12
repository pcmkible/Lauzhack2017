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
std::vector <std::vector<transaction>> transactions; // transactions[i] -> transactions where "i" is a source 

std::vector <transaction> sorted_transactions;

void process_transaction(transaction &trans);
void stream();

int main()
{
  stream();
  return 0;
}


std::vector < std::vector<transaction>> day_transactions;
void process_transaction(transaction &trans)
{
  int no_hours=3;
  int no_secs=60*60*no_hours;
  int n=0;
  {
    /* check if there is anything suspicious */
    std::vector<transaction> filtered_history;

    for ( auto cl_hist_it = day_transactions[trans.source].begin();cl_hist_it<day_transactions[trans.source].end();cl_hist_it++)
    {
      if (cl_hist_it->timestamp + no_secs < trans.timestamp )
      {
        continue;
      }
      else
      {
        filtered_history.push_back(*cl_hist_it);
      }
    }
    if ( filtered_history.size() > 10)
    {
      printf("Detected strange behavior: many transactions... \n\n");
      for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
      {
        cl_hist_it->print();
      }
      printf("\n\n");
    }

      /* check for anything unorthodox */
    int same_amounts=0;
    for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
    {
      if ( cl_hist_it->amount == trans.amount)
        same_amounts++;
    }
    if ( same_amounts>=5 )
    {
      printf("Detected same transactions\n\n"); // kinda ok, think about this later maybe...
      for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
      {
        if ( cl_hist_it->amount == trans.amount)
        {
          cl_hist_it->print();
        }
      }
    }
    // filter trans_history of target
   
    day_transactions[trans.source]=filtered_history;
    day_transactions[trans.source].push_back(trans);
  }

  for ( size_t i = 0;i<transactions.size();i++)
  {
    //printf("%d\n",trans_history[i].size());
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
    transactions.push_back(std::vector<transaction>());
    client_count++;
  }

  for ( size_t i = 0;i<transactions.size();i++)
  {
    day_transactions.push_back(std::vector<transaction>());
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

        process_transaction(t);
      }
  }

}

/** some more pattern detection **/
