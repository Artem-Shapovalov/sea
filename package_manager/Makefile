all: sample sample_loader sample_pkglist sample_pkginfo

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

sample_loader.o: sample_loader.cpp loader.hpp
	$(CXX) -c sample_loader.cpp -o sample_loader.o

sample_loader: sample_loader.o loader.o
	$(CXX) sample_loader.o loader.o -o sample_loader -lcurl

pkglist.cpp:
pkglist.hpp:

pkglist.o: pkglist.cpp pkglist.hpp
	$(CXX) -c pkglist.cpp -o pkglist.o

sample_pkglist.cpp:

sample_pkglist.o: sample_pkglist.cpp pkglist.hpp
	$(CXX) -c sample_pkglist.cpp -o sample_pkglist.o

sample_pkglist: sample_pkglist.o pkglist.o
	$(CXX) sample_pkglist.o pkglist.o -o sample_pkglist

pkginfo.cpp:
pkginfo.hpp:

pkginfo.o: pkginfo.cpp pkginfo.hpp
	$(CXX) -c pkginfo.cpp -o pkginfo.o

sample_pkginfo.cpp:

sample_pkginfo.o: sample_pkginfo.cpp pkginfo.hpp
	$(CXX) -c sample_pkginfo.cpp -o sample_pkginfo.o

sample_pkginfo: sample_pkginfo.o pkginfo.o
	$(CXX) sample_pkginfo.o pkginfo.o -o sample_pkginfo

clean:
	rm -rf *.o sample sample_loader sample_pkglist sample_pkginfo
