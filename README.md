# 🌟 Thread Pool in C: Efficient Task Execution with Multithreading

Welcome to an exciting journey into the world of **concurrent programming**! This project is a robust implementation of a **Thread Pool** in C, designed to handle multiple tasks efficiently using the power of multithreading. Whether you're working on a web server, data processing, or any task that demands concurrency, a thread pool is an essential tool in your software engineering arsenal.

---

## 📖 Introduction

### What is a Thread Pool?
A **Thread Pool** is a collection of pre-instantiated, reusable threads that can execute tasks from a queue. Instead of creating and destroying threads for each task (which is costly in terms of performance), we maintain a pool of threads ready to execute tasks as soon as they are submitted.

In this implementation:
- We create a pool of threads (worker threads) that wait for jobs in a queue.
- The queue stores jobs (function pointers) that are processed one by one.
- When a job is added, one of the available threads picks it up and executes it, keeping the system efficient and avoiding the overhead of thread creation/destruction.

---

## 🛠️ Structure

Here's how the project is structured:

- **Thread Pool**: A manager that oversees task execution.
- **Queue**: A thread-safe structure that stores pending tasks.
- **Worker Threads**: A set of reusable threads that execute tasks from the queue.
- **Task Submission**: Users submit tasks, and worker threads pick them up for execution.

---

## 🚀 Code Walkthrough

Let's dive into the code and understand the key components that make this system work like a well-oiled machine.

### 🔧 Data Structures

- **node_t**: Represents a single job in the task queue.
  ```c
  typedef struct node_t {
      node_t *next;
      job curr_job;
  } node_t;
