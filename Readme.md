#  Farmer's Market Crop Tracker

> A lightweight, terminal-based crop price tracker written in C — built for the farmer who wants to know whether tomatoes are having a good week or a terrible one.

---

## What Is This?

Imagine you're running a small farmer's market. You've got categories of produce — Vegetables, Fruits, Grains — and under each one, dozens of individual crops. Every day, prices fluctuate. You want to know: *What's the average price of wheat this week? What's the highest rice has ever sold for?*

That's exactly what this program solves.

**Market Crop Tracker** is a command-line application written in pure C that lets you organise crops into categories, log their daily market prices, and query statistics like averages, maximums, and minimums — all stored in memory using hand-crafted data structures.

No databases. No external libraries. Just clean C, two binary search trees, and a linked list.

---

## Data Structures at the Heart of It

This project isn't just a utility — it's a deliberate exercise in composing multiple data structures together in a layered, real-world scenario. Here's how it works under the hood:

###  Two-Level Binary Search Tree (BST)

The program maintains a **BST of categories** (e.g., Vegetables, Fruits). Each category node itself contains a **BST of crops** (e.g., Tomato, Carrot, Wheat). Both trees are ordered alphabetically using `strcmp`.

Why a BST and not an array or hash map? Because BSTs give us **O(log n)** insertion and lookup in the average case, keep data sorted for free (so `displayAll` traverses in alphabetical order via in-order traversal), and are straightforward to implement without any standard library magic.

```
CategoryBST
├── Fruits
│   └── CropBST
│       ├── Apple  → [prices: 40 → 38 → 42]
│       └── Mango  → [prices: 80 → 75]
└── Vegetables
    └── CropBST
        ├── Carrot → [prices: 20 → 22 → 19]
        └── Tomato → [prices: 30 → 28]
```

###  Singly Linked List for Daily Prices

Each crop node holds a **singly linked list** of prices — one node per price entry you log. New prices are appended to the tail, preserving chronological order. This makes sense because prices are ever-growing; you keep logging them forward in time.

---

## Time Complexity Breakdown

Let's be precise. Let:
- **C** = number of categories
- **K** = number of crops in a category
- **P** = number of price entries for a crop

| Operation | Function | Complexity | Notes |
|---|---|---|---|
| Add Category | `insertCategory` | **O(log C)** avg, O(C) worst | BST insert; worst case on sorted input (degenerates to linked list) |
| Find Category | `findCategory` | **O(log C)** avg, O(C) worst | BST search |
| Add Crop to Category | `addCropToCategory` | **O(log C + log K)** avg | Finds category, then BST-inserts crop |
| Find Crop | `findCrop` | **O(log K)** avg, O(K) worst | BST search within category |
| Add Price | `addPrice` | **O(log C + log K + P)** | Traverses the price linked list to append at tail |
| Display Average | `displayCropAverage` | **O(log C + log K + P)** | Traverses entire price list once to sum |
| Display Max/Min | `displayCropMaxMin` | **O(log C + log K + P)** | Single pass over price list |
| Display All | `displayAll` | **O(C × K)** | In-order traversal of both trees |
| Free Memory | `freeAll` | **O(C × K × P)** | Must visit every node at every level |

> **Note on the BST worst case:** An unbalanced BST degenerates to O(n) if categories or crops are inserted in already-sorted order (e.g., Apple → Banana → Cherry). For the scale of a typical farmer's market, this is rarely an issue — but it's worth knowing. A future improvement could use an AVL tree or a red-black tree for guaranteed O(log n).

> **On price insertion:** Appending to a linked list tail currently requires traversing the whole list — O(P). This could be optimised to O(1) by storing a tail pointer in `CropNode`.

---

## Space Complexity

| Structure | Space |
|---|---|
| Category BST | O(C) |
| Crop BSTs (all combined) | O(C × K) |
| Price lists (all combined) | O(C × K × P) |
| **Total** | **O(C × K × P)** |

Everything lives in heap-allocated memory. The program carefully frees all of it on exit via `freeAll`, doing a post-order traversal to ensure child nodes are freed before parents.

---

## Features

| # | Feature | What it does |
|---|---|---|
| 1 | **Add Category** | Creates a new top-level category in the category BST |
| 2 | **Add Crop to Category** | Inserts a crop into the BST belonging to a specific category |
| 3 | **Add Daily Price** | Appends a new price entry to a crop's price history |
| 4 | **Show Average Price** | Computes and displays the mean of all recorded prices for a crop |
| 5 | **Show Max/Min Price** | Finds the highest and lowest prices ever recorded for a crop |
| 6 | **View All Categories & Crops** | In-order traversal of both BSTs — prints everything alphabetically |
| 7 | **Exit** | Frees all heap memory and exits cleanly |

---

## Project Structure

```
Market-Crop-Tracker/
├── main.c       # Entry point — the menu loop and user input handling
├── crop.h       # All struct definitions and function declarations
├── crop.c       # All function implementations
├── Makefile     # Build configuration
└── tracker.exe  # Pre-compiled Windows binary (if you just want to run it)
```

The separation of `crop.h` and `crop.c` keeps concerns cleanly divided: the header is your contract (what exists), the source file is your implementation (how it works).

---

## Building & Running

### Prerequisites

You need a C compiler. `gcc` works great.

```bash
# Clone the repo
git clone https://github.com/smonty-19/Market-Crop-Tracker.git
cd Market-Crop-Tracker

# Build using Make
make

# Run
./tracker
```

Or compile manually:

```bash
gcc -o tracker main.c crop.c
./tracker
```

On Windows, you can just run the included `tracker.exe` directly.

---

## Example Session

```
--- Farmer's Market Tracker ---
1. Add Category
...
Enter choice: 1
Enter category name: Vegetables

Enter choice: 2
Enter category name: Vegetables
Enter crop name: Tomato
Added crop 'Tomato' under category 'Vegetables'.

Enter choice: 3
Enter category: Vegetables
Enter crop: Tomato
Enter price: 28.5

Enter choice: 3
Enter category: Vegetables
Enter crop: Tomato
Enter price: 31.0

Enter choice: 4
Enter category: Vegetables
Enter crop: Tomato
Average price of Tomato (Vegetables): 29.75

Enter choice: 5
Enter category: Vegetables
Enter crop: Tomato
Tomato (Vegetables): Max = 31.00 | Min = 28.50
```

---

## Design Decisions Worth Noting

**Why C?** It forces you to think about memory, pointers, and the actual cost of every operation. There's no garbage collector hiding your mistakes. Every `malloc` is your responsibility and every `free` is intentional. Working in C at this level makes you genuinely understand what higher-level languages do for you.

**Why not a hash map for O(1) lookup?** A BST was chosen deliberately — it keeps all data sorted, making `displayAll` an elegant in-order traversal. A hash map would need a separate sort step for display and is more complex to implement from scratch.

**Why a linked list for prices and not an array?** Prices grow dynamically over time with no known upper bound. A linked list avoids the need to pre-allocate or resize. The trade-off is that computing averages/max/min requires a full traversal, but since you need to visit every price anyway, this is unavoidable regardless of the data structure.

---

## Potential Improvements

- **Tail pointer on price list** — O(1) insertion instead of O(P)
- **AVL or Red-Black BST** — Guaranteed O(log n) even on sorted input
- **File persistence** — Save and load market data between sessions
- **Delete operations** — Remove a crop or category (BST deletion is the classic hard case)
- **Price timestamps** — Track *when* a price was recorded, not just the value
- **Summary across all crops** — Which crop had the highest average this week?

---

## License

MIT — do whatever you like with it.

---
