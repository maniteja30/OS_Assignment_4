
CXX = g++
CXXFLAGS = -O2 -std=c++17 -Iinclude
BUILD_DIR = build

COMMON = src/trace.cpp
PART1 = src/main.cpp src/page_table.cpp $(COMMON)
PART2 = src/main_part2.cpp src/two_level_page_table.cpp $(COMMON)
PART3_SINGLE = src/main_part3_single.cpp src/page_table.cpp src/tlb.cpp $(COMMON)
PART3_TWO = src/main_part3_two.cpp src/two_level_page_table.cpp src/tlb.cpp $(COMMON)
PART4 = src/main_part4.cpp src/physical_memory.cpp $(COMMON)

.PHONY: all part1 part2 part3-single part3-two part4 clean

all: part1 part2 part3-single part3-two part4

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

part1: $(BUILD_DIR)/memory_simulator
part2: $(BUILD_DIR)/memory_simulator_part2
part3-single: $(BUILD_DIR)/memory_simulator_part3_single
part3-two: $(BUILD_DIR)/memory_simulator_part3_two
part4: $(BUILD_DIR)/memory_simulator_part4

$(BUILD_DIR)/memory_simulator: $(PART1) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PART1) -o $@

$(BUILD_DIR)/memory_simulator_part2: $(PART2) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PART2) -o $@

$(BUILD_DIR)/memory_simulator_part3_single: $(PART3_SINGLE) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PART3_SINGLE) -o $@

$(BUILD_DIR)/memory_simulator_part3_two: $(PART3_TWO) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PART3_TWO) -o $@

$(BUILD_DIR)/memory_simulator_part4: $(PART4) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(PART4) -o $@

clean:
	rm -rf $(BUILD_DIR)