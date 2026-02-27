# Rosu Mihai Cosmin 343C1
CC=gcc
MPICC=mpicc
CFLAGS=-Wall -Wextra -Werror -pedantic -g

SERIAL=serial_tsp
SERIAL_FILES=$(wildcard ./serial/*.c)
PTHREADS=pthreads_tsp
PTHREADS_FILES=$(wildcard ./pthreads/*.c)
OPENMP=openmp_tsp
OPENMP_FILES=$(wildcard ./openmp/*.c)
MPI=mpi_tsp
MPI_FILES=$(wildcard ./mpi/*.c)
MPI_OPENMP=mpi_openmp_tsp
MPI_OPENMP_FILES=$(wildcard ./mpi_openmp/*.c)
MPI_PTHREADS=mpi_pthreads_tsp
MPI_PTHREADS_FILES=$(wildcard ./mpi_pthreads/*.c)

TEST=test1
THREADS=20
PROC=20

build: $(SERIAL) $(PTHREADS) $(OPENMP) $(MPI) $(MPI_OPENMP) $(MPI_PTHREADS)

serial_tsp: $(SERIAL_FILES)
	$(CC) $(CFLAGS) $^ -o $(SERIAL)

pthreads_tsp: $(PTHREADS_FILES)
	$(CC) $(CFLAGS) $^ -o $(PTHREADS) -lpthread

openmp_tsp: $(OPENMP_FILES)
	$(CC) $(CFLAGS) $^ -o $(OPENMP) -fopenmp

mpi_tsp: $(MPI_FILES)
	$(MPICC) $(CFLAGS) $^ -o $(MPI)

mpi_openmp_tsp: $(MPI_OPENMP_FILES)
	$(MPICC) $(CFLAGS) $^ -o $(MPI_OPENMP) -fopenmp

mpi_pthreads_tsp: $(MPI_PTHREADS_FILES)
	$(MPICC) $(CFLAGS) $^ -o $(MPI_PTHREADS) -lpthread

run_serial: $(SERIAL)
	./$(SERIAL) < tests/in/$(TEST).in > tests/out/serial/$(TEST).out

run_pthreads: $(PTHREADS)
	./$(PTHREADS) $(THREADS) < tests/in/$(TEST).in > tests/out/pthreads/$(TEST).out

run_openmp: $(OPENMP)
	./$(OPENMP) $(THREADS) < tests/in/$(TEST).in > tests/out/openmp/$(TEST).out

run_mpi: $(MPI)
	mpirun -np $(PROC) -oversubscribe $(MPI) < tests/in/$(TEST).in > tests/out/mpi/$(TEST).out

run_mpi_openmp: $(MPI_OPENMP)
	mpirun -np $(PROC) $(MPI_OPENMP) $(THREADS) < tests/in/$(TEST).in > tests/out/mpi_openmp/$(TEST).out

run_mpi_pthreads: $(MPI_PTHREADS)
	mpirun -np $(PROC) $(MPI_PTHREADS) $(THREADS) < tests/in/$(TEST).in > tests/out/mpi_pthreads/$(TEST).out

clean:
	rm $(SERIAL)
	rm $(PTHREADS)
	rm $(OPENMP)
	rm $(MPI)
	rm $(MPI_OPENMP)
	rm $(MPI_PTHREADS)