#ifndef MAKEFILE_TEMPLATE_H
#define MAKEFILE_TEMPLATE_H

#include <vector>
#include <string>

struct make_variable_t
{
public:
    make_variable_t()
    {}

    make_variable_t(const char* Name, const char* Value, const char* Sign, bool IsExported = false)
    : name(Name), value(Value), sign(Sign), is_export(IsExported)
    { is_initialized = true; }

    const char* name = "";
    const char* value = "";
    const char* sign = "";
    bool is_export = false;

    std::string GetLine() const
    {
        std::string return_value = std::string(name) + sign + value + "\n";
        if(is_export)
            return_value = "export " + return_value;
        return return_value;
    }

    bool WasInitialized() const
    { return is_initialized; }

private:
    bool is_initialized = false;
};

struct make_target_t
{
public:
    make_target_t()
    {}

    make_target_t(const char* Name, std::vector<const char*> Value)
    : name(Name), value(Value)
    { is_initialized = true; }

    const char* name = "";
    std::vector<const char*> value = {""};

    std::string GetLines() const
    {
        std::string return_value = std::string(name) + "\n";
        for(const char* line : value)
            return_value += "\t" + std::string(line) + "\n";
        return return_value + "\n";
    }

    bool WasInitialized() const
    { return is_initialized; }

private:
    bool is_initialized = false;
};

namespace MakeVariables
{
    static make_variable_t LINUX_CXX = { "LINUX_CXX", "clang++" , " := "  };
    static make_variable_t LINUX_CC  = { "LINUX_CC" , "clang"   , "  := " };

    static make_variable_t WINDOWS_CXX_LINUX = { "WINDOWS_CXX", "x86_64-w64-mingw32-g++" , " := "  };
    static make_variable_t WINDOWS_CC_LINUX  = { "WINDOWS_CC" , "x86_64-w64-mingw32-gcc" , "  := " };

    static make_variable_t WINDOWS_CXX_WINDOWS = { "WINDOWS_CXX", "g++" , " := "  };
    static make_variable_t WINDOWS_CC_WINDOWS  = { "WINDOWS_CC" , "gcc" , "  := " };

    static make_variable_t FLAGS_DEBUG_COMMON    = { "FLAGS_DEBUG_COMMON"    , "-g -Wall -O0 -D DEBUGGING" , "    := "        };
    static make_variable_t FLAGS_DEBUG_LINUX     = { "FLAGS_DEBUG_LINUX"     , "-fsanitize=address"        , "     := "       };
    static make_variable_t FLAGS_DEBUG_WINDOWS   = { "FLAGS_DEBUG_WINDOWS"   , "# Nothing yet"             , "   := "         };
    static make_variable_t FLAGS_RELEASE_COMMON  = { "FLAGS_RELEASE_COMMON"  , "-O3"                       , "  := "          };
    static make_variable_t FLAGS_RELEASE_WINDOWS = { "FLAGS_RELEASE_WINDOWS" , "# Nothing yet"             , " := "           };
    static make_variable_t FLAGS_RELEASE_LINUX   = { "FLAGS_RELEASE_LINUX"   , "# Nothing yet"             , "   := "         };
    static make_variable_t FLAGS_CXX_COMMON      = { "FLAGS_CXX_COMMON"      , "-std=c++20"                , "      := "      };
    static make_variable_t FLAGS_CC_COMMON       = { "FLAGS_CC_COMMON"       , "# Nothing yet"             , "       := "     };
    static make_variable_t FLAGS_WINDOWS         = { "FLAGS_WINDOWS"         , "-mwindows -static"         , "         := "   };
    static make_variable_t FLAGS_LINUX           = { "FLAGS_LINUX"           , "# Nothing yet"             , "           := " };
    static make_variable_t LDFLAGS_LINUX         = { "LDFLAGS_LINUX"         , "# Nothing yet"             , "         := "   };
    static make_variable_t LDFLAGS_WINDOWS       = { "LDFLAGS_WINDOWS"       , "# Nothing yet"             , "       := "     };

    static make_variable_t INCLUDE = { "INCLUDE", "-I src", " := " };

