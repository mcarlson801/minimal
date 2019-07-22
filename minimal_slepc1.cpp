#include <petsc.h>
#include <slepceps.h>

int main(int argc, char *argv[])
{
    EPS eps;
    Mat A;
    PetscInt N = 1000;
    PetscInt nconv;
    PetscInt i;
    PetscInt col[3];
    PetscScalar value[3];

    SlepcInitialize(&argc, &argv, NULL, NULL);

    // Set up A
    MatCreate(PETSC_COMM_WORLD, &A);
    MatSetType(A, "mpiaij");
    MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, N, N);
    MatSetOption(A, MAT_SYMMETRIC, PETSC_TRUE);
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

    // Solve for all eigenvalues in interval [1, 2]
    PetscOptionsInsertString(nullptr, "-st_type sinvert");
    PetscOptionsInsertString(nullptr, "-st_ksp_type preonly");
    PetscOptionsInsertString(nullptr, "-st_pc_type cholesky");
    PetscOptionsInsertString(nullptr, "-st_pc_factor_mat_solver_type mumps");
    PetscOptionsInsertString(nullptr, "-mat_mumps_icntl_13 1");
    PetscOptionsInsertString(nullptr, "-mat_mumps_icntl_14 100");

    EPSCreate(PETSC_COMM_WORLD,&eps);

    EPSSetOperators(eps,A,NULL);
    EPSSetProblemType(eps,EPS_HEP);
    EPSSetFromOptions(eps);

    EPSSetWhichEigenpairs(eps, EPS_ALL);
    EPSSetInterval(eps, 1.0, 2.0);

    EPSSolve(eps);

    EPSGetConverged(eps, &nconv); 

    PetscPrintf(PETSC_COMM_WORLD, "%d eigenvalues found. Should be about 167.\n", nconv);

    SlepcFinalize();
}