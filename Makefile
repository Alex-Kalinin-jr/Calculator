.PHONY: all install uninstall style leaks dvi test gcov_report clean

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g
GCOV= --coverage
PROJECT = new_calc
INST_DIR = App
TEST_SOURCE = $(wildcard test_*.cc)
SOURCE = s21_model.cc
HEADER = s21_model.h
ROAD=.
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	OPEN = open
	ADD_LIBS = -lgtest
	LEAK_CHECK = leaks
	LEAK_ARGS = -atExit --
	QMAKEDIR=/usr/local/bin/qmake
	INST=new_calc.app
endif
ifeq ($(OS), Linux)
	OPEN = xdg-open
	ADD_LIBS = -lgtest -lrt -lm -lpthread -lsubunit
	LEAK_CHECK = valgrind
	LEAK_ARGS = --tool=memcheck --leak-check=full --track-origins=yes
	QMAKEDIR=/usr/lib/qt5/bin/qmake
	INST=new_calc
endif

all: prepare
	make -f qMakefile

prepare:
	qmake -o qMakefile

install: all
	@test -d "$(INST_DIR)" || mkdir -p "$(INST_DIR)"
	rm -rf $(INST_DIR)/new_calc.app $(INST_DIR)/new_calc 2>/dev/null
	$(QMAKEDIR) -install qinstall $(INST) $(INST_DIR)/new_calc.app
	make clean

test: clean $(SOURCE) $(HEADER)
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCE) $(SOURCE) $(ADD_LIBS)
	./$@
	rm -f $@

gcov_report:
	$(CXX) $(GCOV) $(CXXFLAGS) $(TEST_SOURCE) $(SOURCE) -L . $(ADD_LIBS) -o $(ROAD)/$@
	$(ROAD)/$@
	lcov -t "$@" -o $@.info --include */src/*.cc -c -d .
	genhtml -o report $@.info
	gcov $@.info -f
	rm -rf *.gcda *.gcdo *.gcov *.gcno *.info *dSYM $@
	$(OPEN) report/index.html

dvi:
	$(OPEN) readme.md

dist:
	tar -czpf $(PROJECT).tar.gz *.cpp *.h *.ui Makefile $(PROJECT).pro $(PROJECT).pro.user
	mkdir Dist
	cp $(PROJECT).tar.gz Dist

style:
	clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -n *.cc *.h *.cpp

clean:
	@-rm *.gcov *.gcda *.gcno *.info *.xml *.gz *.o moc_* ui_* *.out 2>/dev/null
	@-rm -rf lcov_report report 2>/dev/null
	@-rm -rf $(INST) 2>/dev/null
	@-rm qMakefile 2>/dev/null
	@-rm -rf .qmake.stash 2>/dev/null

leaks:
	$(CXX) $(CXXFLAGS) -o $@ $(TEST_SOURCE) $(SOURCE) $(ADD_LIBS)
	$(LEAK_CHECK) $(LEAK_ARGS) ./$@
	rm -f $@
