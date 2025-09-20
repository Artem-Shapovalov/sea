# High level design

![components](components.png)

# User interface

## CLI

The purpose is to allow user to use the Sea in his CI/CD pipelines. Should handle next cases:

1. Help.
2. Build debug version of the project binary.
3. Build release version of the project binary.
4. Run tests.
5. Run custom user automation script.

## Interactive

This interface need to control and setup the project. Should handle next cases:

1. Build debug version of the project binary.
2. Build release version of the project binary.
4. Run tests.
5. Run custom user automation script.
6. Change the project name and version.
7. Change the project definitions.
8. Change the project include paths.
9. Install and manage libraries and tools from the repository in the current project.
10. Manage the user-defined variables of the project.

# App

## Expressions

The expression engine converts the one string to another, extracting the values of user-defined variables and global settings.

## Config file

## Project file

## Global settings

# Core

## Builder

## Unit testing

## Automation

## Package Manager

# Helpers

## Files

## Networking

## Timing

## System
