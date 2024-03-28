#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

PLANIFICADOR=$1
INPUT_DIR=$2
OUTPUT_DIR=$3

ALGORITMOS=(
    "fcfs"
    "sjf"
    "prioridad"
    "rr"
)

while IFS="" read -r -d $'\0'; do
    input="$REPLY"
    input_type="${input%.bin}"
    input_type="${input_type#"${INPUT_DIR}/"}"

    output_dir="${OUTPUT_DIR}/${input_type}"
    mkdir -p "${output_dir}"

    for algoritmo in "${ALGORITMOS[@]}"; do
        echo "ejecutando planificador con algoritmo ${algoritmo}"
        "${PLANIFICADOR}" -o "${output_dir}" "${algoritmo}" "${input}"

        mkdir -p "${output_dir}/${algoritmo}"

        echo "Generando métricas y diagrama de Gantt"
        python3 "${SCRIPT_DIR}/Gantt.py" \
            "${output_dir}/${algoritmo}_procesos.csv" \
            "${algoritmo}" \
            -o "${output_dir}/${algoritmo}"

        if [[ "$algoritmo" == "prioridad" ]]; then
            echo "ejecutando planificador con algoritmo ${algoritmo} con prevaciado"
            "${PLANIFICADOR}" -po "${output_dir}" "${algoritmo}" "${input}"

            mkdir -p "${output_dir}/${algoritmo}_preemptive"

            echo "Generando métricas y diagrama de Gantt"
            python3 "${SCRIPT_DIR}/Gantt.py" \
                "${output_dir}/${algoritmo}_preemptive_procesos.csv" \
                "${algoritmo}" \
                -o "${output_dir}/${algoritmo}_preemptive"
        fi
    done
done < <(find "$INPUT_DIR" -name 'procesos_*.bin' -type f -print0)
