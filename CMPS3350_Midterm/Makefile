all: mid19 unit_test

mid19: mid19.cpp
	g++ mid19.cpp -Wall -o mid19

unit_test: mid19.cpp
	g++ mid19.cpp -Wall -D UNIT_TEST -o unit_test

clean:
	rm -f mid19

