#include <petsc.h>
#include <slepceps.h>

int main(int argc, char *argv[])
{
    EPS eps;
    Mat K, M;
    PetscInt nconv;
    PetscViewer viewer;

    SlepcInitialize(&argc, &argv, NULL, NULL);

    // Load K,M from file
    const char* K_filename = "../data/K";
    const char* M_filename = "../data/M";

    PetscViewerBinaryOpen(PETSC_COMM_WORLD, K_filename, FILE_MODE_READ, &viewer);
    MatCreate(PETSC_COMM_WORLD, &K);
    MatLoad(K, viewer);
    MatSetOption(K, MAT_HERMITIAN, PETSC_TRUE);
    MatAssemblyBegin(K, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(K, MAT_FINAL_ASSEMBLY);
    PetscViewerDestroy(&viewer);

    PetscViewerBinaryOpen(PETSC_COMM_WORLD, M_filename, FILE_MODE_READ, &viewer);
    MatCreate(PETSC_COMM_WORLD, &M);
    MatLoad(M, viewer);
    MatSetOption(M, MAT_HERMITIAN, PETSC_TRUE);
    MatAssemblyBegin(M, MAT_FINAL_ASSEMBLY);
    MatAssemblyEnd(M, MAT_FINAL_ASSEMBLY);
    PetscViewerDestroy(&viewer);

    // Solve for all eigenvalues in interval [1, 2]
    PetscOptionsInsertString(nullptr, "-st_type sinvert");
    PetscOptionsInsertString(nullptr, "-st_ksp_type preonly");
    PetscOptionsInsertString(nullptr, "-st_pc_type cholesky");
    PetscOptionsInsertString(nullptr, "-st_pc_factor_mat_solver_type mumps");
    PetscOptionsInsertString(nullptr, "-mat_mumps_icntl_13 1");
    PetscOptionsInsertString(nullptr, "-mat_mumps_icntl_14 100");

    EPSCreate(PETSC_COMM_WORLD,&eps);

    EPSSetOperators(eps,K,M);
    EPSSetProblemType(eps,EPS_GHEP);
    EPSSetFromOptions(eps);

    EPSSetWhichEigenpairs(eps, EPS_ALL);
    EPSSetInterval(eps, 0.0, 1000.0);

    EPSSolve(eps);

    EPSGetConverged(eps, &nconv); 

    PetscPrintf(PETSC_COMM_WORLD, "%d eigenvalues found.\n", nconv);

    SlepcFinalize();
}