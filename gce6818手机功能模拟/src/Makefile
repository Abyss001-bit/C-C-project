CC=arm-linux-gcc
all:
	$(CC) *.c -o main -I../inc -L../lib -ljpeg -lmyfont -lapi_v4l2_arm -pthread
clean:
	rm main

#要添加字库的话，后面在继续加 -lfont -lm