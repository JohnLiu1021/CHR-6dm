CHR_6dm_example : CHR_6dm_example.cpp CHR_6dm.cpp CHR_SensorData.cpp SerialPort_cpp.cpp
	$(CXX) -std=c++11 $^ -lpthread -o $@

CHR_6dm_example_blocking : CHR_6dm_example_blocking.cpp CHR_6dm.cpp CHR_SensorData.cpp SerialPort_cpp.cpp
	$(CXX) -std=c++11 $^ -lpthread -o $@
