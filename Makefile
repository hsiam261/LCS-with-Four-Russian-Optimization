four-russian-optimization: four-russian-optimization.cpp
	g++ -std=c++14 -o four-russian-optimization.out four-russian-optimization.cpp -O3

run-randomized-test: randomized-gen.cpp lcs_checker.cpp 
	g++ -std=c++14 -o randomized-gen.out randomized-gen.cpp
	g++ -std=c++14 -o lcs_checker.out lcs_checker.cpp
	./test.sh

clean:
	rm -r tests/
	rm -r sols/
	rm *.out

	


