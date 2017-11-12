import numpy as np
import matplotlib.pyplot as plt
import pylab
import networkx as nx


def plot(data):
    G = nx.DiGraph()
    ids={}
    #rename ids

    for index,row in data.iterrows():
        if not row["source"] in ids:
            ids[row["source"]] = len(ids)
        if not row["target"] in ids:
            ids[row["target"]] = len(ids)

    for index,row in data.iterrows():
        G.add_edge(ids[row["source"]],ids[row["target"]], weight=row["amount"])


    edge_labels=dict([((u,v,),d['weight'])
                      for u,v,d in G.edges(data=True)])
    #red_edges = [('C','D'),('D','A')]
    #edge_colors = ['black']

    pos=nx.random_layout(G)
    #pos=nx.spring_layout(G)
    nx.draw_networkx_edge_labels(G,pos,edge_labels=edge_labels)
    #nx.draw(G,pos, node_color = values, node_size=1500,edge_color=edge_colors,edge_cmap=plt.cm.Reds)
    nx.draw(G,pos, node_size=150,edge_cmap=plt.cm.Reds)
    pylab.show()
