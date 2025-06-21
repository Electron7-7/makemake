# Because of the way I write Makefiles and export variables, I like to make my variable names as unique as possible.
# To do that, I'll prepend all variable names with '_V_' (v for "vroom"). That's why they look like that.
# Frankly, I might try and limit this to only exported variables but I guess we'll just have to wait and see.

export _V_COMMON_DEBUG_FLAGS  := -g -Wall -O0 -D CPPPP_DEBUGGING
export _V_LINUX_DEBUG_FLAGS   := -fsanitize=address
export _V_WINDOWS_DEBUG_FLAGS :=
export _V_RELEASE_FLAGS       := -O3
export _V_COMMON_FLAGS        := -frtti -D CPPPP_FORWARD_DECLARATIONS
export _V_CXX_STD_FLAG        := -std=c++20

export _V_LINUX_LDFLAGS   := -L src/system/linux/lib
export _V_WINDOWS_LDFLAGS := -L src/system/windows/lib

export _V_LINUX_INCLUDE   := -I src/system/linux/include
export _V_WINDOWS_INCLUDE := -I src/system/windows/include
export _V_COMMON_INCLUDE  := -I src -I src/thirdparty

export _V_BUILD_ROOT         := build
export _V_BUILD_PATH_LINUX   := linux
export _V_BUILD_PATH_WINDOWS := windows
export _V_BUILD_PATH_RELEASE := release
export _V_BUILD_PATH_DEBUG   := debug

# CPPPP stands for "C++ Package Proprietor"
export _V_NAME_BASE := cpppp

export _V_VERSION_FLAGS ?= $(_V_RELEASE_FLAGS)

ifeq ($(OS),Windows_NT)
# Platform == Windows
export _V_LINUX_CXX   ?= g++
export _V_LINUX_CC    ?= gcc
export _V_WINDOWS_CXX ?= g++
export _V_WINDOWS_CC  ?= gcc

export _V_NAME        ?= $(_V_NAME_BASE).exe
export _V_BUILD_PLAT  ?= $(_V_BUILD_PATH_WINDOWS)
export _V_DEBUG_FLAGS ?= $(_V_COMMON_DEBUG_FLAGS) $(_V_WINDOWS_DEBUG_FLAGS)
export CXX            := $(_V_WINDOWS_CXX)
export CC             := $(_V_WINDOWS_CC)
export CXXFLAGS       ?= $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS) $(_V_CXX_STD_FLAG)
export CCFLAGS        ?= $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS)
export INCLUDE        ?= $(_V_COMMON_INCLUDE) $(_V_WINDOWS_INCLUDE)
else
# Platform == Linux
export _V_LINUX_CXX   ?= clang++
export _V_LINUX_CC    ?= clang
export _V_WINDOWS_CXX ?= x86_64-w64-mingw32-g++
export _V_WINDOWS_CC  ?= x86_64-w64-mingw32-gcc

export _V_NAME        ?= $(_V_NAME_BASE)
export _V_BUILD_PLAT  ?= $(_V_BUILD_PATH_LINUX)
export _V_DEBUG_FLAGS ?= $(_V_COMMON_DEBUG_FLAGS) $(_V_LINUX_DEBUG_FLAGS)
export CXX            := $(_V_LINUX_CXX)
export CC             := $(_V_LINUX_CC)
export CXXFLAGS       ?= $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS) $(_V_CXX_STD_FLAG)
export CCFLAGS        ?= $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS)
export INCLUDE        ?= $(_V_COMMON_INCLUDE) $(_V_LINUX_INCLUDE)
endif

export _V_BUILD_VER ?= $(_V_BUILD_PATH_RELEASE)
export _V_BUILD_DIR ?= $(_V_BUILD_ROOT)/$(_V_BUILD_PLAT)_$(_V_BUILD_VER)


SRC_DIRS :=       \
	src/arguments \
	src/common    \
	src/system    \

CXX_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
CC_SRCS  := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

export _V_CXX_OBJS ?= $(addprefix $(_V_BUILD_DIR)/,$(subst .cpp,.obj,$(CXX_SRCS:src/%=%)))
export _V_CC_OBJS  ?= $(addprefix $(_V_BUILD_DIR)/,$(subst .c,.o,$(CC_SRCS:src/%=%)))


