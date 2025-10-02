all: sample

extractor.o: extractor.hpp extractor.cpp
	$(CXX) -c extractor.cpp -o extractor.o

extractor.hpp:
extractor.cpp:

sample.o: sample.cpp extractor.hpp
	$(CXX) -c sample.cpp -o sample.o

sample.cpp:

sample: sample.o extractor.o
	$(CXX) sample.o extractor.o -o sample -larchive

clean:
	rm -rf *.o sample
