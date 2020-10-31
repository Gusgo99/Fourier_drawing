add_library(DIP_strategy OBJECT "DIP_strategy.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_strategy")

add_library(DIP_strategy_utilities OBJECT "DIP_strategy_utilities.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_strategy_utilities")

add_library(DIP_threshold_strategy OBJECT "DIP_threshold_strategy.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_threshold_strategy")

add_library(DIP_selection_strategy OBJECT "DIP_selection_strategy.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_selection_strategy")

add_library(DIP_skeletonization_strategy OBJECT "DIP_skeletonization_strategy.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_skeletonization_strategy")

add_library(DIP_path_generation_strategy OBJECT "DIP_path_generation_strategy.cpp")
set(OBJECT_FILES "${OBJECT_FILES};DIP_path_generation_strategy")

set(OBJECT_FILES "${OBJECT_FILES}" PARENT_SCOPE)