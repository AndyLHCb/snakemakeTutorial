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

rule mergeFiles:
    input:
        file0="{fileA}0{fileB}.root",
        file1="{fileA}1{fileB}.root"
    output:
        "{fileA}Combined{fileB}.root"
    shell:
        'root -l -q "mergeSamples.C(\\\"{input.file0}\\\",\\\"{input.file1}\\\")" > Logs/Merge-{output}.log 2>&1'

#ruleorder: mergeFiles > calculateDTR
