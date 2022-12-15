#make build: makes the full project
#make build_tb: builds the test-benches

build: gate.o parser.o utils.o TOP.o
	${MAKE} link clean

build_tb: gate.o parser.o TB.o utils.o link clean

gate.o: gate/*.cpp gate/PRIMLIB/*.cpp
	g++ -c $^

parser.o: parser/*.cpp parser/wavedrom/*.cpp parser/wavedrom/json/*.cpp
	g++ -c $^

TB.o: TB/*.cpp
	g++ -c $^

TOP.o: TOP/*.cpp
	g++ -c $^

utils.o: utils/*.cpp
	g++ -c $^

link:
	g++ -o wind *.o

clean:
	rm *.o