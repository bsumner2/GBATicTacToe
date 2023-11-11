flags= -std=c++17 -Wall -I .
target: ./src/TikTakToe.cc
	g++ $(flags) ./src/TikTakToe.cc -o ./bin/TikTakToe.elf
	./bin/TikTakToe.elf
