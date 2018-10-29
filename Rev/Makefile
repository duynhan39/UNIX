dummy: reverse upper

reverse: reverse.cpp shm_com.h
	g++ -o reverse reverse.cpp -pthread -lrt

upper: upper.cpp shm_com.h
	g++ -o upper upper.cpp -pthread -lrt

clean:
	rm -f reverse upper

