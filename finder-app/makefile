TARGET = writer

all : $(TARGET)
	$(CROSS_COMPILE)gcc -g -c -Wall -o $(TARGET).o $(TARGET).c
	$(CROSS_COMPILE)gcc -g -I/ writer.o -o writer

clean :
	rm -f *.o *.elf *.map $(TARGET)

