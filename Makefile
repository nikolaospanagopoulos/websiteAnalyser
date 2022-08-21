FLAGS= -Wall -std=c++17
LIB= -lcurl -lmariadbcpp




OBJECTS=./build/Downloader.o ./build/HtmlParser.o ./build/helpers.o ./build/JsonDownloader.o ./build/Database.o ./build/analyzeWebsite.o ./build/JsonResponse.o

all: ${OBJECTS}
	g++ ${FLAGS} -o ./bin/main ./main.cpp ${OBJECTS} ${LIB}

./build/Downloader.o: ./Downloader.cpp
	g++ ${FLAGS} ./Downloader.cpp -c -o ./build/Downloader.o

./build/HtmlParser.o: ./HtmlParser.cpp
	g++ ${FLAGS} ./HtmlParser.cpp -c -o ./build/HtmlParser.o

./build/helpers.o: ./helpers.cpp
	g++ ${FLAGS} ./helpers.cpp -c -o ./build/helpers.o
./build/analyzeWebsite.o: ./analyzeWebsite.cpp
	g++ ${FLAGS} ./analyzeWebsite.cpp -c -o ./build/analyzeWebsite.o

./build/JsonDownloader.o: ./JsonDownloader.cpp
	g++ ${FLAGS}  ./JsonDownloader.cpp -c -o ./build/JsonDownloader.o

./build/Database.o: ./Database.cpp
	g++ ${FLAGS}  ./Database.cpp -c -o ./build/Database.o 

./build/JsonResponse.o: ./JsonResponse.cpp
	g++ ${FLAGS}  ./JsonResponse.cpp -c -o ./build/JsonResponse.o 
