src = $(wildcard Source/*.c)
obj = $(src:.c=.o)
CFLAGS=`mysql_config --libs`

Build/main: $(obj)
	@echo "Cleaning up..."
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(obj) Build/main