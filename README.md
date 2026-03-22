*This project has been created as part of the 42 curriculum by migusant.*

# Philosophers

A multithreaded simulation solving the classic **Dining Philosophers Problem** using threads, mutexes, processes, and semaphores. This project demonstrates concurrent programming concepts, synchronization mechanisms, and race condition prevention in C.

## Description

The Philosophers project implements Dijkstra's famous **Dining Philosophers Problem**—a classical synchronization challenge in computer science. Philosophers sit at a round table with bowls of spaghetti and forks between them. They alternate between eating, thinking, and sleeping, but must acquire two forks to eat. The challenge is to prevent deadlocks, avoid race conditions, and detect when a philosopher dies of starvation.

### Key Features

- **Precise microsecond timing** using `gettimeofday()` for accurate simulation
- **Thread-based solution (mandatory)** using `pthread` library and mutexes
- **Process-based solution (bonus)** using separate processes and POSIX semaphores
- **Deadlock prevention** through resource ordering strategies
- **Real-time monitoring** to detect philosopher death conditions
- **Race condition protection** with proper mutex/semaphore synchronization
- **Lock-free atomic operations** (`<stdatomic.h>`) for thread-safe state management in bonus part
- **Signal handling** for graceful interruption (Ctrl+C) with atomic operations preventing race conditions
- **Adaptive staggered start** algorithm scales from small (≤5) to extra-large (200+) philosopher counts
- **Edge case handling** including single philosopher scenario

### Project Structure

```
├── philo/                 # Mandatory part (threads + mutexes)
│   ├── inc/
│   │   └── philo.h        # Header with structures and function prototypes
│   ├── src/
│   │   ├── main.c         # Entry point and simulation orchestration
│   │   ├── parser.c       # Argument parsing and validation
│   │   ├── init.c         # Initialization of philosophers and mutexes
│   │   ├── threads.c      # Thread creation and joining
│   │   ├── routine.c      # Philosopher routine (eat, sleep, think cycle)
│   │   ├── forks.c        # Fork acquisition and release logic
│   │   ├── monitor.c      # Death detection and simulation monitoring
│   │   ├── utils.c        # Time utilities and status printing
│   │   ├── signals.c      # Signal handling (SIGINT, SIGQUIT, SIGTERM)
│   │   └── cleanup.c      # Resource cleanup and mutex destruction
│   └── Makefile
│
└── philo_bonus/           # Bonus part (processes + semaphores)
    ├── inc/
    │   └── philo_bonus.h  # Header for bonus implementation
    ├── src/
    │   ├── main.c         # Entry point for process-based solution
    │   ├── parser.c       # Argument parsing
    │   ├── init.c         # Semaphore initialization
    │   ├── init_utils.c   # Semaphore creation helpers
    │   ├── processes.c    # Process forking and management
    │   ├── routine.c      # Philosopher process routine
    │   ├── routine_utils.c # Eat, sleep, think actions
    │   ├── forks.c        # Fork handling with semaphores
    │   ├── monitor.c      # Death monitoring thread per process
    │   ├── utils.c        # Utilities
    │   ├── signals.c      # Signal handling with atomic operations
    │   └── cleanup.c      # Semaphore cleanup
    └── Makefile
```

## Instructions

### Compilation

The project provides separate Makefiles for mandatory and bonus parts.

#### Mandatory Part (Threads + Mutexes)

```bash
cd philo
make        # Compile the project
make t      # Run common test cases
make v      # Run tests with Valgrind       (memory leak detection)
make h      # Run tests with Helgrind Tool  (thread error detection)
make d      # Run tests with Data Race Tool (thread error detection)
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile from scratch
```

This produces the `philo` executable.

#### Bonus Part (Processes + Semaphores)

```bash
cd philo_bonus
make        # Compile the bonus project
make t      # Run common test cases
make v      # Run tests with Valgrind (memory leak detection)
make vp     # Run tests with Valgrind (process tracking leak detection)
make d      # Run tests with Data Race Tool (thread error detection)
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile from scratch
```

