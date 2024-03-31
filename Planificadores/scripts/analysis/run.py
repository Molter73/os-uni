import argparse
import os

from . import ALGORITHMS
from .gantt import main as draw_gantt
from .metrics import main as metrics


def main(file, output, algorithm, input_type):
    gantt_output = output if output is None else os.path.join(
        output, input_type, algorithm)
    draw_gantt(file, gantt_output, algorithm, input_type)
    metrics(file, output, algorithm, input_type)


if __name__ == '__main__':
    # Configuración del analizador de argumentos
    parser = argparse.ArgumentParser(
        description='Realizar análisis de datos generados a partir del ' +
                    'ejecutable "planificador"'
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
