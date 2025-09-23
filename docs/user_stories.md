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

# Configure the simple project

# Trying best library for the project

# Projects with the nested sea projects

# Making a package with libraries and resources

# Unit testing

# Embedding the Sea in CI/CD pipelines
