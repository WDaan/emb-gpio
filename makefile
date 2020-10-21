src = $(wildcard Source/*.c)
obj = $(src:.c=.o)

Build/main: $(obj)
	@echo "Cleaning up..."
	$(CC) -o $@ $^

.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -f $(obj) Build/main