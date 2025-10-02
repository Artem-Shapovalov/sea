all: sample sample_loader

extractor.o: extractor.hpp extractor.cpp
	$(CXX) -c extractor.cpp -o extractor.o

extractor.hpp:
extractor.cpp:

sample.o: sample.cpp extractor.hpp
	$(CXX) -c sample.cpp -o sample.o

sample.cpp:

sample: sample.o extractor.o
	$(CXX) sample.o extractor.o -o sample -larchive

loader.cpp:
loader.hpp:

loader.o: loader.cpp loader.hpp
	$(CXX) -c loader.cpp -o loader.o

sample_loader.cpp:

sample_loader: sample_loader.o loader.o
	$(CXX) sample_loader.o loader.o -o sample_loader -lcurl

clean:
	rm -rf *.o sample sample_loader
