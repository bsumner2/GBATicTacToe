flags= -std=c++17 -Wall -I .
target: TikTakToe.cc
	g++ $(flags) TikTakToe.cc
	./a.out
