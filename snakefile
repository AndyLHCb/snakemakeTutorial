from snakemake.utils import min_version
min_version("6.0")

import os
#if the Logs folder doesn't exist, make it
os.system("if [ ! -d Logs ]; then mkdir Logs; fi")

rule calculateDTR:
    input:
        "{file}.root"
    output:
        "{file}-withDTR.root"
    shell:
        'root -l -q "calculate_DTR.C(\\\"{input}\\\")" > Logs/DTR-{input}.log 2>&1'  
