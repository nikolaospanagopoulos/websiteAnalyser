FLAGS= -Wall -std=c++17
LIB= -lcurl


OBJECTS=./build/Downloader.o ./build/HtmlParser.o ./build/App.o

all: ${OBJECTS}
	g++ ${FLAGS} -o ./bin/main ./main.cpp ${OBJECTS} ${LIB}

./build/Downloader.o: ./Downloader.cpp
	g++ ${FLAGS} ./Downloader.cpp -c -o ./build/Downloader.o

./build/HtmlParser.o: ./HtmlParser.cpp
	g++ ${FLAGS} ./HtmlParser.cpp -c -o ./build/HtmlParser.o

./build/App.o: ./App.cpp
	g++ ${FLAGS} ./App.cpp -c -o ./build/App.o
