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
    "prioridad_preemptive"
    "rr"
)

while IFS="" read -r -d $'\0'; do
    input="$REPLY"
    input_type="${input%.bin}"
    input_type="${input_type#"${INPUT_DIR}/procesos_"}"

    mkdir -p "${OUTPUT_DIR}/${input_type}"
    output_dir="$(realpath "${OUTPUT_DIR}")"

    for algoritmo in "${ALGORITMOS[@]}"; do
        extra_args=()
        if [[ "$algoritmo" == "prioridad_preemptive" ]]; then
            extra_args+=("-p")
        fi

        echo "ejecutando planificador con algoritmo ${algoritmo}"
        "${PLANIFICADOR}" \
            -o "${output_dir}/${input_type}" \
            "${extra_args[@]}" \
            "${algoritmo%'_preemptive'}" \
            "${input}"

        mkdir -p "${output_dir}/${input_type}/${algoritmo}"

        echo "Generando métricas y diagrama de Gantt"
        (
            cd "$SCRIPT_DIR"
            python3 -m "analysis.run" \
                "${output_dir}/${input_type}/${algoritmo}_procesos.csv" \
                "${algoritmo}" \
                "${input_type}" \
                -o "${output_dir}"
        )
    done
done < <(find "$INPUT_DIR" -name 'procesos_*.bin' -type f -print0)