    static make_variable_t DIR_ROOT    = { "DIR_ROOT"    , "build"   , "    := " };
    static make_variable_t DIR_LINUX   = { "DIR_LINUX"   , "Linux"   , "   := "  };
    static make_variable_t DIR_WINDOWS = { "DIR_WINDOWS" , "Windows" , " := "    };
    static make_variable_t DIR_DEBUG   = { "DIR_DEBUG"   , "Debug"   , "   := "  };
    static make_variable_t DIR_RELEASE = { "DIR_RELEASE" , "Release" , " := "    };
    static make_variable_t DIR_OBJS    = { "DIR_OBJS"    , ".objs"   , "    := " };

    static make_variable_t NAME_BASE = { "NAME_BASE", "cpppp", " := " };

    namespace IfOnLinux
    {
        static make_variable_t EXPORT_NAME            = { "NAME"            , "$(NAME_BASE)"                                   , "          ?= "   , true };
        static make_variable_t EXPORT_BUILD_ARCH      = { "BUILD_ARCH"      , "$(DIR_LINUX)"                                   , "    ?= "         , true };
        static make_variable_t EXPORT_DEBUG_FLAGS     = { "DEBUG_FLAGS"     , "$(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX)"     , "   ?= "          , true };
        static make_variable_t EXPORT_RELEASE_FLAGS   = { "RELEASE_FLAGS"   , "$(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX)" , " ?= "            , true };
        static make_variable_t EXPORT_CXX_FLAGS       = { "CXX_FLAGS"       , "$(FLAGS_CXX_COMMON) $(FLAGS_LINUX)"             , "     ?= "        , true };
        static make_variable_t EXPORT_CC_FLAGS        = { "CC_FLAGS"        , "$(FLAGS_CC_COMMON) $(FLAGS_LINUX)"              , "      ?= "       , true };
        static make_variable_t EXPORT_LD_FLAGS        = { "LD_FLAGS"        , "$(LDFLAGS_LINUX)"                               , "      ?= "       , true };
        static make_variable_t EXPORT_CXX             = { "CXX"             , "$(LINUX_CXX)"                                   , "           ?= "  , true };
        static make_variable_t EXPORT_CC              = { "CC"              , "$(LINUX_CC)"                                    , "            ?= " , true };
    }

    namespace IfOnWindows
    {
        static make_variable_t EXPORT_NAME          = { "NAME"            , "$(NAME_BASE).exe"                                 , "          ?= "   , true };
        static make_variable_t EXPORT_BUILD_ARCH    = { "BUILD_ARCH"      , "$(DIR_WINDOWS)"                                   , "    ?= "         , true };
        static make_variable_t EXPORT_DEBUG_FLAGS   = { "DEBUG_FLAGS"     , "$(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS)"     , "   ?= "          , true };
        static make_variable_t EXPORT_RELEASE_FLAGS = { "RELEASE_FLAGS"   , "$(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS)" , " ?= "            , true };
        static make_variable_t EXPORT_CXX_FLAGS     = { "CXX_FLAGS"       , "$(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS)"             , "     ?= "        , true };
        static make_variable_t EXPORT_CC_FLAGS      = { "CC_FLAGS"        , "$(FLAGS_CC_COMMON) $(FLAGS_WINDOWS)"              , "      ?= "       , true };
        static make_variable_t EXPORT_LD_FLAGS      = { "LD_FLAGS"        , "$(LDFLAGS_WINDOWS)"                               , "      ?= "       , true };
        static make_variable_t EXPORT_CXX           = { "CXX"             , "$(WINDOWS_CXX)"                                   , "           ?= "  , true };
        static make_variable_t EXPORT_CC            = { "CC"              , "$(WINDOWS_CC)"                                    , "            ?= " , true };
    }

    static make_variable_t EXPORT_BUILD_VERSION = { "BUILD_VERSION", "$(DIR_RELEASE)"   , " ?= ", true };
    static make_variable_t EXPORT_VERSION_FLAGS = { "VERSION_FLAGS", "$(RELEASE_FLAGS)" , " ?= ", true };

    static make_variable_t EXPORT_BUILD_DIR  = { "BUILD_DIR" , "$(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)" , "  ?= ", true };
    static make_variable_t EXPORT_BUILD_OBJS = { "BUILD_OBJS", "$(BUILD_DIR)/$(DIR_OBJS)"                   , " ?= " , true };

