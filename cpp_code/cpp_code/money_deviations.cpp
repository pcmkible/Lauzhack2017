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

int no_clients;

void stream();

int main()
{
  stream();
  return 0;
}


bool* used;
int* circ;
std::vector <double> cur_money;
std::vector <int> reported;
void process_transaction(transaction &trans)
{

    /* check if there is anything suspicious */

    // filter trans_history of target
   
  //printf("%lf %lf\n",cur_money[trans.source],cur_money[trans.target]); 
  cur_money[trans.source] -= trans.amount; 
  cur_money[trans.target] += trans.amount; 

  if ( fabs(cur_money[trans.source]) > 4e4 && reported[trans.source] == 0 )
  {
    printf("Account has a lot/little of money %d %lf\n",trans.source,cur_money[trans.source]);
    reported[trans.source] = 1;
  }
  if ( fabs(cur_money[trans.target]) > 4e4 && reported[trans.target] == 0)
  {
    printf("Account has a lot/little of money %d %lf\n",trans.target,cur_money[trans.target]);
    reported[trans.target] = 1;
  }
  /** maybe only at the end **/
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
  }

  for ( int i = 0;i<no_clients;i++)
  {
    cur_money.push_back(0);
    reported.push_back(0);
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


