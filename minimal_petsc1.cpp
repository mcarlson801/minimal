#include <petsc.h>

int main(int argc, char *argv[])
{
    Mat A, F;
    PetscInt N = 1000;
    PetscInt retval;
    PetscInt i;
    PetscInt col[3];
    PetscScalar value[3];

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

    // Factor A using Mumps
    MatGetFactor(A, "mumps", MAT_FACTOR_CHOLESKY, &F);
    MatMumpsSetIcntl(F, 13, 1);
    MatMumpsSetIcntl(F, 14, 120);
    MatCholeskyFactorSymbolic(F, A, 0, 0);
    MatCholeskyFactorNumeric(F, A, 0);
    MatMumpsGetInfog(F, 12, &retval);

    retval = 0;
    PetscPrintf(PETSC_COMM_WORLD, "Number of eigenvalues greater than 0: %d, should be about 1000\n", N - retval);

    PetscFinalize();
}