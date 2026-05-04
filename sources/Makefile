library = -lws2_32 #윈도우 소켓 라이브러리 링크

src = $(wildcard *.c)
exe = $(patsubst %.c, %.exe, $(src)) 

all: $(exe) 

%.exe: %.c
	gcc -Wall $< -o $@ $(library)

clean:
	del *.exe