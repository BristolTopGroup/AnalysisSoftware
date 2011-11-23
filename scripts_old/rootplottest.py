#import matplotlib
## matplotlib.use("PDF") ## Include this line to make PDF output
#matplotlib.rc('text', usetex=True)
#matplotlib.rc('font', family="serif", serif="palatino")
from matplotlib import pyplot as plt
import ROOT
import rootplot.root2matplotlib as r2m
from random import gauss

th1f_1 = ROOT.TH1F("hpt1", "Distribution of pT;pT;Events", 40, 0, 8)
th1f_2 = ROOT.TH1F("hpt2", "Distribution of pT;pT;Events", 40, 0, 8)
for i in range(2500):
    th1f_1.Fill(gauss(4, 1.))
    th1f_2.Fill(gauss(6, 1.))

cmap = plt.get_cmap('Set3')
colors = [cmap(i/5.) for i in range(5)]

plt.figure(1, (6, 4))
ax1 = plt.axes()
replace = [('pT', r'$p_\mathrm{T}$'),
           ('ZMM', r'$Z\rightarrow \mu\mu$'),
           ('ttbar', r'$t\bar{t}$')]
hist1 = r2m.Hist(th1f_1, replace)
hist2 = r2m.Hist(th1f_2, replace)
stack = r2m.HistStack()
stack.add(hist1, color=colors[0], label="ZMM")
stack.add(hist2, color=colors[1], label="ttbar")
stack.bar(alpha=0.5)
stack.show_titles()
plt.legend(loc='upper left')
plt.xlim(0,8)

plt.savefig('second')


plt.close()
del th1f_1
del th1f_2

th1f_1 = ROOT.TH1F("hpt1", "Distribution of pT;pT;Events", 40, 0, 10)
th1f_2 = ROOT.TH1F("hpt2", "Distribution of pT;pT;Events", 40, 0, 10)
for i in range(2500):
    th1f_1.Fill(gauss(4, 1.))
    th1f_2.Fill(gauss(6, 1.))

cmap = plt.get_cmap('Set3')
colors = [cmap(i/5.) for i in range(5)]

plt.figure(1, (6, 4))
ax1 = plt.axes()
replace = [('pT', r'$p_\mathrm{T}$')]
hist1 = r2m.Hist(th1f_1)
hist2 = r2m.Hist(th1f_2)
stack = r2m.HistStack()

stack.add(hist1, color='b', label=r"$Z\rightarrow \mu\mu$")
stack.add(hist2, color='r', label=r"$t\bar{t}$")
#stack.hist(histtype='bar')
stack.histstack(histtype = "stepfilled")
#stack.barstack()
stack.show_titles(replace=replace)
plt.legend(loc='upper left')
plt.xlim(0,10)

plt.show()
plt.savefig('third') ## Include this line to save image