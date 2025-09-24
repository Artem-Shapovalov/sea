# Shortest helloworld

User have the helloworld directory and he just loaded and installed the Sea. He should be able to build the example with a single command in CLI, such a `sea --build`.

## Solution

1. Check if the package configuration file exists in the current or home directory. If not, generate the default at the home directory.
2. Check if the package cache exists in the directory from config file. Initialize if not.
3. Check if the project file exists in current directory and read it. Initialize by template if no.
4. Install compiler via package manager.
5. Perform build actions.
6. Exit.

# Initialize the empty project at the directory

User have the empty directory and wants to make the project here.

## Solution 1

1. User starts `sea --init`
2. Sea asks User for the project name and project type if flags `--name` or `--type` are not given.
3. Sea generates the project file with given name and type.
4. Exit.

## Solution 2

1. User starts the sea at the empty repository.
2. Sea detects that there's no a single file around and asks User to make the project here.
3. Sea asks User for the project name and project type.
4. Sea generates the project file with given name and type.
5. Sea reads the generates file and continues in interactive mode.

# Trying best library for the project

# Projects with the nested sea projects

# Making a package with libraries and resources

# Unit testing

# Embedding the Sea in CI/CD pipelines
