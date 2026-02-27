# Travelling Salesman Problem
Roșu Mihai Cosmin 343C1

<b>Algorithm details</b>:
- The algorithm used to solve the travelling salesman problem is a simple exhaustive search through
all the possible routes and picking the one with the lowest cost. The search is done through
generating all permutations of the cities with the help of backtracking and calculating the cost
for each of them.
- To generate the permutations, I would always place the first unused value and go to the next
position. If the permutation is complete (the position is equal to the number of cities) I can
calculate the cost of this route. When a position used all possible values, I go to the previous
one to pick a new value. The algorithm ends when I get to the first position, which has a single
possible value (the start city).

<b>Parallel implementations</b>:
- To parallelize the algorithm, I split the workload for picking the second city. Essentially, each
thread/process receives a part of the cities to use as the second node of the permutation and then
proceeds to calculate the rest of the permutation sequentially.

<b>Extra specific details</b>:
- Pthreads:
  - To split the workload I used the following formula to determine the start and the end of the
  interval used by a thread:

  start = ID * N / P
  end = min((ID + 1) * N / P, N)

  where ID is the thread id, N is the number of elements and P is the number of threads

  - To store each thread's result I used an array with num_threads elements, where each thread can
  store its result at the position equal to its id. Then, the main thread picks the lowest cost out
  of the result array.

- OpenMP:
  - To split the workload I used the 'parallel for' macro, where the for iterates through values
  for the second city.
  - To gather all results andd pick the best one, I used the 'reduction' clause, with the 'min'
  operation.

- MPI:
  - To split the workload I used the MPI_Scatterv instruction. To calculate the displacements and
  send counts needed by it, I used a simple algorithm which tries split the workload evenly, and
  if there are still values remaining, splits them again by assigning one more value to each
  process.

  - Because the MPI_Scatterv instruction doesn't permit send counts equal to 0 (basically a process
  receiving 0 values), in case there are more processes than cities, I consider as many cities as
  there are processes, and each process checks if the value it received is actually correct.

  - Lastly, to gather each process' result, each process other than process 0 sends their result to
  process 0, which than picks the best value.

- MPI_Pthreads / MPI_OpenMP:
  - For the hybrid versions, at first the workload for the second node is split among the processes
  and then each process splits the second node workload it received further among its threads.

  - Essentially each hybrid version is a combination of the standalone versions of each technology.

<b>How to run</b>:
- To run a specific test the Makefile rules can be used.
  - Template:
  	make run_<technology> [TEST=<testname> THREADS=<number of threads> PROC=<number of processes>]

  	where technology is the name of the code version you want to run (ex: serial, pthreads, etc.),
  	and the next three options are optional and are used to specify which test to run (default is
  	test1), how many threads to use (in case it is relevant; default is 20) and how many processes
  	to use (in case it is relevant; default is 20)
  - Examples:
  	make run_serial TEST=test2
  	make run_openmp THREADS=5 TEST=test5
  	make run_mpi TEST=test7 PROC=10
  	make run_mpi_openmp PROC=4 THREADS=5 TEST=test10

- To run all tests use the run.sh script.
