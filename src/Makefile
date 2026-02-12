CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic
LDFLAGS = -lcheck -lm -lpthread
CFLAGS_CHECK += $(CFLAGS)


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CHECK_PREFIX := $(shell brew --prefix check 2>/dev/null)
	ifneq ($(CHECK_PREFIX),)
		CFLAGS_CHECK += -I$(CHECK_PREFIX)/include
		LDFLAGS += -L$(CHECK_PREFIX)/lib
	endif

	NCURSES_PREFIX := $(shell brew --prefix ncurses 2>/dev/null)
    ifneq ($(NCURSES_PREFIX),)
        CFLAGS_CHECK += -I$(NCURSES_PREFIX)/include
        LDFLAGS += -L$(NCURSES_PREFIX)/lib -lncurses
    endif
else
	LDFLAGS += -lsubunit -lncursesw
endif

# Директории и файлы
CORE_DIR = brick_game/tetris
GUI_DIR = gui/cli

CORE_SRC = $(CORE_DIR)/tetris_lib.c
CORE_OBJ = $(CORE_SRC:.c=.o)
LIB_NAME = $(CORE_DIR)/libtetris.a

GUI_SRC = $(GUI_DIR)/tetris_cli.c
GUI_OBJ = $(GUI_SRC:.c=.o)

TEST_SRC = $(CORE_DIR)/test.c
TEST_OBJ = $(TEST_SRC:.c=.o)

.PHONY: all test clean install uninstall dist dvi gcov_report

# Основная цель
all: brickgame

# Статическая библиотека
$(LIB_NAME): $(CORE_OBJ)
	ar rcs $@ $^

# Основная программа
brickgame: $(LIB_NAME) $(GUI_OBJ)
	$(CC) $(CFLAGS) -o $@ $(GUI_OBJ) -L$(CORE_DIR) -ltetris -lncurses

# Тестовая программа
test_app: $(LIB_NAME) $(TEST_OBJ)
	$(CC) $(CFLAGS_CHECK) -o $@ $(TEST_OBJ) -L$(CORE_DIR) -ltetris $(LDFLAGS) 

test: test_app
	./test_app

# Правила компиляции объектов
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_OBJ): %.o: %.c
	$(CC) $(CFLAGS_CHECK) -c $< -o $@

# Очистка
clean:
	rm -f brickgame test_app 
	rm -f $(CORE_OBJ) $(GUI_OBJ) $(TEST_OBJ) $(LIB_NAME)
	rm -f */*/*.gcno */*/*.gcda */*/*.gcov test_app highscore.dat
	rm -rf $(BUILD_DIR) $(DIST_DIR) $(DIST_NAME) coverage_report coverage.info docs
	rm -rf build dist
	rm -rf html latex Doxyfile


# Установка
install: brickgame
	@if [ ! -d $(HOME)/bin ]; then mkdir -p $(HOME)/bin; fi
	cp brickgame $(HOME)/bin/
	@echo "Game installed to $(HOME)/bin/brickgame"
	@echo "To use from anywhere, $(HOME)/bin must be in PATH."
	@read -p "Add $(HOME)/bin to PATH in ~/.bashrc? (Y/N): " answer; \
	if [ "$$answer" = "Y" ]; then \
		echo "export PATH=\$$PATH:$(HOME)/bin" >> ~/.bashrc; \
		echo "Added to ~/.bashrc."; \
		echo "To apply changes, run manually: source ~/.bashrc"; \
		echo "Then restart terminal or open new window and test: brickgame"; \
	else \
		echo "Skipped PATH setup. Run locally with: $(HOME)/bin/brickgame"; \
	fi

# Удаление
uninstall:
	rm -f $(HOME)/bin/brickgame
	@echo "Game uninstalled from $(HOME)/bin/brickgame"

# Дистрибутив
dist: clean
	mkdir -p dist
	cp -r ./brick_game ./gui Makefile dist/
	cd dist && tar -czf ../dist/brickgame_tetris.tar.gz brick_game gui Makefile
	rm -rf dist/brick_game dist/gui dist/Makefile

# Документация (Doxygen)
DOXYGEN_CONFIG = Doxyfile
DOXYGEN_OUTPUT = docs

$(DOXYGEN_CONFIG):
	doxygen -g $@

dvi: $(DOXYGEN_CONFIG)
	echo "INPUT = brick_game gui" >> Doxyfile
	echo "OUTPUT_DIRECTORY = $(DOXYGEN_OUTPUT)" >> Doxyfile
	echo "GENERATE_HTML = YES" >> Doxyfile
	echo "GENERATE_LATEX = YES" >> Doxyfile
	echo "RECURSIVE = YES" >> Doxyfile
	echo "PROJECT_NAME = Brick_game" >> Doxyfile
	echo "EXTRACT_ALL = YES" >> Doxyfile
	echo "OUTPUT_LANGUAGE = Russian" >> Doxyfile
	echo "OPTIMIZE_OUTPUT_FOR_C = YES" >> Doxyfile
	echo "GENERATE_TREEVIEW = YES" >> Doxyfile 
	echo "CALL_GRAPH = YES" >> Doxyfile
	echo "HAVE_DOT = YES" >> Doxyfile
	echo "JAVADOC_AUTOBRIEF = YES" >> Doxyfile
	echo "SOURCE_BROWSER = YES" >> Doxyfile
	echo "EXCLUDE = brick_game/tetris/test.c" >> Doxyfile
	echo "QUIET = YES" >> Doxyfile
	doxygen $(DOXYGEN_CONFIG)

# Покрытие кода
gcov_report: CFLAGS += --coverage
gcov_report: clean test
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage_report

# Дополнительные цели

clang-format:
	cp ../materials/linters/.clang-format ./
	clang-format -n $(GUI_DIR)/*.c $(GUI_DIR)/*.h $(CORE_DIR)/*.c $(CORE_DIR)/*.h 

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem $(GUI_DIR) $(CORE_DIR)

valgrind: test
	valgrind --leak-check=full ./test_app

count_depth:
	find . -name '*.c' -exec awk '\
	  { for (i=1; i<=length($$0); i++) {\
	      c = substr($$0,i,1);\
	      if (c == "{") depth++;\
	      else if (c == "}") depth--;\
	    }\
	    if (depth > max) max = depth;\
	  }\
	  END { print FILENAME ": max depth = " max }\
	' {} \;