    static make_variable_t VPATH = { "VPATH", "$(SRC_DIRS)", " := " };

    static make_variable_t SRC = { "SRC", "src", " := " };

    static make_variable_t SRC_DIRS = { "SRC_DIRS", "\n\t$(SRC)           \\\n\t$(SRC)/system    \\\n\t$(SRC)/arguments", " :=          \\" };

    static make_variable_t CC_SRCS  = { "CC_SRCS" , "$(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.c))"   , "  := " };
    static make_variable_t CXX_SRCS = { "CXX_SRCS", "$(foreach directory,$(SRC_DIRS),$(wildcard $(directory)/*.cpp))" , " := "  };

    static make_variable_t EXPORT_CC_OBJS  = { "CC_OBJS" , "$(addprefix $(BUILD_OBJS)/,$(subst .c,.o,$(CC_SRCS:$(SRC)/%=%)))"      , "  ?= " , true };
    static make_variable_t EXPORT_CXX_OBJS = { "CXX_OBJS", "$(addprefix $(BUILD_OBJS)/,$(subst .cpp,.obj,$(CXX_SRCS:$(SRC)/%=%)))" , " ?= "  , true };

    static make_variable_t EXPORT_RESET   = { "RESET"   , "\\\\x1b[0m"    , "   ?= "   , true };
    static make_variable_t EXPORT_BLACK   = { "BLACK"   , "\\\\x1b[1;30m" , "   ?= "   , true };
    static make_variable_t EXPORT_RED     = { "RED"     , "\\\\x1b[1;31m" , "     ?= " , true };
    static make_variable_t EXPORT_GREEN   = { "GREEN"   , "\\\\x1b[1;32m" , "   ?= "   , true };
    static make_variable_t EXPORT_YELLOW  = { "YELLOW"  , "\\\\x1b[1;33m" , "  ?= "    , true };
    static make_variable_t EXPORT_BLUE    = { "BLUE"    , "\\\\x1b[1;34m" , "    ?= "  , true };
    static make_variable_t EXPORT_MAGENTA = { "MAGENTA" , "\\\\x1b[1;35m" , " ?= "     , true };
    static make_variable_t EXPORT_CYAN    = { "CYAN"    , "\\\\x1b[1;36m" , "    ?= "  , true };
    static make_variable_t EXPORT_WHITE   = { "WHITE"   , "\\\\x1b[1;37m" , "   ?= "   , true };
    static make_variable_t EXPORT_DEFAULT = { "DEFAULT" , "\\\\x1b[1;39m" , " ?= "     , true };

    static make_variable_t PHONY = { ".PHONY", "build linux windows release debug build_dir clean disable_colors" , ": " };
}

namespace MakeTargets
{
    static make_target_t TARGET_BUILD =
    {
        "build:",
        {
            R"~(@ printf "$(DEFAULT)::Architecture - $(BLUE)$(BUILD_ARCH)$(RESET)\n")~",
            R"~(@ printf "$(DEFAULT)::Version - $(BLUE)$(BUILD_VERSION)$(RESET)\n")~",
            R"~(@ printf "$(DEFAULT)::C Compiler - $(YELLOW)$(CC)$(RESET)\n")~",
            R"~(@ printf "$(DEFAULT)::C++ Compiler - $(YELLOW)$(CXX)$(RESET)\n")~",
            R"~(@ $(MAKE) -s $(CC_OBJS) $(CXX_OBJS))~",
            R"~(@ $(MAKE) -s $(BUILD_DIR)/$(NAME))~",
            R"~(@ printf "$(DEFAULT)::Program Location - $(GREEN)$(DIR_ROOT)/$(BUILD_ARCH)/$(BUILD_VERSION)/$(NAME)$(RESET)\n")~",
        }
    };

    static make_target_t TARGET_LINUX =
    {
        "linux: ;@:",
        {
            "$(eval NAME          = $(NAME_BASE))",
            "$(eval BUILD_ARCH    = $(DIR_LINUX))",
            "$(eval DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_LINUX))",
            "$(eval RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_LINUX))",
            "$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_LINUX))",
            "$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_LINUX))",
            "$(eval LD_FLAGS      = $(LDFLAGS_LINUX))",
            "$(eval CXX           = $(LINUX_CXX))",
            "$(eval CC            = $(LINUX_CC))",
        }
    };

