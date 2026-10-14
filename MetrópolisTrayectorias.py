import numpy as np
import matplotlib.pyplot as plt

# Intervalos de beta
betas = [0.1,0.3,0.5,0.7,1.0,10.0]

# Crear el scatter plot
plt.figure(figsize=(10, 6))

# Colores para cada fichero
colors = ['r', 'g', 'b', 'c', 'm', 'y', 'k', 'orange', 'purple', 'brown',
          'pink', 'gray', 'olive', 'cyan', 'magenta', 'black']


for i, beta in enumerate(betas):
    filename = f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/MetrópolisTest_beta{beta:.1f}.txt'
    data = np.loadtxt(filename)
    x = data[:, 0]
    y = data[:, 1]
    plt.scatter(x, y, label=f'beta={beta:.2f}', color=colors[i % len(colors)])


# Etiquetas y título
plt.xlabel('Pasos')
plt.ylabel('Energía')
plt.title('Evolución de la energía')
plt.legend()
plt.show()