<div align="center">

# Aethera

</div>

<div align="center">
  <img src="./src/assets/icons/iconLogo.jpg" height="200" alt="qt logo"  />
</div>

<img height="20">

<div align="center">

![Status](https://img.shields.io/badge/status-active-brightgreen)
![Last Commit](https://img.shields.io/github/last-commit/didimov24/Aethera)
![Issues](https://img.shields.io/github/issues-closed/didimov24/Aethera)

</div>

<div align="center">
  <img src="https://skillicons.dev/icons?i=qt" height="40" alt="qt logo"  />
  <img width="5">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" height="40" alt="cplusplus logo"  />
  <img width="5">
  <img src="https://skillicons.dev/icons?i=cmake" height="40" alt="cmake logo"  />
</div>

## Table of Contents

- [Project Description](#project-description)
  - [Objectives](#objectives)
  - [Features](#features)
- [Architecture](#architecture)
  - [Project Structure](#project-structure)
  - [Design Patterns](#design-patterns)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Building](#building)
- [Contributors](#contributors)

## Project Description

### Objectives

Aethera is a school platform. We built a desktop application for exams using Qt and C++. We wanted to:

- Make a good exam system where users must log in
- Create a question bank with many subjects and different difficulty levels
- Design a nice interface for taking exams and viewing results

### Features

- **User Management**: Users can create an account and log in. Passwords are protected using hashing.
- **Exam Sessions**: Students can take timed exams and answer multiple-choice questions.
- **Question Bank**: Questions are organized by subject and have different difficulty levels.
- **Results & Statistics**: Students can see their scores and track their progress over time.
- **Multi-Platform**: The application works on Windows, Linux, and macOS.

## Architecture

### Project Structure

```
src/
├── assets/
├── core/
│   ├── db/
│   ├── session/
│   └── utils/
├── ui/
│   ├── exam/
│   ├── home/
│   ├── inbox/
│   ├── lessons/
│   ├── login/
│   ├── profile/
│   ├── register/
│   ├── schedule/
│   ├── settings/
│   ├── sidebar/
│   └── statistics/
```

### Design Patterns

- **MVC (Model-View-Controller)**: We keep the application's work separate from how it looks. The `core/` folder has all the work (like saving and reading data), and the `ui/` folder is what users see.

- **Singleton**: The database has one instance for the whole application. All parts of the application use the same instance.

- **Repository**: We created a middle layer between the user interface and the database. The UI doesn't talk to the database directly - it asks the middle layer to get or save data. For example, when the login screen needs user data, it doesn't write SQL. Instead it does: `Database::instance().validateUser(username, password)`. The middle layer handles all SQL and database work.

## Getting Started

### Prerequisites

Make sure you have the following installed:

- **Qt Framework** (v6.9 or higher)

### Installation

Download and install [Qt Creator](https://www.qt.io/development/download-qt-installer-oss) from the official website.

### Building

Open the project in Qt Creator:

1. Open Qt Creator
2. Click **File** > **Open File or Project**
3. Select the `CMakeLists.txt` from the project folder
4. Click **Build** > **Build Project**

## Contributors

- **Denislav Dimov** - [@didimov24](https://github.com/didimov24)
- **Georgi Georgiev** - [@gageorgiev24](https://github.com/gageorgiev24)
- **Aleksandar Dyanov** - [@avdyanov24](https://github.com/avdyanov24)
- **Dimitar Rusev** - [@ddrusev24](https://github.com/ddrusev24)
