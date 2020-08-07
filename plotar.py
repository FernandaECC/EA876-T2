
import numpy as np

import matplotlib.pyplot as plt

bar_names = []
bar_heights = []
desvio = []
bar_names2 = []
bar_heights2 = []
desvio2 = []
bar_names3 = []
bar_heights3 = []
desvio3 = []


width_n = 0.5 

textfile1 = open("plot_linear.txt", "r")
textfile2 = open("plot_processo.txt", "r")
textfile3 = open("plot_thread.txt", "r")

for line in textfile1:

	bar_name1, bar_height1, desvio1_ = line.split()
	

for line in textfile2:

	bar_name2, bar_height2, desvio2_ = line.split()
	

for line in textfile3:

	bar_name3, bar_height3, desvio3_ = line.split()







bar_names.append(bar_name3)	
bar_names.append(bar_name2)		
bar_names.append(bar_name1)


bar_heights.append(float(bar_height3))
bar_heights.append(float(bar_height2))
bar_heights.append(float(bar_height1))


desvio.append(float(desvio3_))
desvio.append(float(desvio2_))
desvio.append(float(desvio1_))
print(bar_heights)
print(desvio)
x_pos = np.arange(len(bar_names))


fig, ax = plt.subplots()
ax.bar(x_pos, bar_heights,
       yerr= desvio,
       align='center',
       alpha=0.5,
       ecolor='black',
       capsize=10)
ax.set_ylabel('Tempo médio')
ax.set_xticks(x_pos)
ax.set_xticklabels(bar_names)
ax.set_title('Tempo médio de execução após 100 execuções')
ax.yaxis.grid(True)


plt.tight_layout()

plt.show()



