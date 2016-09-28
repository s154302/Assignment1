import matplotlib.pyplot as plt
import numpy as np

peaks = np.loadtxt('Rpeaks.txt')
times = np.loadtxt('Rpeaks_time.txt')
SBpeaks = np.loadtxt('SB_Rpeaks.txt')
SBtimes = np.loadtxt('SB_Rpeaks_time.txt')
thresh1 = np.loadtxt('Threshold1.txt')
thresh2 = np.loadtxt('Threshold2.txt')
data = np.loadtxt('vECG.txt')

plt.plot(data)
plt.plot(thresh1)
plt.plot(thresh2)
plt.plot(times, peaks, 'o')
plt.plot(SBtimes, SBpeaks, 'o')
plt.axvline(2669, color = 'r', linestyle='--')
plt.legend(['Data', 'Threshold1', 'Threshold2', 'Ordinary Peaks', 'Searchback Peaks', 'Warnings'])
plt.show()