# ANSI color code variables
export RESET   ?= \\033[0m
export BLACK   ?= \\033[30m
export RED     ?= \\033[31m
export GREEN   ?= \\033[32m
export YELLOW  ?= \\033[33m
export BLUE    ?= \\033[34m
export MAGENTA ?= \\033[35m
export CYAN    ?= \\033[36m
export WHITE   ?= \\033[37m
export DEFAULT ?= \\033[39m

.PHONY: build build_dir sublime linux windows release debug clean

build:
	@ printf "::Compiling object files\n"
	@ printf "::C++ Compiler - $(YELLOW)$(CXX)$(RESET)\n"
	@ printf "::C++ Flags    - $(YELLOW)$(CXXFLAGS)$(RESET)\n"
	@ printf "::C Compiler   - $(YELLOW)$(CC)$(RESET)\n"
	@ printf "::C Flags      - $(YELLOW)$(CCFLAGS)$(RESET)\n"

	@ $(MAKE) -s $(_V_CC_OBJS) $(_V_CXX_OBJS)

	@ printf "::Linking program\n"
	@ printf "::Output directory - $(YELLOW)$(_V_BUILD_DIR)$(RESET)\n"
	@ printf "::Program name     - $(YELLOW)$(_V_NAME)$(RESET)\n"

	@ $(MAKE) -s $(_V_BUILD_DIR)/$(_V_NAME)

build_dir:
	@ -mkdir -p $(_V_BUILD_DIR)

# This target is for disabling the ANSI colors.
# It's called 'sublime' bc Sublime Text's output panel doesn't support ANSI colors natively (hence why this exists).
sublime:
	$(eval RESET   := "")
	$(eval BLACK   := "")
	$(eval RED     := "")
	$(eval GREEN   := "")
	$(eval YELLOW  := "")
	$(eval BLUE    := "")
	$(eval MAGENTA := "")
	$(eval CYAN    := "")
	$(eval WHITE   := "")
	$(eval DEFAULT := "")
	@ printf "::Disabled colored output\n"

linux:
	$(eval _V_NAME        := $(_V_NAME_BASE))
	$(eval _V_BUILD_PLAT  := $(_V_BUILD_PATH_LINUX))
	$(eval _V_DEBUG_FLAGS := $(_V_COMMON_DEBUG_FLAGS) $(_V_LINUX_DEBUG_FLAGS))
	$(eval CXX            := $(_V_LINUX_CXX))
	$(eval CC             := $(_V_LINUX_CC))
	$(eval CXXFLAGS       := $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS) $(_V_CXX_STD_FLAG) $(_V_LINUX_FLAGS))
	$(eval CCFLAGS        := $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS) $(_V_LINUX_FLAGS))
	@ printf "::Architecture - Linux\n"

windows:
	$(eval _V_NAME        := $(_V_NAME_BASE).exe)
	$(eval _V_BUILD_PLAT  := $(_V_BUILD_PATH_WINDOWS))
	$(eval _V_DEBUG_FLAGS := $(_V_COMMON_DEBUG_FLAGS) $(_V_WINDOWS_DEBUG_FLAGS))
	$(eval CXX            := $(_V_WINDOWS_CXX))
	$(eval CC             := $(_V_WINDOWS_CC))
	$(eval CXXFLAGS       := $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS) $(_V_CXX_STD_FLAG))
	$(eval CCFLAGS        := $(_V_COMMON_FLAGS) $(_V_VERSION_FLAGS))
	@ printf "::Architecture - Windows\n"

debug:
	$(eval _V_VERSION_FLAGS := $(_V_DEBUG_FLAGS))
	$(eval _V_BUILD_VER     := $(_V_BUILD_PATH_DEBUG))
	@ printf "::Version - Debug\n"

release:
	$(eval _V_VERSION_FLAGS := $(_V_RELEASE_FLAGS))
	$(eval _V_BUILD_VER     := $(_V_BUILD_PATH_RELEASE))
	@ printf "::Version - Release\n"

$(_V_BUILD_DIR)/$(_V_NAME):
	@ printf "::Linking $(CYAN)$@$(RESET)\n"
	clang++ $(CXXFLAGS) $(INCLUDE) $(_V_CC_OBJS) $(_V_CXX_OBJS) -o $@

$(_V_BUILD_DIR)/%.o: src/%.c | build_dir
	@ printf "::Compiling $< -> $(CYAN)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	@ $(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@

$(_V_BUILD_DIR)/%.obj: src/%.cpp | build_dir
	@ printf "::Compiling $< -> $(CYAN)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	@ $(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@


# For when I implement different types of cleaning
define clean_with_message
	@ if [ -d $(1) ]; then printf "\n::Deleted $(RED)$(1)$(RESET)\n"; fi
	@ -rm -rf $(1)
endef

clean:
	$(call clean_with_message,$(_V_BUILD_ROOT))
