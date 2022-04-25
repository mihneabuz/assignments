CXXFLAGS = -Wall -Wextra
CXX = g++

build: crypto.cpp stocks.cpp valley.cpp ridge.cpp
	$(CXX) $(CXXFLAGS) crypto.cpp -o crypto
	$(CXX) $(CXXFLAGS) stocks.cpp -o stocks
	$(CXX) $(CXXFLAGS) valley.cpp -o valley
	$(CXX) $(CXXFLAGS) ridge.cpp -o ridge

run-p1:
	./crypto

run-p2:
	./stocks

run-p3:
	./valley

run-p4:
	./ridge

clean:
	rm crypto stocks valley ridge
