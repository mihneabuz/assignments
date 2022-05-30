all: gpu_sol

gpu_sol: helper.o
	nvcc -g -rdc=true main.cu helper.o -o gpu_sol -O3 --std=c++11

helper.o:
	nvcc -g helper.cpp -c -O3 --std=c++11

clean:
	rm -f gpu_sol *.o
