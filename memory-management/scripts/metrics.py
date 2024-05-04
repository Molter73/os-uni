# graphs.py
import argparse
import json
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
        return {
            "page_fault_rate": (self.page_faults / self.total_requests) * 100,
            "eviction_rate": (self.page_evictions / self.total_requests) * 100,
            "hit_rate": (self.page_hits / self.total_requests) * 100,
        }

    def print_metrics(self, output):
        json.dump(self.calculate_metrics(), output)


def main(log_path, output):
    stats = Stats(log_path)
    stats.print_metrics(output)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str, help='Log de simulación de entrada')
    parser.add_argument('--output', '-o', type=str, default=None,
                        help='Path al archivo JSON de salida')

    args = parser.parse_args()
    log_path = args.input
    output = sys.stdout if args.output is None else open(args.output, 'w')

    main(log_path, output)
