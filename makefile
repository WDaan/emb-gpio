src = $(wildcard Source/*.c)
obj = $(src:.c=.o)
CFLAGS=`mysql_config --libs`
LIBS = -lgpiod 

Build/main: $(obj)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(obj) Build/main
