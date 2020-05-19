all: expression pong

expression: expression.cpp
	g++ expression.cpp -Wall -Wextra -oexpression

pong: pong.cpp
	g++ pong.cpp -Wall -lrt -lX11 -lGL -o pong

clean:
	rm -f expression

