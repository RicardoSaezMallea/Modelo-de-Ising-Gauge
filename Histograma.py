import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Leer el fichero de datos
data = np.loadtxt('InicializarRedTest.txt')

# Crear el histograma normalizado
hist, bins = np.histogram(data, bins=51, density=True)
bin_centers = (bins[:-1] + bins[1:]) /2

# Graficar el histograma
plt.hist(data, bins=51, density=True, edgecolor='black')
plt.title('Histograma de Energías Normalizado')
plt.xlabel('Energía')
plt.ylabel('Frecuencia (Normalizada)')
plt.show()

def gaussian(x, amp, std, c):
    return amp * np.exp(-(x-c)**2 / (2 * std**2))

popt, pcov = curve_fit(gaussian, bin_centers, hist, p0=(0.07,10,0))

amp,std,c = popt

plt.hist(data, bins=51, density=True, alpha=0.6, color='g', edgecolor='black', label='Datos')
x = np.linspace(min(data), max(data), 1000)
plt.plot(x, gaussian(x, *popt), 'r--',
         label=f'Gaussiana Ajustada:\namplitud = {amp:.2f}\nstd = {std:.2f}\nc={c:.5f}')
plt.title('Histograma de Energías con Ajuste Gaussiano')
plt.xlabel('Energía')
plt.ylabel('Frecuencia (Normalizada)')
plt.legend()
plt.show()