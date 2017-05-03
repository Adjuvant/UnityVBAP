g++ -Wall -I/user/local/include -c VBAP.cpp test_vbap.cpp;
g++ -L/usr/local/lib test_vbap.o VBAP.o -lgsl -lgslcblas -lm -o test_vbap
