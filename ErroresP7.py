import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

# Definir el rango de valores de beta
betas = [0.72, 0.74, 0.80]  # Lista de valores de beta

# Factor de escala
f = 5.7

# Función de ajuste
def exponencial(x, a, b):
    return a * np.exp(b * x)

# Iterar sobre los valores de beta
for beta in betas:
    # Leer los datos correspondientes a cada beta
    filename = f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/LoopsRed24_beta{beta:.2f}.txt'
    data = np.loadtxt(filename)

    # Extraer columnas
    red_number = data[:, 0]
    loop_size = data[:, 1]
    mean_energy = data[:, 2]
    std_energy = data[:, 3]*f

    # Calcular el valor medio y el error de las energías
    unique_n = np.unique(loop_size)
    avg_energy = []
    error_energy = []

    for n in unique_n:
        indices = np.where(loop_size == n)
        avg_energy.append(np.mean(mean_energy[indices]))
        error_energy.append(np.sqrt(1 / (np.sum(1 / std_energy[indices]**2))))

    # Convertir listas a arrays
    avg_energy = np.array(avg_energy)
    error_energy = np.array(error_energy)

    # Ajuste de datos según beta
    if beta < 0.76:
        ua = unique_n**2
        xlabel = 'Área del Loop ($n^2$)'
        title = 'Comprobación Ley del Área'
    else:
        ua = unique_n * 4
        xlabel = 'Perímetro del Loop ($4n$)'
        title = 'Comprobación Ley del Perímetro'

    # Ajuste con la función exponencial
    ppot, pcov = curve_fit(exponencial, ua, avg_energy, p0=(1, -1), sigma=error_energy)

    # Crear gráfico
    plt.errorbar(ua, avg_energy, yerr=error_energy, fmt='o', capsize=5, capthick=2, label='Energía Media')
    plt.plot(ua, exponencial(ua, *ppot), 'r--',
             label=f'Ajuste:\na = {ppot[0]:.6f}\n$\sigma$ = {ppot[1]:.6f}')
    plt.xlabel(xlabel)
    plt.ylabel('Energía Media')
    plt.title(f'{title} (beta = {beta:.2f})')
    plt.legend()
    plt.savefig(f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/Plot_beta{beta:.2f}.png')
    plt.close()

    # Guardar resultados en archivo
    output_filename = f'C:/Users/ricar/Desktop/Pruebas VSC/Prubea/P7/Resultados_beta{beta:.2f}.txt'
    with open(output_filename, 'w') as file:
        file.write("n\tE\n")
        for i in range(len(unique_n)):
            file.write(f"{unique_n[i]}\t{avg_energy[i]:.6f} ± {error_energy[i]:.6f}\n")
        file.write(f"\nParámetros del ajuste:\n")
        file.write(f"a = {ppot[0]:.6f} ± {np.sqrt(pcov[0][0]):.6f}\n")
        file.write(f"b = {ppot[1]:.6f} ± {np.sqrt(pcov[1][1]):.6f}\n")

    print(f"Resultados para beta = {beta:.2f} guardados en {output_filename}")
