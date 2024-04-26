# graphs.py
import matplotlib.pyplot as plt
from collections import defaultdict


def parse_log(file_path):
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


def calculate_metrics(page_faults, page_evictions, page_hits, total_requests):
    page_fault_rate = (page_faults / total_requests) * 100
    eviction_rate = (page_evictions / total_requests) * 100
    hit_rate = (page_hits / total_requests) * 100

    return page_fault_rate, eviction_rate, hit_rate


def plot_metrics(page_fault_rate, eviction_rate, hit_rate):
    labels = ['Page Fault Rate', 'Eviction Rate', 'Hit Rate']
    values = [page_fault_rate, eviction_rate, hit_rate]

    plt.bar(labels, values, color=['red', 'blue', 'green'])
    plt.ylabel('Percentage')
    plt.title('Memory Management Performance Metrics')
    plt.ylim(0, 100)  # Optional: Adjust based on your data range
    plt.show()


# Example Usage
log_path = 'simulation_log.txt'
page_faults, page_evictions, page_hits, total_requests = parse_log(log_path)
page_fault_rate, eviction_rate, hit_rate = calculate_metrics(
    page_faults, page_evictions, page_hits, total_requests)

print("Page Fault Rate:", "{:.2f}%".format(page_fault_rate))
print("Eviction Rate:", "{:.2f}%".format(eviction_rate))
print("Hit Rate:", "{:.2f}%".format(hit_rate))

# Plot the metrics
plot_metrics(page_fault_rate, eviction_rate, hit_rate)
