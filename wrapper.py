#!/usr/bin/env python

# Wrapper to submit a script to Slurm using a custom job script.
#
# __author__: Aidan Wiederhold
# __email__: aidan.richard.wiederhold@cern.ch
#
# Thanks to Blaise Delaney who wrote a Condor script which I've changed into a Slurm script, who also directs *many* thanks to John Smeaton for the template script

#===========================================================================
# run:"snakemake <target> --cluster ./workflows/preselection/scripts/slurm_wrapper.py --latency-wait 3600 --use-conda --jobs 5 --groups preselection=group0 --group-components group0=10"
#===========================================================================

from tempfile import NamedTemporaryFile
from snakemake.utils import read_job_properties
import subprocess
import sys
import os

# Get the jobscript, and the properties for this job
jobscript = sys.argv[1] #slurm submission wrapper
job_props = read_job_properties(jobscript) #extrapolate job submission config for conda from wrapper

print(job_props)

jobid = job_props["jobid"]

# Create a directory for slurm logs, if this doesn't exist
logdir = os.path.abspath("SlurmLogs")
if not os.path.exists(logdir):
    os.makedirs(logdir)
logfile=f"{logdir}/{jobid}.out"

# Open a temporary file for the job submission script
with NamedTemporaryFile("w") as sub_file:
    sub_file.write(f"#!/bin/bash \n")
    sub_file.write(f"#SBATCH --ntasks=1 \n")
    sub_file.write(f"#SBATCH --mem-per-cpu=3997 \n")

    if "fit" in job_props["rule"]:
        sub_file.write(f"#SBATCH --time=1:00:00 \n")

    else:
        sub_file.write(f"#SBATCH --time=2:00:00 \n")

    sub_file.write(f"#SBATCH --cpus-per-task=1 \n")
    sub_file.write(f"#SBATCH --partition=epp \n")

    # Job script submission
    sub_file.write(f'{jobscript} \n')

    # Now submit this slurm script, and delete the temporary file
    sub_file.flush()
    subprocess.call( ["sbatch", sub_file.name, ">", logfile] )
