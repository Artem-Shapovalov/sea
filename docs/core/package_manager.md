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

- 'package name' is the name of the file in 'packages' directory without extension.

- 'list of deps' is a list of another package names that should be installed additionally. Separated with whitespace. Optional, may be empty.

- 'checksum' is a hexadecimal CRC32 of 7z array with package. Optional. In case, when this field is empty, no file would be downloaded except dependencies.

## pkglist.chksum

Contains hexadecimal CRC32 of the pkglist file.

# Package format

Package is a 7z archive that contains at least 'pkginfo' file in the root.

## pkginfo