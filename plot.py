import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('core1.csv')
plot_x1 = df1['xaxis']
plot_y1 = df1['yaxis']

plt.plot(plot_x1, plot_y1)

df2 = pd.read_csv('core2.csv')
plot_x2 = df2['xaxis']
plot_y2 = df2['yaxis']

plt.plot(plot_x2, plot_y2)

df3 = pd.read_csv('core3.csv')
plot_x3 = df3['xaxis']
plot_y3 = df3['yaxis']

plt.plot(plot_x3, plot_y3)

df4 = pd.read_csv('core4.csv')
plot_x4 = df4['xaxis']
plot_y4 = df4['yaxis']

plt.plot(plot_x4, plot_y4)
plt.show()