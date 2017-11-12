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
void detect_circles();

int main()
{
  input();
  detect_circles();
  return 0;
}


bool* used;
int* circ;

void detect_circles()
{
  std::vector <std::vector<client_involved>> trans_history;
  for ( size_t i = 0;i<transactions.size();i++)
  {
    trans_history.push_back(std::vector<client_involved>());
  }
  printf("No of clients: %d\n",transactions.size());
  int no_days=5;
  int no_secs=60*60*24*no_days;
  int n=0;
  for (auto it = sorted_transactions.begin();it< sorted_transactions.end();it++)
  {
    /* check if there is anything suspicious */
    std::vector<client_involved> filtered_history;

    for ( auto cl_hist_it = trans_history[it->source].begin();cl_hist_it<trans_history[it->source].end();cl_hist_it++)
    {
      if (cl_hist_it->involved_at + no_secs < it->timestamp )
      {
        continue;
      }
      else
      {
        filtered_history.push_back(*cl_hist_it);
      }
    }
    for ( auto cl_hist_it = filtered_history.begin();cl_hist_it<filtered_history.end();cl_hist_it++)
    {
      if ( cl_hist_it->id == it->target)
      {
        printf("Suspicious circle %d\n",it->target);
        n+=1;
      }
      else
      {
        trans_history[it->target].push_back(*cl_hist_it);
      }
    }

    // filter trans_history of target
   

    trans_history[it->source]=filtered_history;
    trans_history[it->target].push_back(client_involved(it->timestamp,it->source));
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


