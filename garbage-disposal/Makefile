src = src/main.c src/binary_tree.c
bin = garbage-disposal

CFLAGS = -Iincl

$(bin): $(src)
	gcc $(CFLAGS) $^ -o$@

clean:
	rm -f $(bin)

