from snakemake.utils import min_version
min_version("6.0")

import os
#if the Logs folder doesn't exist, make it
os.system("if [ ! -d Logs ]; then mkdir Logs; fi")

module process:
    snakefile: "process.snake"

use rule * from process as p_*

module fit:
    snakefile: "fit.snake"

use rule * from fit as f_*

rule all:
    input:
        "DTRfit.pdf",
        "TrueDTfit.pdf"

ruleorder: p_mergeFiles > p_calculateDTR
