# Recipe Book Manager (Recepteskönyv)

A comprehensive terminal-based application developed in **C** for managing a digital cookbook. This project was created as part of a university curriculum to demonstrate proficiency in dynamic memory management, file I/O, and modular programming.

## 🚀 Features
* **Recipe Management:** Create, view, edit, and delete recipes.
* **Ingredient Tracking:** Manage complex ingredient lists for each recipe.
* **Persistent Storage:** Data is saved to and loaded from text files (`dummyrecept.txt`).
* **Memory Safety:** Includes `debugmalloc.h` to ensure no memory leaks occur during execution.
* **Modular Design:** Separate modules for file handling, recipe logic, and menu systems.

## 🛠️ Tech Stack
* **Language:** C
* **Build System:** CMake
* **IDE:** CLion

## 📂 Project Structure
* `main.c`: The entry point of the application.
* `receptek.c/h`: Core logic for recipe handling.
* `fajlkezelo.c/h`: Functions for saving and loading data.
* `menu.c/h`: Terminal-based user interface logic.
* `osszetevok.c/h`: Management of recipe ingredients.
* `adatszerkezet.h`: Definition of custom structs and data types.

## 📖 Documentation
The project includes detailed documentation in Hungarian:
* [**User Guide**](./recepteskonyv_felhasznaloi_dok.pdf) - Instructions on how to use the software.
* [**Programming Documentation**](./recepteskonyv_programozoi_dok.pdf) - Details regarding the code architecture and data structures.

## 🔨 How to Build
This project uses **CMake**. To compile it manually:

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/THEinmager/C-Recepteskonyv.git](https://github.com/THEinmager/C-Recepteskonyv.git)
    cd C-Recepteskonyv
    ```
2.  **Build the project:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

## ⚖️ License & Academic Integrity
This repository is for educational purposes. If you are a student at the same university, please refer to your department's **Academic Honesty Policy** before using any code from this repository.
