CC=gcc
CFLAG=-c 
CFLAGS=-Wall -Werror -Wextra -std=c11
# SRC_DIR=backend
PRO=3dviewer.pro
APP=3dviewer.app
INSTALL_DIR=~/Desktop
SRC_DIR=./3dviewer
BACK_SRC=./3dviewer/backend/*.c
TEST=./tests/unit_tests.c
OS := $(shell uname)

ifeq ($(OS),Linux)
TEST_LIBS = -lcheck -lm -lpthread -lrt -lsubunit -fprofile-arcs -ftest-coverage
else
TEST_LIBS = -lcheck -lm -lpthread -fprofile-arcs -ftest-coverage
endif

all: clean install open

install: clean
	@cd $(SRC_DIR) && qmake $(PRO) && make && make clean && rm Makefile *qrc && rm -rf $(INSTALL_DIR)/$(APP) && mv $(APP) $(INSTALL_DIR)


uninstall:
	rm -rf $(INSTALL_DIR)/$(APP)

open:
	@open -n $(INSTALL_DIR)/$(APP) --args -AppCommandLineArg

dvi:
	@open README.pdf

test: clean
	$(CC) $(CFLAGS) $(TEST) $(BACK_SRC) $(TEST_LIBS) -o unit_test
	./unit_test

gcov_report:
	$(CC) $(CFLAGS) --coverage $(TEST) $(BACK_SRC) $(TEST_LIBS) -o gcov_test
	chmod +x *
	./gcov_test
	lcov -t "gcov_test" -o gcov_test.info --no-external -c -d .
	genhtml -o report/ gcov_test.info
	open ./report/index.html

dist:
	rm -rf Archive_viewer/
	mkdir Archive_viewer/
	cp -R $(INSTALL_DIR)/$(APP) Archive_viewer
	tar -cvzf Archive_viewer.tar.gz Archive_viewer

format:
	@cp ../materials/linters/.clang-format ./
	clang-format -i $(SRC_DIR)/backend/*.c $(SRC_DIR)/backend/*.h 
	@rm .clang-format

check: 
	@cp ../materials/linters/.clang-format ./
	clang-format -n $(SRC_DIR)/backend/*.c $(SRC_DIR)/backend/*.h 
	@rm .clang-format

clean:
	@rm -rf *.o
	@rm -rf *.gcda *.gcno *.info
	@rm -rf gcov_report
	@rm -rf gcov_test
	@rm -rf report
	@rm -rf unit_test
	@rm -rf build/
	@rm -rf front/calc.app
	@rm -rf front/calc_autogen
	@rm -rf front/CMakeFiles
	@rm -rf front/CMakeCache.txt
	@rm -rf front/cmake_install.cmake
	@rm -rf Archive_calc
	@rm -rf dist-newstyle/


.PHONY: all install uninstall open dvi test gcov_report dist format check clean
