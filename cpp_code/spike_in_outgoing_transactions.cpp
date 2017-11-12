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

void input();
void detect_incoming_spikes();

int main()
{
  input();
  detect_incoming_spikes();
  return 0;
}


bool* used;
int* circ;

void detect_incoming_spikes()
{
  std::vector < std::vector<transaction>> day_transactions;
  for ( size_t i = 0;i<transactions.size();i++)
  {
    day_transactions.push_back(std::vector<transaction>());
  }
  int no_hours=3;
  int no_secs=60*60*no_hours;
  int n=0;
  for (auto it = sorted_transactions.begin();it< sorted_transactions.end();it++)
  {
    /* check if there is anything suspicious */
    std::vector<transaction> filtered_history;

    for ( auto cl_hist_it = day_transactions[it->source].begin();cl_hist_it<day_transactions[it->source].end();cl_hist_it++)
    {
      if (cl_hist_it->timestamp + no_secs < it->timestamp )
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
      printf("Detected strange behavior: many transactions... \n");
    }

      /* check for anything unorthodox */
    int same_amounts=0;
    for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
    {
      if ( cl_hist_it->amount == it->amount)
        same_amounts++;
    }
    if ( same_amounts>=5 )
    {
      printf("Detected same transactions\n"); // kinda ok, think about this later maybe...
    }
    // filter trans_history of target
   

    day_transactions[it->source]=filtered_history;
    day_transactions[it->source].push_back(*it);
  }

  for ( size_t i = 0;i<transactions.size();i++)
  {
    //printf("%d\n",trans_history[i].size());
  }
}


int trans_id_n;

void input()
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
        int source = source_it->int_id;
        int target= target_it->int_id;
        //if ( target!=1000)
        transaction t; 
        t.id=id;
        t.source = source;
        t.target = target;
        t.amount = amount;
        t.timestamp = getEpochTime(date,time);
        transactions[source].push_back(t);

        sorted_transactions.push_back(t);
      }
  }

  std::sort(sorted_transactions.begin(),sorted_transactions.end());
}

/** some more pattern detection **/
