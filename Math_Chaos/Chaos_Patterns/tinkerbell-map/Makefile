CXX = g++
CXXFLAGS = -std=c++17 -I /usr/local/include/eigen3/
LIBS = -lGL -lglfw

TinkerbellMap: tinkerbell-map.cpp OrbitCamera.cpp
	$(CXX) $(CXXFLAGS) -o TinkerbellMap tinkerbell-map.cpp OrbitCamera.cpp $(LIBS)
