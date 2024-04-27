# graphs.py
import argparse
import os
import matplotlib.pyplot as plt
import sys


class Stats:
    def __init__(self, file_path):
        (self.page_faults, self.page_evictions, self.page_hits,
            self.total_requests) = Stats._parse_log(file_path)

    def _parse_log(file_path):
        with open(file_path, 'r') as file:
            lines = file.readlines()

        page_faults = 0
        page_evictions = 0
        page_hits = 0
        total_requests = 0

        for line in lines:
            total_requests += 1

            if "Estado inicial: Página" in line:
                if "no válida" in line:
                    page_faults += 1
                else:
                    page_hits += 1
            elif "Fallos de página" in line:
                page_evictions += 1

        return page_faults, page_evictions, page_hits, total_requests

    def calculate_metrics(self):
        page_fault_rate = (self.page_faults / self.total_requests) * 100
        eviction_rate = (self.page_evictions / self.total_requests) * 100
        hit_rate = (self.page_hits / self.total_requests) * 100

        return page_fault_rate, eviction_rate, hit_rate

    def print_metrics(self, output):
        page_fault_rate, eviction_rate, hit_rate = self.calculate_metrics()
        output.write(f"Page Fault Rate: {page_fault_rate:.2f}%\n")
        output.write(f"Eviction Rate: {eviction_rate:.2f}%\n")
        output.write(f"Hit Rate: {hit_rate:.2f}%\n")


def plot_metrics(stats: Stats, output_dir):
    page_fault_rate, eviction_rate, hit_rate = stats.calculate_metrics()
    labels = ['Page Fault Rate', 'Eviction Rate', 'Hit Rate']
    values = [page_fault_rate, eviction_rate, hit_rate]

    plt.bar(labels, values, color=['red', 'blue', 'green'])
    plt.ylabel('Percentage')
    plt.title('Memory Management Performance Metrics')
    plt.ylim(0, 100)  # Optional: Adjust based on your data range
    if output_dir is None:
        plt.show()
    else:
        plt.savefig(os.path.join(output_dir, 'graph.png'))


def main(log_path, output_dir):
    stats = Stats(log_path)
    metrics_output = sys.stdout
    if output_dir is not None:
        metrics_output = open(os.path.join(output_dir, 'metrics.txt'), 'w')
    stats.print_metrics(metrics_output)

    # Plot the metrics
    plot_metrics(stats, output_dir)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str, help='Log de simulación de entrada')
    parser.add_argument('--output', '-o', type=str, default=None,
                        help='Path al directorio de salida')

    args = parser.parse_args()
    log_path = args.input
    output_dir = args.output

    main(log_path, output_dir)
