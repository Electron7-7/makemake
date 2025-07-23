# MakeMake
A C++ Makefile generator for simple, scalable builds.

The default Makefile that MakeMake generates is designed to be a good starting point for projects. It includes:
	- build targets for both Windows and Linux
	- build targets for Debug and Release versions
	- lots of easily configurable variables
	- colored output
		- with a target for disabling colored output

# What is MakeMake?
MakeMake takes a unique approach towards build targets. Instead of having different targets for different builds, there's a single `build` target that will build the program; the rest of the build targets configure the exported variables, setting up the environment for the final `build` target. The tradeoff is ease of use and simplicity for complexity. Instead of having to either manually change variables in the Makefile or on the command line, or even worse: using CMake, you can just call one or more of the other targets before `build`. You can even call them in any order you want; as long as `build` is the last target called.

# What are the benefits?
At first, the above approach might seem less attractive than just doing things the old fashioned way; for example, to make a debug build you have to run `make debug build` instead of just `make debug`. However, this technique applies to all builds; if you're cross-compiling a debug Windows build on Linux, all you have to do is run `make debug windows build` *or* `make windows debug build` instead of something like `make debug CXX=x86_64-w64-mingw32-g++ CXXFLAGS+=-mwindows -static` (while hoping that everything compiles fine).

MakeMake also saves you from having to use a big, bulky build system like 'CMake' for projects that don't really need it. I personally dislike how many files CMake generates, as well as its folder structure (not to mention that 'CMakeLists.txt' actually ends in '.txt'), but creating my own Makefiles by hand for every project was a pain in the ass, even for tiny projects like, well, this one! MakeMake solves all of that (at least for me) by generating a single Makefile with highly configurable variables and targets. If you take a peek inside, it should be pretty easy to figure out how to scale it out to fit your needs. Need to add more versions? Add more variable types and 'eval' them in a separate target for that version! Need to swap out the compiler for a specific architecture? Literally just do that! MakeMake generates highly configurable and scalable Makefiles that make, well, GNU make a lot more enjoyable to use (in my opinion).

## What are the drawbacks?
Well, the nature of how the Makefile works causes some issues. How the makefile essentially works is that each target will 'eval' certain exported variables, changing their values. These variables are used to abstract target-specific values away from the variables that are used by the main `build` target. Those variables are also exported, to make sure that their values get changed along with the other variables. This, however, means that any variables that don't have unique names will get overridden; this is why I use `CXX_COMPILER` and `C_COMPILER` instead of `CXX` and `CC`, respectively. However, if you're okay with that caveat, I think the benefits of this Makefile system are really nice! You get a Makefile that acts almost like a program, with targets that act like pseudo commandline flags, and a directory structure that isn't a crime on the eyes.

## What's planned for MakeMake?
I want MakeMake to eventually be able to edit these types of Makefiles, to make changing the build system easier. As I said before, one of the caveats to this build system is that it can be a little tedious to make new build targets if you want to be really anal about it. Having MakeMake do that work for you would be really handy and is a feature I want to work on asap.
