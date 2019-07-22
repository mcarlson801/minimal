#include <petsc.h>

int main(int argc, char *argv[])
{
    KSP ksp;
    PC pc;
    Mat A, F;
    Vec x, b;
    PetscInt N = 1000;
    PetscInt retval;
    PetscInt i;
    PetscInt col[3];
    PetscScalar value[3];
    PetscReal norm;

    PetscInitialize(NULL, NULL, NULL, NULL);

    // Set up A
    MatCreate(PETSC_COMM_WORLD, &A);
    MatSetType(A, "mpiaij");
    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, N, N);
    MatSetUp(A);

    i = 0; col[0] = 0; col[1] = 1; value[0] = 2.0; value[1] = -1.0;
    MatSetValues(A, 1, &i, 2, &col[0], &value[0], INSERT_VALUES);
    i = N-1; col[0] = N-2; col[1] = N-1; value[0] = -1.0; value[1] = 2.0;
    MatSetValues(A, 1, &i, 2, &col[0], &value[0], INSERT_VALUES);
    for (i = 1; i < N-1; i++)
    {
        col[0] = i-1; col[1] = i; col[2] = i+1;
        value[0] = -1.0; value[1] = 2.0; value[2] = -1.0;
        MatSetValues(A, 1, &i, 3, &col[0], &value[0], INSERT_VALUES);
    }

    MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);

    // Factor A using Mumps as linear system solver
    KSPCreate(PETSC_COMM_WORLD, &ksp);
    KSPSetOperators(ksp, A, A);
    KSPSetType(ksp, KSPPREONLY);
    KSPGetPC(ksp, &pc);
    PCSetType(pc, PCCHOLESKY);
    PCFactorSetMatSolverType(pc, MATSOLVERMUMPS);
    PCFactorSetUpMatSolverType(pc);
    PCFactorGetMatrix(pc, &F);
    MatMumpsSetIcntl(F, 13, 1);
    MatMumpsSetIcntl(F, 14, 80);

    // Solve system
    VecCreateMPI(PETSC_COMM_WORLD, PETSC_DECIDE, N, &x);
    VecCreateMPI(PETSC_COMM_WORLD, PETSC_DECIDE, N, &b);
    VecSet(b, 1.0);
    KSPSolve(ksp, b, x);
    VecNorm(x, NORM_2, &norm);

    PetscPrintf(PETSC_COMM_WORLD, "||x|| = %lf, should be about 2893973.637\n", norm);

    PetscFinalize();
}