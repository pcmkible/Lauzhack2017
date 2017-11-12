import pandas as pd
import graph_plotter
#df = pd.read_csv("data/incoming_data1.csv")
#df = pd.read_csv("data/outgoing_data.csv")
#df = pd.read_csv("data/circular_scheme.csv")
df = pd.read_csv("data/circular_scheme2.csv")
graph_plotter.plot(df)
