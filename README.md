# MakeMake
A C++ Makefile generator for simple, scalable builds.

```
    Usage: makemake [-hvn] [--no-color] [-s|--source <source_code_dir>] [-p|--name <global_ProgramName>]
        -h, --help              print help document
        -v, --version           print program version
        -n, --dry-run           print the generated Makefile instead of creating/replacing it
        -s, --source SOURCEDIR  set the source code directory (defaults to 'src')
        -p, --name NAME         set the name of the final binary (defaults to the name of the current directory)
    
    Examples:
        makemake
        makemake -s my_source_code_is_in_this_directory
        makemake -p the-name-of-my-executable
        makemake -n >> Makefile.test
```

# Todo
    - Implement updating certain parts of an existing Makefile instead of overwriting it entirely
        - For example: changing the C++/C standard or fully replacing the `FLAGS_DEBUG_LINUX` variable without overwriting any additional customization done to the file
        - This will require more complex file interaction; for example, the ability to search for specific strings in the Makefile in order to replace them
    - Implement interactive editing of a Makefile
    - Implement creating multiple binaries based on multiple unique source directories
        - This will probably require further modularizing the makefile generation, which is something I wanted to do anyways
    - Implement generating a Makefile for building libraries
