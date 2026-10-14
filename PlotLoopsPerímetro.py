import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Leer los datos del fichero
filename = 'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/Error_beta0.80.txt'
data = np.loadtxt(filename)

# Extraer columnas
red_number = data[:, 0]
loop_size = data[:, 1]
mean_energy = data[:, 2]
std_energy = data[:, 3]

# Calcular el valor medio y el error de las energías
unique_n = np.unique(loop_size)
avg_energy = []
error_energy = []

for n in (unique_n):
    indices = np.where(loop_size == n)
    avg_energy.append(np.mean(mean_energy[indices]))

def covariance_matrix(std_devs, rho):
    """Crea una matriz de covarianza basada en una correlación común rho."""
    n = len(std_devs)
    cov_matrix = np.zeros((n, n))
    for i in range(n):
        for j in range(n):
            if i == j:
                cov_matrix[i, j] = std_devs[i]**2
            else:
                cov_matrix[i, j] = rho * std_devs[i] * std_devs[j]
    return cov_matrix

# Ejemplo:
indices = np.where(red_number != 25)
rho = 1  # Correlación común
cov_matrix = covariance_matrix(std_energy[indices], rho)

jndices = np.where(red_number == 25)
aux=[]
sigma_0 = std_energy[jndices]
sigma_eff = []

for i in range (len(cov_matrix[:][0])):
    aux.append(np.sqrt(np.sum(cov_matrix[i][indices])))
    
aux = np.array(aux)

for n in (unique_n):
    indices = np.where(np.logical_and(loop_size == n, red_number != 25))
    sigma_eff.append(np.sum(np.sqrt(1/(sum(1/(aux[indices]**2))))))

f = sigma_eff/sigma_0

print(f)