    static make_target_t TARGET_WINDOWS =
    {
        "windows: ;@:",
        {
            "$(eval NAME          = $(NAME_BASE).exe)",
            "$(eval BUILD_ARCH    = $(DIR_WINDOWS))",
            "$(eval DEBUG_FLAGS   ?= $(FLAGS_DEBUG_COMMON) $(FLAGS_DEBUG_WINDOWS))",
            "$(eval RELEASE_FLAGS ?= $(FLAGS_RELEASE_COMMON) $(FLAGS_RELEASE_WINDOWS))",
            "$(eval CXX_FLAGS     = $(FLAGS_CXX_COMMON) $(FLAGS_WINDOWS))",
            "$(eval CC_FLAGS      = $(FLAGS_CC_COMMON) $(FLAGS_WINDOWS))",
            "$(eval LD_FLAGS      = $(LDFLAGS_WINDOWS))",
            "$(eval CXX           = $(WINDOWS_CXX))",
            "$(eval CC            = $(WINDOWS_CC))",
        }
    };

    static make_target_t TARGET_RELEASE =
    {
        "release: ;@:",
        {
            "$(eval VERSION_FLAGS ?= $(RELEASE_FLAGS))",
            "$(eval BUILD_VERSION = $(DIR_RELEASE))",
        }
    };

    static make_target_t TARGET_DEBUG =
    {
        "debug: ;@:",
        {
            "$(eval VERSION_FLAGS ?= $(DEBUG_FLAGS))",
            "$(eval BUILD_VERSION = $(DIR_DEBUG))",
        }
    };

    static make_target_t TARGET_BUILD_DIR =
    {
        "build_dir:",
        { "@ -mkdir -p $(BUILD_DIR) $(BUILD_OBJS)" }
    };

    static make_target_t TARGET_CLEAN =
    {
        "clean:",
        {
            R"~(@ -rm -rf $(DIR_ROOT))~",
            R"~(@ printf "::Cleaned $(RED)$(DIR_ROOT)/$(RESET)\n")~",
        }
    };

    static make_target_t TARGET_DISABLE_COLORS =
    {
        "disable_colors:",
        {
            R"~($(eval RESET   := ""))~",
            R"~($(eval BLACK   := ""))~",
            R"~($(eval RED     := ""))~",
            R"~($(eval GREEN   := ""))~",
            R"~($(eval YELLOW  := ""))~",
            R"~($(eval BLUE    := ""))~",
            R"~($(eval MAGENTA := ""))~",
            R"~($(eval CYAN    := ""))~",
            R"~($(eval WHITE   := ""))~",
            R"~($(eval DEFAULT := ""))~",
            R"~(@ printf "::Output colors disabled\n")~",
        }
    };

    static make_target_t TARGET_CXX_OBJS =
    {
        R"~($(BUILD_OBJS)/%.obj: $(SRC)/%.cpp | build_dir)~",
        {
            R"~(@ printf "::Compiling $(BLUE)$@$(RESET)\n")~",
            R"~(@ -mkdir -p $(dir $@))~",
            R"~($(CXX) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@)~",
        }
    };

    static make_target_t TARGET_CC_OBJS =
    {
        R"~($(BUILD_OBJS)/%.o: $(SRC)/%.c | build_dir)~",
        {
            R"~(@ printf "::Compiling $(BLUE)$@$(RESET)\n")~",
            R"~(@ -mkdir -p $(dir $@))~",
            R"~($(CC) $(CC_FLAGS) $(VERSION_FLAGS) $(INCLUDE) -c $< -o $@)~",
        }
    };

    static make_target_t TARGET_PROGRAM =
    {
        R"~($(BUILD_DIR)/$(NAME):)~",
        {
            R"~(@ printf "::Linking $(CYAN)$@$(RESET)\n")~",
            R"~($(CXX) $(CXX_FLAGS) $(VERSION_FLAGS) $(INCLUDE) $(CC_OBJS) $(CXX_OBJS) -o $@ $(LD_FLAGS))~",
        }
    };
}

#endif // MAKEFILE_TEMPLATE_H