This produces the `philo_bonus` executable.

`Helgrind` is a thread debugger which finds data races in multithreaded programs. It looks for memory locations which are accessed by more than one (POSIX p-)thread, but for which no consistently used (pthread_mutex_) lock can be found. Such locations are indicative of missing synchronisation between threads, and could cause hard-to-find timing-dependent problems. It is useful for any program that uses pthreads. It is a somewhat experimental tool, so your feedback is especially welcome here.

`DRD` is a tool for detecting errors in multithreaded C and C++ programs. The tool works for any program that uses the POSIX threading primitives or that uses threading concepts built on top of the POSIX threading primitives. While Helgrind can detect locking order violations, for most programs DRD needs less memory to perform its analysis. 

### Execution

Both programs accept the same arguments:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

**Arguments:**
- `number_of_philosophers`: Number of philosophers (and forks) [1-200]
- `time_to_die`: Time in milliseconds until a philosopher dies without eating
- `time_to_eat`: Time in milliseconds a philosopher spends eating
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `[number_of_times_each_philosopher_must_eat]`: Optional - simulation stops when all philosophers have eaten this many times

**Examples:**

```bash
# No philosopher should die - continuous simulation
./philo 5 800 200 200

# Simulation stops after each philosopher eats 7 times
./philo 5 800 200 200 7

# Edge case: single philosopher (should die)
./philo 1 800 200 200

# Stress test with many philosophers
./philo 199 410 200 200

# Tight death timing (should die)
./philo 4 310 200 100
```

**Output Format:**

Each status change is logged with a timestamp (in milliseconds from start) and philosopher ID:

```
201 3 is eating
201 1 is sleeping
201 4 has taken a fork
201 4 is eating
201 5 has taken a fork
401 1 is thinking
```

Death is reported as:
```
311 1 died
```

## Technical Implementation

### Mandatory Part: Threads & Mutexes

**Architecture:**
- Each philosopher is a separate **thread** (`pthread_t`)
- Each fork is protected by a **mutex** (`pthread_mutex_t`)
- A **monitor thread** in the main process checks for deaths and completion
- **Deadlock prevention**: Even-numbered philosophers acquire forks in reverse order
- **Adaptive staggered start**: Philosophers start with calculated delays based on count (small: ≤5, medium: ≤50, large: ≤100, xlarge: 200+)

**Key Synchronization Points:**
- `fork_mutexes[]`: One mutex per fork (circular array)
- `meal_mutex`: Per-philosopher mutex protecting meal count and last meal time
- `print_mutex`: Global mutex for synchronized output
- `stop_mutex`: Global mutex protecting simulation stop flag

**Concurrency Strategy:**
- Philosophers take forks in a specific order based on ID (even vs odd)
- Single philosopher edge case handled separately (no eating possible)
- Precise sleep using busy-wait + `usleep()` for accuracy
- Adaptive thinking time for odd philosopher counts to prevent starvation

**Signal Handling:**
- Uses `__atomic_exchange_n` with `__ATOMIC_RELAXED` ordering for lock-free signal handler protection
- Prevents multiple signal handler executions through atomic flag exchange
- Ensures clean shutdown without mutex deadlocks in signal context

### Bonus Part: Processes & Semaphores

**Architecture:**
- Each philosopher is a separate **process** (`fork()`)
- Forks are managed by a **named semaphore** (`/philo_forks`) with value = philosopher count
- Additional named semaphores for printing, stopping, and death detection
- Each process creates a **monitoring thread** to detect its own death using atomic operations
- **Adaptive staggered start**: Uses the same algorithm as mandatory part for consistent behavior across implementations

**Key Semaphores:**
- `/philo_forks`: Counting semaphore (value = number of forks available)
- `/philo_print`: Binary semaphore for synchronized printing
- `/philo_stop`: Binary semaphore protecting stop flag
- `/philo_death`: Binary semaphore ensuring only one death message

