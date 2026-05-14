# The Iron Nexus — Real-Time Railway Management System

A comprehensive command-line railway management system built entirely from scratch in C++ with no STL containers.

---

## Group Members

| Name           | Roll No.  | Section |
|----------------|-----------|---------|
| Shahram Shafiq | 24i-2541  | B       |
| Haris Said     | 24i-0527  | B       |
| Haris Shafiq   | 24i-0746  | B       |

---

## Project Overview

The Iron Nexus manages the complete lifecycle of a railway environment:
- Train registration and lookup via a self-balancing AVL tree
- Coach composition management using a doubly linked list
- Station network routing with Dijkstra's shortest path
- Seat booking using a dual-layer Hash Table + BST system
- Full operation logging using a LIFO stack
- Undo/Redo with the two-stack pattern
- Persistent save/load for all modules

---

## Modules and Data Structures

| Module              | Data Structure      | Justification |
|---------------------|---------------------|---------------|
| Train Registry      | AVL Tree            | O(log n) insert/delete/search; self-balancing prevents worst-case O(n) |
| Coach Management    | Doubly Linked List  | O(1) front/back insert; bidirectional traversal; in-place reversal |
| Railway Network     | Adjacency List + Dijkstra | Sparse graph — matrix wastes memory; Dijkstra gives optimal path |
| Seating (Layer 1)   | Hash Table (Linear Probing) | O(1) average booking/lookup/cancellation |
| Seating (Layer 2)   | Binary Search Tree  | Ordered in-order traversal for sorted seat display |
| Operation Log       | Linked-List Stack   | LIFO — most recent events shown first; O(1) push/pop |
| Undo / Redo         | Two Stacks          | Standard pattern; clear redo on new action |

---

## Features

- **Train Registry**: Insert, remove, search, and display trains (in-order, pre-order, post-order AVL traversal)
- **Coach Management**: Attach at front/back/position; remove by ID; forward/backward traversal; reverse orientation
- **Network**: Add/remove stations and routes; Dijkstra shortest path by distance or travel time; connectivity check; adjacency matrix view
- **Seat Management**: Hash table for O(1) booking; BST for sorted display; dual sync on every operation
- **Operation Log**: Push/pop/peek; filter by module; view recent N entries
- **Undo/Redo** *(Bonus)*: Full action reversal for all modules using two linked-list stacks
- **File I/O**: Save and load each module independently or all at once; hierarchical BST structure file *(Bonus)*
- **Colored Terminal Output** *(Bonus)*: Full ANSI color with Windows VT processing enabled
- **Novel Optimizations** *(Bonus)*: Half-traversal coach insertion; prime-sized hash table; pre-order AVL save for faithful restore

---

## File Structure

```
.
├── DS Project/
│   ├── Coach.cpp
│   ├── Coach.h
│   ├── FileIO.cpp
│   ├── FileIO.h
│   ├── Logger.cpp
│   ├── Logger.h
│   ├── Makefile
│   ├── main.cpp
│   ├── Network.cpp
│   ├── Network.h
│   ├── Seat.cpp
│   ├── Seat.h
│   ├── Train.cpp
│   ├── Train.h
│   ├── UndoRedo.cpp
│   ├── UndoRedo.h
│   ├── Utils.cpp
│   └── Utils.h
├── .gitignore
├── README.md
├── Report.pdf
```

**Main source code** is under [`DS Project/`](https://github.com/shahramshafiq/Railway-Management-System-using-Data-Structures/tree/main/DS%20Project).  
- Each module is split into `.cpp` and `.h` files.
- [`Makefile`](https://github.com/shahramshafiq/Railway-Management-System-using-Data-Structures/blob/main/DS%20Project/Makefile) builds the project with `make`.

Other top-level files:
- [`README.md`](https://github.com/shahramshafiq/Railway-Management-System-using-Data-Structures/blob/main/README.md): This file.
- [`Report.pdf`](https://github.com/shahramshafiq/Railway-Management-System-using-Data-Structures/blob/main/Report.pdf): Original project report.
- [`.gitignore`](https://github.com/shahramshafiq/Railway-Management-System-using-Data-Structures/blob/main/.gitignore): Git ignore rules.

---

## Running the Application

### Option 1 — Makefile (requires MinGW g++ in PATH)
```
make
./IronNexus.exe
```

### Option 2 — Visual Studio (rebuild from source)
Open `DS Project\DS Project.vcxproj` in Visual Studio 2022 and press **Ctrl+F5**.

### Option 3 — Manual g++ command (requires MinGW g++ in PATH)
```
g++ -std=c++17 -Wall -O2 -o IronNexus.exe "DS Project/main.cpp" "DS Project/Utils.cpp" "DS Project/Train.cpp" "DS Project/Coach.cpp" "DS Project/Seat.cpp" "DS Project/Network.cpp" "DS Project/Logger.cpp" "DS Project/UndoRedo.cpp" "DS Project/FileIO.cpp" -o IronNexus.exe
```

---

## Runtime Notes

- A `data/` directory is created automatically on first save
- Save files are plain-text and human-readable
- BST structure is saved in hierarchical pre-order format (Bonus)
- ANSI colors require Windows 10 v1511+ or any modern Linux/macOS terminal

---

## Engineering Constraints

- **No STL**: No `vector`, `list`, `stack`, `queue`, `map`, or any pre-built container
- **Raw pointers**: All dynamic memory managed manually with `new`/`delete`
- **No advanced syntax**: No walrus operator, range-for, or complex unpacking
- **Crash-proof**: Full input validation, null checks, and edge-case handling throughout

---

## Academic Integrity

Each group implemented their own independent solution. Code is original.
