import argparse
import json
import os

import pandas as pd

from . import ALGORITHMS


# Función para leer y procesar los datos de un algoritmo
def procesar_datos(df):
    # Calcular métricas importantes
    df['Tiempo de Espera'] = df['Inicio de Ejecución'] - \
        df['Tiempo de Llegada']
    df['Tiempo de Retorno'] = df['Fin de Ejecución'] - df['Tiempo de Llegada']
    # Asumiendo igualdad para simplificación
    df['Tiempo de Respuesta'] = df['Tiempo de Espera']


# Función para calcular métricas de rendimiento
def calcular_metricas(df, algorithm, input_type, preemptive=False):
    metrics = {
        'algorithm': algorithm,
        'input_type': input_type,
    }
    # Tiempo de espera promedio: Tiempo que cada proceso pasa en la cola antes
    # de su ejecución. Es una métrica crucial para evaluar la percepción de
    # rapidez de un sistema por parte del usuario.
    metrics['tiempo_espera_promedio'] = df['Tiempo de Espera'].mean()

    # Tiempo de retorno promedio: Tiempo total desde la llegada del proceso
    # hasta su finalización. Importante para entender el tiempo total que un
    # proceso requiere para completarse, incluyendo el tiempo de espera y de
    # ejecución.
    metrics['tiempo_retorno_promedio'] = df['Tiempo de Retorno'].mean()

    # Tiempo de respuesta promedio: Tiempo desde que el proceso llega hasta
    # que se inicia su ejecución. En este caso, se asume igual al tiempo de
    # espera para simplificación, pero en algoritmos con desalojo podría ser
    # diferente.
    if preemptive:
        # Ordenar el dataframe por 'ID Proceso' y luego por 'Inicio de
        # Ejecución'
        df_sorted = df.sort_values(by=['ID Proceso', 'Inicio de Ejecución'])
        # Eliminar duplicados manteniendo la primera aparición para obtener el
        # inicio de ejecución de cada proceso
        df_first_execution = df_sorted.drop_duplicates(
            subset='ID Proceso', keep='first')
        # Calcular el tiempo de respuesta como la diferencia entre 'Inicio de
        # Ejecución' y 'Tiempo de Llegada'
        df_first_execution.loc[:, 'Tiempo de Respuesta'] = \
            df_first_execution['Inicio de Ejecución'] - \
            df_first_execution['Tiempo de Llegada']
        # Calcular el tiempo de respuesta promedio específicamente para
        # algoritmos con prevaciado.
        metrics['tiempo_respuesta_promedio'] = \
            df_first_execution['Tiempo de Respuesta'].mean()
    else:
        metrics['tiempo_respuesta_promedio'] = df['Tiempo de Respuesta'].mean()

    # Variabilidad del tiempo de espera: Mide cuánto varía el tiempo de espera
    # entre diferentes procesos. Una alta variabilidad puede indicar una falta
    # de equidad en el tratamiento de procesos.
    metrics['var_tiempo_espera'] = df['Tiempo de Espera'].std()

    # Máximo tiempo de espera: El mayor tiempo que un proceso ha esperado antes
    # de ejecutarse. Útil para identificar casos extremos y evaluar la equidad
    # del algoritmo.
    metrics['max_tiempo_espera'] = int(df['Tiempo de Espera'].max())

    # Utilización de la CPU: Porcentaje del tiempo total que la CPU estuvo
    # ejecutando procesos. Una alta utilización indica eficiencia, mientras que
    # una baja utilización puede señalar ineficiencias o tiempo ocioso de la
    # CPU.

    # El tiempo total observado es el periodo desde el inicio de la primera
    # ejecución hasta el final de la última ejecución.
    tiempo_total_observado = \
        df['Fin de Ejecución'].max() - df['Tiempo de Llegada'].min()

    if preemptive:
        # Para Round Robin, consideramos cada periodo de ejecución
        # individualmente. Sumamos solo las duraciones efectivas donde la CPU
        # estuvo ejecutando procesos.
        tiempo_total_actividad_cpu = \
            df['Fin de Ejecución'].max() - df['Inicio de Ejecución'].min()
    else:
        # En otros algoritmos, si asumimos una ejecución continua sin grandes
        # periodos de inactividad, podemos simplificar el cálculo a la suma de
        # las duraciones de ráfaga.
        tiempo_total_actividad_cpu = df['Duración de la Ráfaga'].sum()

    # Utilización de la CPU: Porcentaje del tiempo de actividad de la CPU sobre
    # el tiempo total observado.
    metrics['utilizacion_cpu'] = \
        (tiempo_total_actividad_cpu / tiempo_total_observado) * 100

    # Rendimiento: Número de procesos completados por unidad de tiempo.
    # Esta métrica ayuda a entender cuánto trabajo es capaz de realizar el
    # sistema en un tiempo determinado.
    metrics['rendimiento'] = len(df) / tiempo_total_observado

    return metrics


# Función para imprimir métricas de rendimiento
def imprimir_metricas(metrics):
    print("Tiempo de Espera Promedio: " +
          f"{metrics['tiempo_espera_promedio']:.2f} unidades de tiempo")
    print("Tiempo de Retorno Promedio: " +
          f"{metrics['tiempo_retorno_promedio']:.2f} unidades de tiempo")
    print("Tiempo de Respuesta Promedio: " +
          f"{metrics['tiempo_respuesta_promedio']:.2f} unidades de tiempo")
    print("Variabilidad del Tiempo de Espera: " +
          f"{metrics['var_tiempo_espera']:.2f}")
    print("Máximo Tiempo de Espera: " +
          f"{metrics['max_tiempo_espera']} unidades de tiempo")
    print(f"Utilización de la CPU: {metrics['utilizacion_cpu']:.2f}%")
    print("Rendimiento: " +
          f"{metrics['rendimiento']:.2f} procesos/unidad de tiempo")


def main(file, output, algorithm, input_type):
    df = pd.read_csv(file)
    procesar_datos(df)
    preemptive = algorithm == 'rr' or algorithm == 'prioridad_aging'
    metrics = calcular_metricas(df, algorithm, input_type, preemptive)

    if output is None:
        print(f"Métricas para Planificador: {algorithm.upper()}")
        imprimir_metricas(metrics)
    else:
        with open(os.path.join(output, 'metrics.jsonl'), 'a+') as f:
            json.dump(metrics, f)
            f.write("\n")


if __name__ == '__main__':
    # Configuración del analizador de argumentos
    parser = argparse.ArgumentParser(
        description='Genera un diagrama de Gantt a partir de un ' +
                    'archivo CSV de procesos.'
    )
    parser.add_argument(
        'file', type=str,
        help='Ruta al archivo CSV que contiene los datos de los procesos.'
    )
    parser.add_argument(
        'algorithm', type=str, choices=ALGORITHMS,
        help="Algoritmo a ser analizado."
    )
    parser.add_argument(
        'input_type', type=str,
        help='Distribución de entrada de los procesos utilizada'
    )
    parser.add_argument(
        '--output', '-o', type=str, default=None,
        help='Ruta al directorio de salida.'
    )

    # Parsear los argumentos de línea de comandos
    args = parser.parse_args()

    file = args.file
    output = args.output
    algorithm = args.algorithm
    input_type = args.input_type
    main(file, output, algorithm, input_type)
