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

User wants to try several libraries for his project and chose the best.

## Solution

1. Assume the project is already exists, the Sea started right now and runs in interactive mode.
2. User goes to the project settings and adds the package to his project.
3. Package manager loads the package and installs it to the project `deps` directory.
4. User modifies the code and builds the project.
5. User goes to the project settings and removes the package.
6. Package manager deletes the project from the `deps` directory, but keep it in cache.
7. User tries another one and so on.

# Projects with the nested sea projects

User wants to reuse the code from another project and places it in some place in his current projects.

## Solution

1. There's no other cases instead of build.
2. Sea checks the filesystem for project files.
3. Sea prepares list and sort it from the far to near.
4. Sea and package manager checks out the packages and install it if needed at the root project directory
5. Sea scans the list and collect includes and defines. Include paths are appended to the current project includes, macro definitions may be overriden in the projects near to root.
6. Builder builds the projects in list and place the output binaries at the root project directory.

# Making a package with libraries and resources

# Unit testing

# Embedding the Sea in CI/CD pipelines

# TODO

1. Seems to be Sea should have a separate component `project manager`
