
output: Main.o fsys.o
		g++ Main.o fsys.o -ljsoncpp -lstdc++fs -o output
main.o: Main.cpp
		g++ -c Main.cpp -ljsoncpp -lstdc++fs
func.o: fsys.cpp fsys.h
		g++ -c fsys.cpp -ljsoncpp -lstdc++fs


clean:
		rm *.o output