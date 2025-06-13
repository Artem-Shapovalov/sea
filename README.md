<p align="center">
  <img src="logo.svg" width="300"/><br>
  Every C/C++ developer deserves just to develop his project.<br>
  <a href="https://github.com/Artem-Shapovalov/sea/actions/workflows/build.yml">
    <img style="margin: 50px;" src="https://github.com/Artem-Shapovalov/sea/actions/workflows/build.yml/badge.svg"/>
  </a>
</p>

# sea

C and C++ development toolkit.

# Motivation

Lots of the projects have the common pain point - the build system. Build scrips are complicated, testing and CI is complicated, everything is complicated, build systems requires lots of time to learn it just for simple task - build a binary. Further more, all that steps should be billable, but customers don't want ever hear about all of that.

This project goal is make your development as a vacation on seashore.

# Features

- Interactive interface, you don't need to learn harsh syntax and read tons of documents, ask LLM to solve your problem, spend a time at stackoverflow, etc. Just relax.

- Build system, it just builds the sources and do not bloat your source directory, it just builds all of containment in a single file.

# Installation

Dependencies: gcc compiler

Clone the project and 'make install'

# Usage

1. Navigate to your sources directory.

2. Run sea

3. Change the project settings as you wish if needed.

4. Build the project.

5. If you want to build in CLI just run 'sea --build'

6. Have fun!

# Not implemented at this iteration

- Build system, tracking changes and conditional build.

- Build system, for now it works with gcc only.

- Build system, make debug and release binaries build.

- Interface, the project variables are not visible for now, but can be used.

- Hosts, implement Windows version. May be mac too.

# Project notes

The sources looks old-fashioned and outdated and there are some reasons for it:

- It's a proof that you don't need to be scammed of the cutting edge last standards of the language you use. The older ones are enough to make useful software without any loss in readability and maintainability.

- No special code editor was used, just vanilla Vim in a 80 columns screen, maximum tabs opened was 2. Indeed, featured code editor is no needed to make a good enough project. It even better because you forced to write simple and modular code.

- C++ stdlib is useful, but ugly, I don't want to see it inside the project, that's why it fenced off.

- Sea builds itself.
