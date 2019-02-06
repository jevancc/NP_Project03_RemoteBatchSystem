CXX = g++
CFLAGS = -std=c++11 -Wall -pedantic -pthread -lboost_system -lboost_filesystem
INCLUDES = \
	-Isrc/include \
	-Ilib/fmt/include \
	-Ilib/json/include

.PHONY: default all run
default: all

OUT := ./build
SRC := ./src/http_server
LIB := ./lib
OBJ := $(OUT)/obj/http_server
EXE := $(OUT)/http_server
ALL := \
	$(wildcard $(LIB)/fmt/src/*.cc) \
	$(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(wildcard $(SRC)/*.cc))
ALL_EXE := $(ALL_EXE) $(EXE)

$(OBJ)/%.o: $(SRC)/%.cc $(shell find ./src/include/np/http_server/ -name "*.h")
	@mkdir -p ./build/obj/http_server
	$(CXX) $(INCLUDES) -c $< -o $@ $(CFLAGS)

$(EXE): $(ALL)
	$(CXX) $(INCLUDES) $^ -o  $@ $(CFLAGS)
	@chmod 775 $@

OUT := ./build
SRC := ./src/console_cgi
LIB := ./lib
OBJ := $(OUT)/obj/console_cgi
EXE := $(OUT)/console.cgi
ALL := \
	$(wildcard $(LIB)/fmt/src/*.cc) \
	$(patsubst $(SRC)/%.cc,$(OBJ)/%.o,$(wildcard $(SRC)/*.cc))
ALL_EXE := $(ALL_EXE) $(EXE)

$(OBJ)/%.o: $(SRC)/%.cc $(SRC)/static_html.hhpp $(shell find ./src/include/np/cgi/console/ -name "*.h")
	@mkdir -p ./build/obj/console_cgi
	$(CXX) $(INCLUDES) -c $< -o $@ $(CFLAGS)

$(SRC)/static_html.hhpp: $(SRC)/web/RenderStaticHTMLCPP.js $(OUT)/web/index.html
	$(SRC)/web/RenderStaticHTMLCPP.js > $@

$(EXE): $(ALL)
	$(CXX) $(INCLUDES) $^ -o  $@ $(CFLAGS)
	@chmod 775 $@

all: $(ALL_EXE)

run: all
	echo $(ALL_EXE)
	cd build; ./http_server 8080
