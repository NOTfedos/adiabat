import matplotlib.pyplot as plt
import numpy as np

y_arr = open("data.txt", "r").read().split("\n")[:100]
y_arr = list(map(float, y_arr))
x_arr = range(len(y_arr))

#---------------------------/ Graphics /--------------------------

fig, ax = plt.subplots()

ax.minorticks_on()
ax.grid(which='major', linewidth=0.5, linestyle='dashed', color='black')

#устанавливаем подпись
ax.set_title(r"Давление от N")

#делаем штрихи внутрь
ax.tick_params(which='major', direction='in', width=1.5, length=8)
ax.tick_params(which='minor', direction='in', width=1.5, length=4)

#устанавливаем подписи к осям
ax.set_xlabel(r'N', fontsize='large')
ax.set_ylabel(r'p', fontsize='large')

#ax.axhline(linewidth=3)
#ax.axvline(linewidth=3)    


# Погрешности

ax.errorbar(x_arr, y_arr, fmt='s')


#---------------------------/ МНК /--------------------------------

z = np.polyfit(x_arr, y_arr, 1)
p = np.poly1d(z)

ax.errorbar(x_arr, p(x_arr), linewidth=2)

#---------------------------//-----------------------------------

plt.show()