**Atomic Operations (`<stdatomic.h>`):**
- `atomic_init()`: Initialize atomic variables for time-to-live, monitor flags, and death status
- `atomic_store()`: Thread-safe writes to shared state (philosopher death, monitor stop, simulation interrupt)
- `atomic_load()`: Thread-safe reads from shared state in monitoring threads
- `__atomic_exchange_n()` with `__ATOMIC_SEQ_CST`: Stronger ordering for signal handlers in multi-process environment

**Advantages over threads:**
- Complete memory isolation between philosophers
- More realistic simulation of independent agents
- Practice with IPC (Inter-Process Communication)
- Lock-free monitoring using atomic operations reduces contention

## Resources

### Classic References

**Dining Philosophers Problem:**
- [Wikipedia: Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem) - Problem definition and classical solutions

**POSIX Threads (pthreads):**
- `man pthread_create(3)` - Thread creation
- `man pthread_join(3)` - Waiting for thread completion
- `man pthread_detach(3)` - Detaching threads
- `man pthread_mutex_init(3)` - Mutex initialization
- `man pthread_mutex_lock(3)` - Mutex locking
- `man pthread_mutex_unlock(3)` - Mutex unlocking
- `man pthread_mutex_destroy(3)` - Mutex destruction
- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/) - Comprehensive threading tutorial

**POSIX Semaphores:**
- `man sem_open(3)` - Named semaphore creation/opening
- `man sem_wait(3)` - Semaphore wait (decrement)
- `man sem_trywait(3)` - Non-blocking semaphore wait
- `man sem_post(3)` - Semaphore post (increment)
- `man sem_close(3)` - Close semaphore descriptor
- `man sem_unlink(3)` - Remove named semaphore
- [Semaphores in Linux](https://man7.org/linux/man-pages/man7/sem_overview.7.html) - Overview of semaphore types

**Atomic Operations:**
- `man stdatomic(7)` - C11 atomic operations
- [GCC Atomic Builtins](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html) - `__atomic_exchange_n` and related functions
- [Memory Order Documentation](https://en.cppreference.com/w/c/atomic/memory_order) - Understanding memory ordering semantics

**Timing & System Calls:**
- `man gettimeofday(2)` - Get current time with microsecond precision
- `man usleep(3)` - Microsecond sleep

**Debugging Tools:**
- `man valgrind(1)` - Valgrind command-line options
- [Valgrind Manual](https://valgrind.org/docs/manual/manual.html) - Memory error detection
- [Helgrind Manual](https://valgrind.org/docs/manual/hg-manual.html) - Thread error detection
- [DRD Manual](https://valgrind.org/docs/manual/drd-manual.html) - Data Race error detection

### AI Usage

AI tools (GitHub Copilot, ChatGPT) were used as a **thinking partner and debugging assistant** to discuss problems and approaches, but all final code, architecture decisions, and bug fixes were produced by the student after understanding the underlying concepts.

**Documentation & Understanding:**
- Explaining complex threading concepts (mutexes, race conditions, deadlocks)
- Clarifying differences between named and unnamed semaphores (even though only named semaphores were used in this implementation)
- Understanding atomic operations and memory ordering semantics
- Refining and structuring README.md documentation to accurately represent technical implementations and use the appropriate technical terminology for atomic operations and synchronization mechanisms

**Code Review**
- Identifying potential race conditions in critical sections
- Suggesting proper mutex placement and lock ordering
- Reviewing atomic operation usage for correctness

**Learning Resources:**
- Providing quick reference for pthread and semaphore function signatures
- Clarifying process vs thread memory models
- Explaining lock-free programming concepts

**Testing Assistance & Debugging:**
- Helping design test cases for edge scenarios
- Debugging edge cases (single philosopher, high philosopher counts)
- Identifying race conditions in signal handlers

## License

This project is part of the 42 Common Core curriculum. See [LICENSE](LICENSE) for details.