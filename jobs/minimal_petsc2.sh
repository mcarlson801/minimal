#!/bin/csh
#SBATCH --time=02:00:00
#SBATCH --nodes 1
#SBATCH --output="../logs/petsc2.log"
#SBATCH --ntasks-per-node 5
#SBATCH --partition=development

mpirun -np $SLURM_NTASKS ../minimal_petsc2 -v -info -log_trace
