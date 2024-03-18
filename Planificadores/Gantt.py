import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import argparse

# Función para leer y procesar los datos de un algoritmo
def procesar_datos(file):
    df = pd.read_csv(file)

    # Calcular métricas importantes
    df['Tiempo de Espera'] = df['Inicio de Ejecución'] - df['Tiempo de Llegada']
    df['Tiempo de Retorno'] = df['Fin de Ejecución'] - df['Tiempo de Llegada']
    df['Tiempo de Respuesta'] = df['Tiempo de Espera']  # Asumiendo igualdad para simplificación

    return df

# Función para calcular métricas de rendimiento
def calcular_metricas(df, RoundRobin = False):
    metrics = {}
    # Tiempo de espera promedio: Tiempo que cada proceso pasa en la cola antes de su ejecución.
    # Es una métrica crucial para evaluar la percepción de rapidez de un sistema por parte del usuario.
    metrics['tiempo_espera_promedio'] = df['Tiempo de Espera'].mean()

    # Tiempo de retorno promedio: Tiempo total desde la llegada del proceso hasta su finalización.
    # Importante para entender el tiempo total que un proceso requiere para completarse, incluyendo el tiempo de espera y de ejecución.
    metrics['tiempo_retorno_promedio'] = df['Tiempo de Retorno'].mean()

    # Tiempo de respuesta promedio: Tiempo desde que el proceso llega hasta que se inicia su ejecución.
    # En este caso, se asume igual al tiempo de espera para simplificación, pero en algoritmos con desalojo podría ser diferente.
    if RoundRobin:
        # Ordenar el dataframe por 'ID Proceso' y luego por 'Inicio de Ejecución'
        df_sorted = df.sort_values(by=['ID Proceso', 'Inicio de Ejecución'])
        # Eliminar duplicados manteniendo la primera aparición para obtener el inicio de ejecución de cada proceso
        df_first_execution = df_sorted.drop_duplicates(subset='ID Proceso', keep='first')
        # Calcular el tiempo de respuesta como la diferencia entre 'Inicio de Ejecución' y 'Tiempo de Llegada'
        df_first_execution.loc[:,'Tiempo de Respuesta'] = df_first_execution['Inicio de Ejecución'] - df_first_execution['Tiempo de Llegada']
        # Calcular el tiempo de respuesta promedio específicamente para Round Robin
        metrics['tiempo_respuesta_promedio'] = df_first_execution['Tiempo de Respuesta'].mean()
    else:
        metrics['tiempo_respuesta_promedio'] = df['Tiempo de Respuesta'].mean()

    # Variabilidad del tiempo de espera: Mide cuánto varía el tiempo de espera entre diferentes procesos.
    # Una alta variabilidad puede indicar una falta de equidad en el tratamiento de procesos.
    metrics['var_tiempo_espera'] = df['Tiempo de Espera'].std()

    # Máximo tiempo de espera: El mayor tiempo que un proceso ha esperado antes de ejecutarse.
    # Útil para identificar casos extremos y evaluar la equidad del algoritmo.
    metrics['max_tiempo_espera'] = df['Tiempo de Espera'].max()

    # Utilización de la CPU: Porcentaje del tiempo total que la CPU estuvo ejecutando procesos.
    # Una alta utilización indica eficiencia, mientras que una baja utilización puede señalar ineficiencias o tiempo ocioso de la CPU.

    # El tiempo total observado es el periodo desde el inicio de la primera ejecución hasta el final de la última ejecución.
    tiempo_total_observado = df['Fin de Ejecución'].max() - df['Tiempo de Llegada'].min()
    
    if RoundRobin:
        # Para Round Robin, consideramos cada periodo de ejecución individualmente.
        # Sumamos solo las duraciones efectivas donde la CPU estuvo ejecutando procesos.
        tiempo_total_actividad_cpu = df['Fin de Ejecución'].max() - df['Inicio de Ejecución'].min()
    else:
        # En otros algoritmos, si asumimos una ejecución continua sin grandes periodos de inactividad,
        # podemos simplificar el cálculo a la suma de las duraciones de ráfaga.
        tiempo_total_actividad_cpu = df['Duración de la Ráfaga'].sum()
    
    # Utilización de la CPU: Porcentaje del tiempo de actividad de la CPU sobre el tiempo total observado.
    metrics['utilizacion_cpu'] = (tiempo_total_actividad_cpu / tiempo_total_observado) * 100

    # Rendimiento: Número de procesos completados por unidad de tiempo.
    # Esta métrica ayuda a entender cuánto trabajo es capaz de realizar el sistema en un tiempo determinado.
    metrics['rendimiento'] = len(df) / tiempo_total_observado

    return metrics

# Función para imprimir métricas de rendimiento
def imprimir_metricas(metrics):
    print(f"Tiempo de Espera Promedio: {metrics['tiempo_espera_promedio']:.2f} unidades de tiempo")
    print(f"Tiempo de Retorno Promedio: {metrics['tiempo_retorno_promedio']:.2f} unidades de tiempo")
    print(f"Tiempo de Respuesta Promedio: {metrics['tiempo_respuesta_promedio']:.2f} unidades de tiempo")
    print(f"Variabilidad del Tiempo de Espera: {metrics['var_tiempo_espera']:.2f}")
    print(f"Máximo Tiempo de Espera: {metrics['max_tiempo_espera']} unidades de tiempo")
    print(f"Utilización de la CPU: {metrics['utilizacion_cpu']:.2f}%")
    print(f"Rendimiento: {metrics['rendimiento']:.2f} procesos/unidad de tiempo")

# Función para generar el diagrama de Gantt
def generar_diagrama_gantt(df, titulo):
    fig, ax = plt.subplots(figsize=(10, 6))
    for index, row in df.iterrows():
        inicio = row['Inicio de Ejecución']
        fin = row['Fin de Ejecución']
        duracion = fin - inicio
        ax.barh(row['ID Proceso'], duracion, left=inicio, color='skyblue', edgecolor='black')

    ax.set_xlabel('Tiempo')
    ax.set_ylabel('ID Proceso')
    ax.set_title(titulo)
    ax.invert_yaxis()  # Invertir el eje Y
    plt.grid(axis='x')
    plt.show()

# Main script para procesar los datos de Round Robin como ejemplo
# Configuración del analizador de argumentos
parser = argparse.ArgumentParser(description='Genera un diagrama de Gantt a partir de un archivo CSV de procesos.')
parser.add_argument('file', type=str, help='Ruta al archivo CSV que contiene los datos de los procesos.')

# Parsear los argumentos de línea de comandos
args = parser.parse_args()

file = args.file
df = procesar_datos(file)
metrics_rr = calcular_metricas(df, True)
print("Métricas para Planificador: "+file)
imprimir_metricas(metrics_rr)
if (file == 'data/rr_procesos.csv'):
    generar_diagrama_gantt(df, 'Diagrama de Gantt para Planificador Round Robin')
else:
    if (file == 'data/fcfs_procesos.csv'):
        generar_diagrama_gantt(df, 'Diagrama de Gantt para planificador FCFS')
    elif (file == 'data/sjf_procesos.csv'):
        generar_diagrama_gantt(df, 'Diagrama de Gantt para Planificador SJF')
    elif (file == 'data/prioridad_procesos.csv'):
        generar_diagrama_gantt(df, 'Diagrama de Gantt para Planificador por Prioridad')


