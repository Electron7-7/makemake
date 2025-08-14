LINUX_CXX := clang++
LINUX_CC  := clang

ifneq ($(OS),Windows_NT)
	WINDOWS_CXX := i686-w64-mingw32-g++
	WINDOWS_CC  := i686-w64-mingw32-gcc
else
	WINDOWS_CXX := g++
	WINDOWS_CC  := gcc
endif

FLAGS_DEBUG_COMMON    := -g -O0 -D DEBUGGING
FLAGS_DEBUG_LINUX     := # -fsanitize=address
FLAGS_DEBUG_WINDOWS   := # Nothing yet
FLAGS_RELEASE_COMMON  := -O3
FLAGS_RELEASE_WINDOWS := # Nothing yet
FLAGS_RELEASE_LINUX   := # Nothing yet
FLAGS_CXX_COMMON      := -Wall -std=c++23
FLAGS_CC_COMMON       := -Wall -std=c11
FLAGS_WINDOWS         := # Nothing yet
FLAGS_LINUX           := # Nothing yet
LDFLAGS_LINUX         :=  -Lsrc/lib -lgetargs
LDFLAGS_WINDOWS       := -lstdc++exp -Lsrc/lib -lgetargs

INCLUDE := -I src -I src/include

DIR_ROOT    := build
DIR_LINUX   := Linux
DIR_WINDOWS := Windows
DIR_DEBUG   := Debug
DIR_RELEASE := Release
DIR_OBJS    := .objs

NAME_BASE := makemake

# LINUX
ifneq ($(OS),Windows_NT)
	export NAME          ?= $(NAME_BASE)
	export BUILD_ARCH    ?= $(DIR_LINUX)
	export DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX)
	export RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX)
	export CXX_FLAGS     ?= $(FLAGS_CXX_COMMON) $(FLAGS_LINUX)
	export CC_FLAGS      ?= $(FLAGS_CC_COMMON) $(FLAGS_LINUX)
	export LD_FLAGS      ?= $(LDFLAGS_LINUX)
	export CXX_COMPILER  ?= $(LINUX_CXX)
	export C_COMPILER    ?= $(LINUX_CC)
else # WINDOWS
	export NAME          ?= $(NAME_BASE).exe
	export BUILD_ARCH    ?= $(DIR_WINDOWS)
	export DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS)
	export RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS)
	export CXX_FLAGS     ?= $(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS)
	export CC_FLAGS      ?= $(FLAGS_CC_COMMON) $(FLAGS_WINDOWS)
	export LD_FLAGS      ?= $(LDFLAGS_WINDOWS)
	export CXX_COMPILER  ?= $(WINDOWS_CXX)
	export C_COMPILER    ?= $(WINDOWS_CC)
endif

export BUILD_VERSION ?= $(DIR_RELEASE)
export VERSION_FLAGS ?= $(RELEASE_FLAGS)

export BUILD_DIR  ?= $(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)
export BUILD_OBJS ?= $(BUILD_DIR)/$(DIR_OBJS)

export CLEAN_ARCH    ?= .+
export CLEAN_VERSION ?= .+

VPATH := $(SRC_DIRS)

SRC := src

SRC_DIRS :=         \
    $(SRC)/makefile \
    $(SRC)/system   \

CC_SRCS  := $(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.c))
CXX_SRCS := $(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.cpp))

export CC_OBJS  ?= $(addprefix $(BUILD_OBJS)/,$(subst .c,.o,$(CC_SRCS:$(SRC)/%=%)))
export CXX_OBJS ?= $(addprefix $(BUILD_OBJS)/,$(subst .cpp,.obj,$(CXX_SRCS:$(SRC)/%=%)))

export RESET   ?= \\x1b[0m
export BLACK   ?= \\x1b[1;30m
export RED     ?= \\x1b[1;31m
export GREEN   ?= \\x1b[1;32m
export YELLOW  ?= \\x1b[1;33m
export BLUE    ?= \\x1b[1;34m
export MAGENTA ?= \\x1b[1;35m
export CYAN    ?= \\x1b[1;36m
export WHITE   ?= \\x1b[1;37m
export DEFAULT ?= \\x1b[1;39m

.PHONY: build linux windows release debug build_dirs .__clean_target .__clean_all clean mostlyclean disable_colors

build:
	@ printf "$(DEFAULT)::Architecture - $(BLUE)$(BUILD_ARCH)$(RESET)\n"
	@ printf "$(DEFAULT)::Version - $(BLUE)$(BUILD_VERSION)$(RESET)\n"
	@ printf "$(DEFAULT)::C Compile Command - $(YELLOW)$(C_COMPILER) $(CC_FLAGS) $(VERSION_FLAGS) $(INCLUDE)$(RESET)\n"
	@ printf "$(DEFAULT)::C++ Compile Command - $(YELLOW)$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE)$(RESET)\n"
	@ $(MAKE) -s $(BUILD_DIR)/$(NAME)
	@ printf "$(DEFAULT)::Program Location - $(GREEN)$(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)/$(NAME)$(RESET)\n"

linux: ;@:
	$(eval NAME          = $(NAME_BASE))
	$(eval BUILD_ARCH    = $(DIR_LINUX))
	$(eval DEBUG_FLAGS   = $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX))
	$(eval RELEASE_FLAGS = $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX))
	$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_LINUX))
	$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_LINUX))
	$(eval LD_FLAGS      = $(LDFLAGS_LINUX))
	$(eval CXX_COMPILER  = $(LINUX_CXX))
	$(eval C_COMPILER    = $(LINUX_CC))
	$(eval TARGET_CALLED = 1)
	$(eval CLEAN_ARCH    = $(DIR_LINUX))

windows: ;@:
	$(eval NAME          = $(NAME_BASE).exe)
	$(eval BUILD_ARCH    = $(DIR_WINDOWS))
	$(eval DEBUG_FLAGS   = $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS))
	$(eval RELEASE_FLAGS = $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS))
	$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS))
	$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_WINDOWS))
	$(eval LD_FLAGS      = $(LDFLAGS_WINDOWS))
	$(eval CXX_COMPILER  = $(WINDOWS_CXX))
	$(eval C_COMPILER    = $(WINDOWS_CC))
	$(eval TARGET_CALLED = 1)
	$(eval CLEAN_ARCH    = $(DIR_WINDOWS))

