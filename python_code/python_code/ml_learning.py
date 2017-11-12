import datetime
import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

def is_outlier(points, thresh=19.5):
    """
    Returns a boolean array with True if points are outliers and False
    otherwise.

    Parameters:
    -----------
        points : An numobservations by numdimensions array of observations
        thresh : The modified z-score to use as a threshold. Observations with
            a modified z-score (based on the median absolute deviation) greater
            than this value will be classified as outliers.

    Returns:
    --------
        mask : A numobservations-length boolean array.

    References:
    ----------
        Boris Iglewicz and David Hoaglin (1993), "Volume 16: How to Detect and
        Handle Outliers", The ASQC Basic References in Quality Control:
        Statistical Techniques, Edward F. Mykytka, Ph.D., Editor.
    """
    if len(points.shape) == 1:
        points = points[:,None]
    median = np.median(points, axis=0)
    diff = np.sum((points - median)**2, axis=-1)
    diff = np.sqrt(diff)
    med_abs_deviation = np.median(diff)

    modified_z_score = 0.6745 * diff / med_abs_deviation

    return modified_z_score > thresh

if __name__=="__main__":
    atms = pd.read_csv("atms.small.csv")
    clients = pd.read_csv("clients.small.csv")
    companies = pd.read_csv("companies.small.csv")
    transactions = pd.read_csv("transactions.small.csv")
    client_ids={}
    for index,row in clients.iterrows():
        client_ids[row['id']]=0

    source_transactions=transactions.sort_values(['source','date','time'],ascending=[True,True,True])
    source_unique = source_transactions['source'].unique()

    target_transactions = transactions.sort_values(['target','date','time'])
    targets_unique = target_transactions['target'].unique()

    target_avg=0
    target_dict = {}
    for uniq in targets_unique:
        list  = target_transactions[ target_transactions['target'] == uniq]
        target_dict[uniq]  =list
    source_dict ={}
    for uniq in source_unique:
        list  = source_transactions[ source_transactions['source'] == uniq]
        source_dict[uniq]  =list

    x=[]
    y=[]
    avg=0.0
    inc_dates = 0
    total_transactions = 0
    for target in target_dict:
        data = target_dict[target]
        for index,row in data.iterrows():
            try:
                datetime.datetime.strptime(row['date'], '%m/%d/%Y')
            except ValueError:
                inc_dates+=1

        target_avg+=len(data)
        x.append(target)
        y.append(len(data))
        avg+=(len(data))
        total_transactions+=len(data)
        #print len(target)
    print("Incorrect dates: {}".format(inc_dates))
    avg/=len(target_dict)
    fraud_transactions = 0
    for target in target_dict:
        data = target_dict[target]
        if len(data) > avg*3:
            print "Fraud is found!"
            print target
            print (avg,len(data))
            for index,row in data.iterrows():
                fraud_transactions +=1
                print row['date'],row['time'],row['amount'],row['source']
            print "Reporting fraud is over"
    print "Percentage of found frauds is {}%".format(100.0*fraud_transactions/total_transactions)
    #sns.distplot(y)
    #print y
    target_avg/=len(target_dict)
    #clf = RandomForestClassifier(source_transactions)
    #print target_transactions

    clients = {}
    for index,row in transactions.iterrows():
        source = row['source']
        target = row['target']
        if source in client_ids:
            if not source in clients:
                clients[source]=0
            clients[source] -= row['amount']

        if target in client_ids:
            if not target in clients:
                clients[target]=0
            clients[target] += row['amount']

    # check whether it is a company
    outliers = is_outlier(np.asarray(clients.values()))
    keys = clients.keys()
    fraud_transactions = 0
    fraud_clients = 0
    k=0
    for val in clients.values():
        print k,val
        k+=1
    for i in range(len(keys)):
       if outliers[i]:
            fraud_clients +=1
            print "Outlier found {}".format(clients.values()[i])
            print "Checking transactions...."
            print "Transactions towards the client"
            if keys[i] in target_dict:
                for index,row in target_dict[keys[i]].iterrows():
                    fraud_transactions+=1
                    print row['date'],row['time'],row['amount']
            print "Transactions from the client"
            if keys[i] in source_dict:
                for index,row in source_dict[keys[i]].iterrows():
                    fraud_transactions+=1
                    print row['date'],row['time'],row['amount']
    print len(clients), fraud_clients
    print "Percentage of found frauds is {}%".format(100.0*fraud_transactions/total_transactions)
    sns.distplot(clients.values())
    plt.show()

# time-dependent stuff later
# where does the money end?!
