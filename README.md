<p align="center">
  <img src="logo.svg" width="300"/><br/><br/>
  Every C/C++ developer deserves just to develop his project.<br/><br/>
  <a href="https://github.com/Artem-Shapovalov/sea/actions/workflows/build.yml">
    <img src="https://github.com/Artem-Shapovalov/sea/actions/workflows/build.yml/badge.svg"/>
  </a>
</p>

# sea

C and C++ development toolkit.

https://github.com/user-attachments/assets/a75c76e0-7b8f-4ded-bff6-d0f6c7bd39f4

# Motivation

Lots of the projects have the common pain point - the build system. Build scrips are complicated, testing and CI is complicated, everything is complicated, build systems requires lots of time to learn it just for simple task - build a binary. Further more, all that steps should be billable, but customers don't want ever hear about all of that.

This project goal is make your development as a vacation on seashore.

# Features

- Interactive interface:

    - Easy to use because all the necessary info is always in front of your eyes.

    - Do not be scared of mistake.

- CLI interface:

    - Just 4 keys including "--help".

    - May build and clean, that convenient to use the sea in your scripts.

- Project management:

    - Avoids compiler-specific features.

    - Have expression evaluator for all of the project settings.

- Build system:

    - Do not require build scripts at all, just build every source file around in the output binary of chosen type.

    - Builds only files affected to changes.

    - Do not bloat your project directory with lots of the objects.

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

- Build system:

    - For now it works with gcc only.

    - for now no debug versions.

- Hosts:

    - for now it's linux-only.

# Project rules

1. Sea builds itself. Rationale: that's the best proof of concept.

2. No "modern" cpp. Rationale: use STL as library, not a language extension surrogate.

3. Avoid use auto as much as you can. Rationale: best option to use auto is inside templates, in other places is leads to loosing control of the source.

4. Recommended code editor is plain vim. Rationale: by default it has no autocomplete, it formats the code with a linux kernel standards, together it forces you to write cleaner code because it's hard to develop something overcomplicated without special tools for it.

5. Don't use singletons. Just write some functions and use static data. Rationale: special olympics competition of making yet another singleton implementation is over.

6. Maximum code width is 80 characters. Rationale: make your code clearer, decreases nesting level and makes you thinking about the names without that messy namespaces.
