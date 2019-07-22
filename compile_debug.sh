module load petsc/3.11-debug
module load slepc/3.11-debug

mpicc minimal_petsc1.cpp -o minimal_petsc1 -I$PETSC_DIR/include -I$PETSC_DIR/$PETSC_ARCH/include -L$PETSC_DIR/$PETSC_ARCH/lib -lpetsc
mpicc minimal_petsc2.cpp -o minimal_petsc2 -I$PETSC_DIR/include -I$PETSC_DIR/$PETSC_ARCH/include -L$PETSC_DIR/$PETSC_ARCH/lib -lpetsc
mpicc minimal_slepc1.cpp -o minimal_slepc1 -I$PETSC_DIR/include -I$PETSC_DIR/$PETSC_ARCH/include -L$PETSC_DIR/$PETSC_ARCH/lib -lpetsc -I$SLEPC_DIR/include -I$SLEPC_DIR/$PETSC_ARCH/include -L$SLEPC_DIR/$PETSC_ARCH/lib -lslepc
mpicc minimal_slepc2.cpp -o minimal_slepc2 -I$PETSC_DIR/include -I$PETSC_DIR/$PETSC_ARCH/include -L$PETSC_DIR/$PETSC_ARCH/lib -lpetsc -I$SLEPC_DIR/include -I$SLEPC_DIR/$PETSC_ARCH/include -L$SLEPC_DIR/$PETSC_ARCH/lib -lslepc
