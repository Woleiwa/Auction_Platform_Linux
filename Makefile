

CXXFLAGS = -l inc

SRC = src/main.cpp src/commodity.cpp src/Commodity_List.cpp src/consumer.cpp src/mail.cpp src/Mail_List.cpp src/md5.cpp src/mytime.cpp src/order.cpp src/Order_List.cpp src/seller.cpp src/user.cpp src/User_List.cpp src/admin.cpp

OBJ = $(SRC:.cpp = .o)

INC = inc/commodity.h inc/Commodity_List.h inc/consumer.h inc/mail.h inc/Mail_List.h inc/md5.h inc/mytime.h inc/order.h inc/Order_List.h inc/seller.h inc/user.h inc/User_List.h inc/admin.h


main:$(SRC)
	g++ $(CFLAGS) -c $(SRC) 
	g++ -o main *.o -lpthread
	

main.o:$(INC)


.PHONY:clean
clean:
	rm *.o main 


