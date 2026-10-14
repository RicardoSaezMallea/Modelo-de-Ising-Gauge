import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Leer los datos del fichero
beta = 0.80
filename = f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/LoopsRed24_beta{beta:.2f}.txt'
output = "Energias_Datos"
data = np.loadtxt(filename)

# Extraer columnas
red_number = data[:, 0]
loop_size = data[:, 1]
mean_energy = data[:, 2]
std_energy = data[:, 3]

#Factor de escala
f=5.7
std_energy = std_energy

# Calcular el valor medio y el error de las energías
unique_n = np.unique(loop_size)
unique_r = np.unique(red_number)
avg_energy = []
error_energy = []

for n in unique_n:
    indices = np.where(loop_size == n)
    avg_energy.append(np.mean(mean_energy[indices]))
    error_energy.append(np.sqrt(1/(np.sum(1/std_energy[indices]**2))))


# Convertir listas a arrays
avg_energy = np.array(avg_energy)
error_energy = np.array(error_energy)*f


#Ajuste Ley del área
def exponencial(x,a,b):
    return a*np.exp(b*x)

if(beta<0.76):
    ua = unique_n**2
    ppot, pcov = curve_fit(exponencial,ua,avg_energy,p0=(1,-1),sigma=error_energy)
    plt.errorbar(ua, avg_energy, yerr=error_energy, fmt='o', capsize=5, capthick=2, label='Energía Media')
    plt.plot(ua, exponencial(ua, *ppot), 'r--',
             label=f'Exponencial:\na = {ppot[0]:.6f}\nb = {ppot[1]:.6f}')
    plt.xlabel('Área del Loop ($n^2$)')
    plt.ylabel('Energía Media')
    plt.title(f'Comprobación Ley del Área (beta = {beta:.2f})')
    plt.legend()
else:
    ua = unique_n*4
    ppot, pcov = curve_fit(exponencial,ua,avg_energy,p0=(1,-1),sigma=error_energy)
    plt.errorbar(ua, avg_energy, yerr=error_energy, fmt='o', capsize=5, capthick=2, label='Energía Media')
    plt.plot(ua, exponencial(ua, *ppot), 'r--',
             label=f'Exponencial:\na = {ppot[0]:.6f}\nb = {ppot[1]:.6f}')
    plt.xlabel('Perímetro del Loop ($4n$)')
    plt.ylabel('Energía Media')
    plt.title(f'Comprobación Ley del Perímetro (beta = {beta:.2f})')
    plt.legend()
    
plt.show()

print("beta = ",beta)
print ("n\t","E")
for i in range (10):
    print(unique_n[i],"\t", avg_energy[i],"$\pm$",error_energy[i])
    
print("$a$=",ppot[0],"$\pm$",pcov[0][0])
print("$\sigma$=",ppot[1],"$\pm$",pcov[1][1])
