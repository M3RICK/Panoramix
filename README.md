# Panoramix - Thread Synchronization Project

A multi-threaded C program simulating the famous Gallic village from Asterix, demonstrating advanced concurrency concepts through the implementation of the magic potion distribution system.

## 🏛️ Context

The year is 50 BC. Gaul is entirely occupied by the Romans... Well, not entirely! One small village of indomitable Gauls still holds out against the invaders. In this simulation, villagers drink magic potion from a communal pot to gain strength for fighting Roman soldiers. When the pot runs empty, they wake the druid Panoramix to refill it.

## 🎯 Features

- Multi-threaded simulation with individual threads for each villager and the druid
- Thread synchronization using mutexes and semaphores
- Realistic simulation of resource sharing and producer-consumer patterns
- Graceful program termination when resources are exhausted
- Configurable simulation parameters

## 🔧 Prerequisites

- GCC compiler
- POSIX threads library (pthread)
- Make build tool
- Linux/Unix environment

## 📦 Installation

```bash
# Clone the repository
git clone https://github.com/yourusername/panoramix.git
cd panoramix

# Build the project
make

# Clean build files
make clean

# Full clean (including binary)
make fclean

# Rebuild
make re
```

## 🚀 Usage

```bash
./panoramix <nb_villagers> <pot_size> <nb_fights> <nb_refills>
```

### Parameters

- `nb_villagers`: Number of villagers in the simulation (must be > 0)
- `pot_size`: Maximum servings the magic potion pot can hold (must be > 0)
- `nb_fights`: Maximum number of fights each villager will engage in (must be > 0)
- `nb_refills`: Maximum number of times the druid can refill the pot (must be > 0)

### Example

```bash
./panoramix 3 5 3 1
```

This runs a simulation with:
- 3 villagers
- A pot that holds 5 servings
- Each villager fights 3 times
- The druid can refill the pot once

## 🔄 Thread Synchronization

The program demonstrates several key concurrency concepts:

### 🧙‍♂️ Druid Thread
- Waits to be awakened by villagers when the pot is empty
- Refills the pot with `pot_size` servings
- Terminates when ingredients are exhausted (`nb_refills` reached)
- Uses semaphores to coordinate with villagers

### 🗡️ Villager Threads
- Each villager runs in its own thread
- Must drink potion before each fight
- Checks pot availability using mutex protection
- Wakes the druid when the pot is empty
- Terminates after completing all fights

### 🔒 Synchronization Mechanisms
- **Mutexes**: Protect shared resources (the pot)
- **Semaphores**: Coordinate druid awakening and pot refilling
- **Condition handling**: Prevents race conditions and deadlocks

## 📋 Output Messages

The program outputs specific messages for each event:

### Villager Messages
```
Villager <id>: Going into battle!
Villager <id>: I need a drink... I see <nb_servings_left> servings left.
Villager <id>: Hey Pano wake up! We need more potion.
Villager <id>: Take that roman scum! Only <fights> left.
Villager <id>: I'm going to sleep now.
```

### Druid Messages
```
Druid: I'm ready... but sleepy...
Druid: Ah! Yes, yes, I'm awake! Working on it! Beware I can only make <refills> more refills after this one.
Druid: I'm out of viscum. I'm going back to... zZz
```

## ⚠️ Edge Cases

The program handles several important scenarios:
- Villagers waiting when the pot is empty
- Proper termination when the druid runs out of ingredients
- Graceful exit when all villagers complete their fights
- Prevention of endless waiting conditions

## 🏗️ Technical Details

- Language: C
- Compilation: Via Makefile
- Binary name: `panoramix`
- Thread model: POSIX threads
- Synchronization: Mutexes and semaphores

## 📊 Example Output

```bash
~/B-CCP-400> ./panoramix 3 5 3 1
Druid: I'm ready... but sleepy...
Villager 2: Going into battle!
Villager 1: Going into battle!
Villager 0: Going into battle!
Villager 2: I need a drink... I see 5 servings left.
Villager 0: I need a drink... I see 4 servings left.
[...]
Druid: I'm out of viscum. I'm going back to... zZz
Villager 0: I'm going to sleep now.
Villager 1: I'm going to sleep now.
Villager 2: I'm going to sleep now.
```

## 🤝 Contributing

This is an educational project from EPITECH. Please follow the school's guidelines for academic integrity.

## 📄 License

This project is part of the EPITECH curriculum. All rights reserved.

*Note: The output order may vary due to thread asynchronicity, but all synchronization rules are strictly followed.*
