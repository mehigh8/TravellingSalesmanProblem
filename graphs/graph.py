from matplotlib import pyplot as plt
threads=[4, 8, 16, 20]

def time_graph(thread, with_serial=True):
	xs=range(1,13)
	ys=[]
	if with_serial:
		for x in xs:
			file=open("../tests/out/serial/test" + str(x) + ".out")
			ys.append(float(file.read().split()[2]))
			file.close()
		plt.plot(xs, ys, marker="o", label="serial")

		serial_y_max=ys[11]

	implementations=["pthreads", "openmp", "mpi", "mpi_pthreads", "mpi_openmp"]
	parallel_y_max=0
	for implementation in implementations:
		ys=[]
		for x in xs:
			file=open("../tests/out/" + implementation + "/test" + str(x) + ".out." + str(thread) + ".average")
			ys.append(float(file.read().split()[4]))
			file.close()
		plt.plot(xs, ys, marker="o", label=implementation)
		parallel_y_max=max(parallel_y_max, ys[11])

	plt.xlabel('Test Number')
	plt.ylabel('Time (seconds)')
	if with_serial:
		plt.title(f'Time plot for Serial vs Parallel implementations with {thread} threads/processes')
	else:
		plt.title(f'Time plot for Parallel implementations with {thread} threads/processes')

	plt.xticks(xs)
	if with_serial:
		plt.yticks(range(0, int(serial_y_max) + 1, 100))
	else:
		plt.yticks(range(0, int(parallel_y_max) + 1, 25))
	plt.legend()

def speedup_graph(thread):
	xs=range(1,13)
	implementations=["pthreads", "openmp", "mpi", "mpi_pthreads", "mpi_openmp"]
	for implementation in implementations:
		ys=[]
		for x in xs:
			serial_file=open("../tests/out/serial/test" + str(x) + ".out")
			serial_time=float(serial_file.read().split()[2])
			parallel_file=open("../tests/out/" + implementation + "/test" + str(x) + ".out." + str(thread) + ".average")
			parallel_time=float(parallel_file.read().split()[4])
			ys.append(serial_time / parallel_time)
			serial_file.close()
			parallel_file.close()
		plt.plot(xs, ys, marker="o", label=implementation)

	plt.xlabel('Test Number')
	plt.ylabel('Speedup')
	plt.title(f'Speedup plot for Parallel implementations with {thread} threads/processes')

	plt.xticks(xs)
	plt.legend()

def speedup_specific_test(test, implementation):
	serial_file=open("../tests/out/serial/test" + str(test) + ".out")
	serial_time=float(serial_file.read().split()[2])
	serial_file.close()

	ys=[]
	for thread in threads:
		parallel_file=open("../tests/out/" + implementation + "/test" + str(test) + ".out." + str(thread) + ".average")
		parallel_time=float(parallel_file.read().split()[4])
		ys.append(serial_time / parallel_time)
		parallel_file.close()
	plt.plot(threads, ys, marker="o", label="Speedup")

	plt.xlabel('Number of processes')
	plt.ylabel('Speedup')
	plt.title(f'Speedup for {implementation} test {test}')

	plt.xticks(threads)
	plt.legend()


if __name__ == "__main__":
	# Generate time graphs with serial
	for thread in threads:
		plt.figure(figsize=(20, 12))
		time_graph(thread)

		plt.savefig("serial_time_" + str(thread) + ".png")
		plt.clf()
	# Generate time graphs without serial
	for thread in threads:
		plt.figure(figsize=(20, 12))
		time_graph(thread, with_serial=False)

		plt.savefig("parallel_time_" + str(thread) + ".png")
		plt.clf()

	# Generate speedup graphs
	for thread in threads:
		plt.figure(figsize=(20, 12))
		speedup_graph(thread)

		plt.savefig("speedup_" + str(thread) + ".png")
		plt.clf()

	# Generate speedup graph for a specific test and technology
	for test in range(10, 13):
		plt.figure(figsize=(20, 12))
		speedup_specific_test(test, "mpi")

		plt.savefig("test_" + str(test) + "_mpi_speedup")
		plt.clf()