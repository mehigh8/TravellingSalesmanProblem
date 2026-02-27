#!/bin/bash
# Rosu Mihai Cosmin 343C1
test_no=12
THREADS=12
HYB_THR=2
HYB_PROC=6

make build

echo "---------- SERIAL ---------"
for i in $(seq 1 1 $test_no)
do
	./serial_tsp < "tests/in/test$i.in" > "tests/out/serial/test$i.out"
	echo "Test $i completed"
	cat tests/out/serial/test$i.out | tail -n 1
done

echo "---------- PTHREADS ---------"
for i in $(seq 1 1 $test_no)
do
	total_time=0
	for run in $(seq 1 1 3)
	do
		start_time=$(date +%s.%N)
		output=$(./pthreads_tsp ${THREADS} < "tests/in/test$i.in" | tail -n 1)
		end_time=$(date +%s.%N)

		elapsed_time=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $elapsed_time" | bc -l)

        echo "Total time for 'test$i' - Run $run: $elapsed_time seconds"
	done

	average_time=$(echo "$total_time / 3" | bc -l)
	echo "Average time for 'test$i': $average_time seconds"
    echo "Average time for 'test$i': $average_time seconds" > "tests/out/pthreads/test${i}.out.${THREADS}.average"
done

echo "---------- OPENMP ---------"
for i in $(seq 1 1 $test_no)
do
	total_time=0
	for run in $(seq 1 1 3)
	do
		start_time=$(date +%s.%N)
		output=$(./openmp_tsp ${THREADS} < "tests/in/test$i.in" | tail -n 1)
		end_time=$(date +%s.%N)

		elapsed_time=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $elapsed_time" | bc -l)

        echo "Total time for 'test$i' - Run $run: $elapsed_time seconds"
	done

	average_time=$(echo "$total_time / 3" | bc -l)
	echo "Average time for 'test$i': $average_time seconds"
    echo "Average time for 'test$i': $average_time seconds" > "tests/out/openmp/test${i}.out.${THREADS}.average"
done

echo "---------- MPI ---------"
for i in $(seq 1 1 $test_no)
do
	total_time=0
	for run in $(seq 1 1 3)
	do
		start_time=$(date +%s.%N)
		output=$(mpirun -np ${THREADS} -oversubscribe mpi_tsp < "tests/in/test$i.in" | tail -n 1)
		end_time=$(date +%s.%N)

		elapsed_time=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $elapsed_time" | bc -l)

        echo "Total time for 'test$i' - Run $run: $elapsed_time seconds"
	done

	average_time=$(echo "$total_time / 3" | bc -l)
	echo "Average time for 'test$i': $average_time seconds"
    echo "Average time for 'test$i': $average_time seconds" > "tests/out/mpi/test${i}.out.${THREADS}.average"
done

echo "---------- MPI-OPENMP ---------"
for i in $(seq 1 1 $test_no)
do
	total_time=0
	for run in $(seq 1 1 3)
	do
		start_time=$(date +%s.%N)
		output=$(mpirun -np ${HYB_PROC} mpi_openmp_tsp ${HYB_THR} < "tests/in/test$i.in" | tail -n 1)
		end_time=$(date +%s.%N)

		elapsed_time=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $elapsed_time" | bc -l)

        echo "Total time for 'test$i' - Run $run: $elapsed_time seconds"
	done

	average_time=$(echo "$total_time / 3" | bc -l)
	echo "Average time for 'test$i': $average_time seconds"
    echo "Average time for 'test$i': $average_time seconds" > "tests/out/mpi_openmp/test${i}.out.${THREADS}.average"
done

echo "---------- MPI-PTHREADS ---------"
for i in $(seq 1 1 $test_no)
do
	total_time=0
	for run in $(seq 1 1 3)
	do
		start_time=$(date +%s.%N)
		output=$(mpirun -np ${HYB_PROC} mpi_pthreads_tsp ${HYB_THR} < "tests/in/test$i.in" | tail -n 1)
		end_time=$(date +%s.%N)

		elapsed_time=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $elapsed_time" | bc -l)

        echo "Total time for 'test$i' - Run $run: $elapsed_time seconds"
	done

	average_time=$(echo "$total_time / 3" | bc -l)
	echo "Average time for 'test$i': $average_time seconds"
    echo "Average time for 'test$i': $average_time seconds" > "tests/out/mpi_pthreads/test${i}.out.${THREADS}.average"
done

make clean