release: ;@:
	$(eval VERSION_FLAGS = $(RELEASE_FLAGS))
	$(eval BUILD_VERSION = $(DIR_RELEASE))
	$(eval TARGET_CALLED = 1)
	$(eval CLEAN_VERSION = $(DIR_RELEASE))

debug: ;@:
	$(eval VERSION_FLAGS = $(DEBUG_FLAGS))
	$(eval BUILD_VERSION = $(DIR_DEBUG))
	$(eval TARGET_CALLED = 1)
	$(eval CLEAN_VERSION = $(DIR_DEBUG))

build_dirs:
	@ -mkdir -p $(BUILD_DIR)

# Cleaning Functions
CLEAN       = find $(DIR_ROOT) -type $(1) $(2) -not -path "*.git/*" -delete -print 2>/dev/null
CLEAN_OBJS  = $(call CLEAN,$(1),-regex '.+\.objs_.+/.+' $(2))
CLEAN_DIRTY = $(call CLEAN_OBJS,$(1),$(DIRTY_SRC_DIRS:%=-not -path "*%*"))
CLEAN_PRINT = \
$(eval MAKE_TARGET != if [[ -n "$(1)" && "$(1)" != " " ]]; then printf "$(1)"; else printf "NOTHING_TO_CLEAN"; fi) \
$(MAKE) -s $(foreach cleaned,$(MAKE_TARGET),$(cleaned).clean)

# Cleans directories based on previous targets
# (e.g: 'make windows clean' will clean 'build/Windows/', but 'make windows debug clean' will only clean 'build/Windows/Debug/')
.__clean_target: ;@:
	$(eval CLEAN_FILES != $(call CLEAN,f,-regex '$(DIR_ROOT)/$(CLEAN_ARCH)/$(CLEAN_VERSION)/.*'))
	$(eval CLEAN_DIRS  != $(call CLEAN,d,-regex '$(DIR_ROOT)/$(CLEAN_ARCH)/$(CLEAN_VERSION)'))
	$(eval EMPTY_DIRS  != $(call CLEAN,d,-empty -regex '$(DIR_ROOT)/$(CLEAN_ARCH)'))
	@ $(call CLEAN_PRINT,$(EMPTY_DIRS))

# Cleans the entire 'build/' directory (default behaviour for 'clean')
.__clean_all: ;@:
	$(eval CLEAN_FILES != $(call CLEAN,f,-regex '$(DIR_ROOT)/.*'))
	$(eval CLEAN_DIRS  != $(call CLEAN,d,-regex '$(DIR_ROOT)/.*'))
	$(eval EMPTY_DIR   != find ./ -type d -not -path "*.git*" -empty -delete -print 2>/dev/null)
	@ $(call CLEAN_PRINT,$(EMPTY_DIR))

# What 'clean' does depends on if it's called by itself, or after other targets
clean:
	@ if [ "$(TARGET_CALLED)" == "1" ]; then \
	    $(MAKE) -s .__clean_target;          \
	else                                     \
	    $(MAKE) -s .__clean_all;             \
	fi

# 'mostlyclean' doesn't clean files from 'DIRTY_SRC_DIRS'
mostlyclean:
	$(eval CLEAN_FILES != $(call CLEAN_DIRTY,f))
	$(eval CLEAN_DIRS  != $(call CLEAN_DIRTY,d -empty))
	@ $(call CLEAN_PRINT, $(CLEAN_FILES)$(CLEAN_DIRS))

disable_colors:
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
	@ printf "::Output colors disabled\n"

# C++ Object Files
$(BUILD_OBJS)/%.obj: $(SRC)/%.cpp | build_dirs
	@ printf "::Compiling $(BLUE)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@

# C Object Files
$(BUILD_OBJS)/%.o: $(SRC)/%.c | build_dirs
	@ printf "::Compiling $(BLUE)$@$(RESET)\n"
	@ -mkdir -p $(dir $@)
	$(C_COMPILER) $(CC_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@

# Program Linking
$(BUILD_DIR)/$(NAME): $(CC_OBJS) $(CXX_OBJS)
	@ printf "::Linking $(GREEN)$@$(RESET)\n"
	$(CXX_COMPILER) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) $^ -o $@ $(LD_FLAGS)


# Prints a unique cleanup message
NOTHING_TO_CLEAN.clean:
	@ printf "::Nothing left to clean\n"

# Prints a cleanup message
%.clean:
	@ printf "::Cleaned $(RED)$*$(RESET)\n"
