import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

x_arr = []
y_arr = []

for line in open("data.txt", "r"):
    p, v = list(map(float, line.split(" ")))
    if (p != 0) and (v != 0):
        x_arr.append(v)
        y_arr.append(p)

#---------------------------/ Graphics /--------------------------

fig, ax = plt.subplots()

ax.minorticks_on()
ax.grid(which='major', linewidth=0.5, linestyle='dashed', color='black')

#устанавливаем подпись
ax.set_title(r"pV - диаграмма")

#делаем штрихи внутрь
ax.tick_params(which='major', direction='in', width=1.5, length=8)
ax.tick_params(which='minor', direction='in', width=1.5, length=4)

#устанавливаем подписи к осям
ax.set_xlabel(r'V, у.е.', fontsize='large')
ax.set_ylabel(r'p, у.е.', fontsize='large')

#ax.axhline(linewidth=3)
#ax.axvline(linewidth=3)    


# Погрешности

ax.errorbar(x_arr, y_arr, fmt='s')


#---------------------------/ МНК /--------------------------------

def f(x, a):
    return a*pow(x, -1)

z = curve_fit(f, x_arr, y_arr)
print(z[0][0])

#z = np.polyfit(x_arr, y_arr, 1)
y_app = []
for el in (x_arr):
    y_app.append(f(el,z[0][0]))

ax.errorbar(x_arr, y_app, linewidth=2)

#---------------------------//-----------------------------------

plt.show()
