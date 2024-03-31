import matplotlib.pyplot as plt
import pandas as pd
import argparse
import os

from . import ALGORITHMS


# Función para generar el diagrama de Gantt
def generar_diagrama_gantt(df, titulo, output):
    fig, ax = plt.subplots(figsize=(10, 6))
    for index, row in df.iterrows():
        inicio = row['Inicio de Ejecución']
        fin = row['Fin de Ejecución']
        duracion = fin - inicio
        ax.barh(row['ID Proceso'], duracion, left=inicio,
                color='skyblue', edgecolor='black')

    ax.set_xlabel('Tiempo')
    ax.set_ylabel('ID Proceso')
    ax.set_title(titulo)
    ax.invert_yaxis()  # Invertir el eje Y
    plt.grid(axis='x')

    if output is None:
        plt.show()
    else:
        plt.savefig(os.path.join(output, 'gantt.png'))


def main(file, output, algorithm, input_type):
    df = pd.read_csv(file)
    generar_diagrama_gantt(
        df,
        f'Diagrama de Gantt para Planificador {algorithm.upper()} ' +
        f'/ Entrada {input_type.upper()}',
        output
    )


# Main script para procesar los datos de Round Robin como ejemplo
if __name__ == "__main__":
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
        '--output', '-o', type=str, default=None,
        help='Ruta al directorio de salida.'
    )

    # Parsear los argumentos de línea de comandos
    args = parser.parse_args()

    file = args.file
    output = args.output
    algorithm = args.algorithm
    main(file, output, algorithm)
