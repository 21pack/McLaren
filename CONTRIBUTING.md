# Contributing Guide

Hello there! So glad you want to help out with McLaren. Here’s a guide for how you can contribute to the project. You can not only find the source code of engine but also of game in our engine in this repository.

This document provides guidelines and instructions to help you contribute effectively. Following these guidelines helps to maintain a high-quality codebase and makes the review process smoother for everyone.

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [How Can You Contribute](#how-can-you-contribute)
3. [Issue Guidelines](#issue-guidelines)
4. [Pull Request Guidelines](#pull-request-guidelines)
5. [Commit Message Convention](#commit-message-convention)
6. [C++ Code Style Guide](#c-code-style-guide)

## Code of Conduct

We are committed to providing a friendly, safe, and welcoming environment for all. By participating, we expect you to follow our Code of Conduct.

## How Can You Contribute

### Reporting Bug

You can help us by reporting any bugs you find. Please see the [Issue Guidelines](#issue-guidelines) below.

### Suggesting Enhancement

We welcome ideas for new features and improvements to existing ones. Feel free to open an issue to discuss your suggestion.

### Pulling Request

If you want to contribute improvements or new features we are happy to review your PR.
Please use a meaningful commit message and add a little description of your changes. Test your changes with the existing examples or add a new one if it's needed for your changes. Please follow our [Pull Request Guidelines](#pull-request-guidelines).

## Issue Guidelines

Before creating a new issue, please search the existing ones to avoid duplicates.

### What to Include in a Bug Report

A good bug report should include the following information:

* **A clear and descriptive title.**
* **A detailed description of the problem.**
* **Steps to reproduce:** A step-by-step list of actions that trigger the bug.
* **Expected behavior:** What you thought should happen.
* **Actual behavior:** What actually happened.
* **Environment:** Your OS, compiler version, and hardware if relevant.

### What to Include in a Feature Request

* **A clear and descriptive title.**
* **A detailed description of the proposed feature.**
* **Motivation:** Explain why this feature would be useful to the engine and its users.
* **Possible Implementation (Optional):** If you have ideas on how to implement it, please share.

## Pull Request Guidelines

1. **Fork the Repository.**
2. **Create a Branch:** Create a feature branch from the `main` or `develop` branch. Use a descriptive name (e.g., `fix/audio-crash-linux` or `feature/particle-editor`).
3. **Code:** Write your code, following our [C++ Code Style Guide](#c-code-style-guide).
4. **Test:** Ensure your changes do not break existing functionality and add new tests if applicable.
5. **Commit:** Make your commits with clear messages using our [commit convention](#commit-message-convention).
6. **Push & Pull Request:** Push your branch to your fork and open a Pull Request against the upstream `develop` branch.

### PR Description Template

When opening a PR, please use the following template to provide context:

```markdown
## Summary
<!-- A brief description of what this PR does. -->

## Related Issue
<!-- Link to the issue this PR fixes (e.g., "Fixes #123"). Use "Relates to #456" for related issues. -->

## Changes
<!-- A more detailed list of changes made in this PR. -->

## Testing
<!-- Describe the tests you performed to verify your changes. -->
```

## Commit Message Convention

We follow a structured commit message format to maintain a clear and searchable history. This convention is based on [Conventional Commits](https://www.conventionalcommits.org/). Please ensure your Git commit email is associated with your GitHub account. All commits must be signed using `--signoff`.

### Format

```.
<type>: <short summary>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

### Types

* `feat`: A new feature.
* `fix`: A bug fix.
* `docs`: Documentation only changes.
* `style`: Changes that do not affect the meaning of the code (white-space, formatting, etc.).
* `refactor`: A code change that neither fixes a bug nor adds a feature.
* `test`: Adding missing tests or correcting existing tests.

## C++ Code Style Guide

Please adhere to the following rules.

### Naming Conventions

* **Classes, Structs, Enums:** `PascalCase`
* **Functions & Methods:** `camelCase`
* **Variables:** `camelCase`
* **Constants & Enumerators:** `UPPER_SNAKE_CASE`
* **Macros:** `UPPER_SNAKE_CASE`
* **Private Member Variables:** `m_camelCase`

### Formatting

* **Style:** LLVM.
* **Indentation:** Use tab.
* **Line Length:** Aim for 70-80 characters per line.
