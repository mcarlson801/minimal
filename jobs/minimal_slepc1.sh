#!/bin/csh
#SBATCH --time=02:00:00
#SBATCH --nodes 1
#SBATCH --output="../logs/slepc1.log"
#SBATCH --ntasks-per-node 5
#SBATCH --partition=development

mpirun -np $SLURM_NTASKS ../minimal_slepc1 -v -info -log_trace
