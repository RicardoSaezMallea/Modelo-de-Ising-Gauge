import numpy as np
import matplotlib.pyplot as plt

# Intervalos de beta
betas = np.arange(0.7, 0.85, 0.02)

# Crear el scatter plot
plt.figure(figsize=(10, 6))

# Colores para cada fichero
colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k', 'orange', 'purple', 'brown',
          'pink', 'gray', 'olive', 'cyan', 'magenta', 'black']


for i, beta in enumerate(betas):
    filename = f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/MetropolisTest_beta{beta:.2f}.txt'
    data = np.loadtxt(filename)
    x = data[20:30, 0]
    y = data[20:30, 1]/5000
    plt.scatter(x, y, label=f'beta={beta:.2f}', color=colors[i % len(colors)])

plt.plot(x,np.zeros(len(x)))
# Etiquetas y título
plt.xlabel('Pasos')
plt.ylabel('Diferencias de Energia Promedio')
plt.title('Termalización')
plt.legend()
plt.show()

