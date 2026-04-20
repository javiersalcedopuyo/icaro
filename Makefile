EXECUTABLE_NAME=icaro_test

CC = clang++
DEBUGGER = lldb

CFLAGS = -std=c++20\
		 -g\
		 -Werror -Wall -pedantic\
		 -DDEBUG\
		 -fno-exceptions\
		 -fno-rtti\
		 -fsanitize=undefined,address\
		 -MJ tmp.json

CLANG_TIDY_CHECKS = bugprone-*,\
					performance-*,\
					modernize-*,\
					clang-analyzer-*,\
					misc-*,\
					-misc-non-private-member-variables-in-classes,\
					-misc-use-anonymous-namespace

build: test.cpp icaro.hpp
	@echo "Compiling..."
	@time $(CC) $(CFLAGS) test.cpp -o $(EXECUTABLE_NAME)
	@echo '[' > compile_commands.json
	@cat tmp.json >> compile_commands.json
	@echo ']' >> compile_commands.json
	@rm tmp.json

test:
	$(MAKE) build
	./$(EXECUTABLE_NAME)

tidy:
	$(MAKE) build
	clang-tidy test.cpp -p . --checks='$(CLANG_TIDY_CHECKS)'

clean:
	rm -rf $(EXECUTABLE_NAME) compile_commands.json *.dSYM

install:
	cp icaro.hpp /usr/local/include

uninstall:
	rm /usr/local/include/icaro.hpp

debug:
	$(MAKE) build
	$(DEBUGGER) $(EXECUTABLE_NAME)

.PHONY: test tidy clean install uninstall debug
