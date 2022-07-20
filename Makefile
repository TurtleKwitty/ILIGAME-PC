APPNAME = ILIGAME
COMPILER = g++
LINKER_FLAGS = -lSDL2 -lboost_filesystem
COMPILER_FLAGS = -Wall -Wextra -g -Og

SOURCE_DIR = src
BUILD_DIR = build

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.cpp) $(wildcard $(SOURCE_DIR)/**/*.cpp)
OBJ_FILES = $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/obj/%.o, $(SOURCE_FILES))
OBJ_FOLDERS = $(dir $(OBJ_FILES))

$(BUILD_DIR)/outputs/$(APPNAME): $(OBJ_FOLDERS) $(OBJ_FILES) $(BUILD_DIR)/outputs/
	$(COMPILER) -o $(BUILD_DIR)/outputs/$(APPNAME) $(OBJ_FILES) $(LINKER_FLAGS) 
	chmod +x $(BUILD_DIR)/outputs/$(APPNAME)

$(BUILD_DIR)/obj/%.o: $(SOURCE_DIR)/%.h
$(BUILD_DIR)/obj/%.o: $(SOURCE_DIR)/%.cpp
	$(COMPILER) $(COMPILER_FLAGS) -o $@ -c $<

$(BUILD_DIR)%/:
	mkdir -p $@

clean: 
	rm -r build

build: $(BUILD_DIR)/outputs/$(APPNAME)

run: build
	./$(BUILD_DIR)/outputs/$(APPNAME)

debug: build
	gdb ./$(BUILD_DIR)/outputs/$(APPNAME)
