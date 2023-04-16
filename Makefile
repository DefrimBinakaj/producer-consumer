output: main.o commands.o producer.o consumer.o tands.o
	g++ -pthread -o prodcon main.o commands.o producer.o consumer.o tands.o -lpthread

main.o: main.cpp
	g++ -c main.cpp

commands.o: commands.cpp commands.h
	g++ -c commands.cpp

producer.o: producer.cpp common.h
	g++ -c producer.cpp

consumer.o: consumer.cpp common.h
	g++ -c consumer.cpp

tands.o: tands.cpp tands.h
	g++ -c tands.cpp

clean:
	rm *.o *.log prodcon