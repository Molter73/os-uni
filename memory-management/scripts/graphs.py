#!/usr/bin/env python3

import argparse
import os

import matplotlib.pyplot as plt
import pandas as pd


def read_metrics(input):
    df = pd.read_json(input, orient='records', lines=True)
    df.sort_values(by=['method', 'queue', 'accesses'], inplace=True)
    return df


def single_plot(method, metrics, output_dir):
    fig, ax = plt.subplots(nrows=3, ncols=1, layout='constrained')
    i = 0

    for metric in ['page_fault_rate', 'eviction_rate', 'hit_rate']:
        for queue_type in metrics['queue'].unique():
            queue_metrics = metrics[metrics['queue'] == queue_type]
            label = queue_type if i == 0 else ''
            ax[i].plot(queue_metrics['accesses'],
                       queue_metrics[metric], label=label)
            ax[i].set_title(metric)
            ax[i].grid(True)
        i = i+1

    fig.legend(loc='upper left')
    fig.suptitle(method, fontsize='x-large')
    plt.xlabel('Accesos a páginas')
    if output_dir is None:
        plt.show()
    else:
        plt.savefig(os.path.join(output_dir, f'{method}.png'))


def plot_metrics(metrics, output_dir):
    for method, submetrics in metrics.groupby('method'):
        single_plot(method, submetrics, output_dir)


def main(input, output_dir):
    metrics = read_metrics(input)

    # Plot the metrics
    plot_metrics(metrics, output_dir)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str,
                        help='Archivo json lines con métricas')
    parser.add_argument('--output', '-o', type=str, default=None,
                        help='Path al directorio de salida')

    args = parser.parse_args()
    input = args.input
    output_dir = args.output

    main(input, output_dir)
