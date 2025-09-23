# Repository structure

Web and local repositories should have the same structure:

```
./pkglist
./pkglist.chksum
./packages/<package name>.7z
```

`package name` should fit regex "[0-9a-z_-]+"

## pkglist

This file contains the list of files placed in this Repository. Each line of this file have this format.

```
<package name>:[list of deps]:[checksum];
```

- `package name` is the name of the file in 'packages' directory without extension.

- `list of deps` is a list of another package names that should be installed additionally. Separated with whitespace. Optional, may be empty.

- `checksum` is a hexadecimal CRC32 of 7z array with package. Optional. In case, when this field is empty, no file would be downloaded except dependencies.

## pkglist.chksum

Contains hexadecimal CRC32 of the pkglist file.

# Package format

Package is a 7z archive that contains at least `pkginfo` file in the root.

There's two types of the packages: tooling and libraries. Tooling contains the binaries, that sea may call via inner logic or in scripts, such as compiler or linker. Library is the package that contains header/source files and library binaries. Libraries installed in the project may add their own compilation flags to the project. Libraries should not use environment or modify it. For example, some libraries may provide pkgconfig-like flags generators, but Sea forbids it, use flag set that would be common for any platform, even if the program would not be fine tuned. Portability is the priority.

## pkginfo

This file have a basic ini format. Possible fields:

- `type`, mandatory, may be `library` or `tooling`
- `definitions`, optional, suitable for libraries, comma separated list of <key>=<value> pairs, where key is the definition (without -D)
- `include_path`, optional, suitable for libraries, comma separated list of relative include paths
- `library_path`, optional, suitable for libraries, comma separated list of relative library paths for static and shared libraries
- `binary_path`, optional, suitable for tooling, comma separated list of relative paths with binaries.

# API

- `add_repo`
- `add_local_repo`
- `get_repos_list`
- `get_local_repos_list`
- `find_packages`
- `install_package`
- `remove_package`
- `set_cache_dir`
- `remove_from_cache`