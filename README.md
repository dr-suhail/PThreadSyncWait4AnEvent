# PThreadSyncWait4AnEvent
Process synchronization for “Wait for an event” using mutex, condition variable, and predicate variable of pthread library of C.

Program Design Description:
- Creates a mutex, a condition variable, and a predicate variable. These three will work together to synchronize occurrence of the required events. Initially the value of the predicate variable is set to 1.
- Create thread1. The thread1 acquires the mutex and checks the value of the predicate variable. If the value is 0 then it waits on condition variable until it is signaled. When it is signaled, it calls the display(“ab”) function and then sets the value of the predicate to 0. Then it signals the condition variable and releases the mutex.
- Create thread2. The thread2 acquires the mutex and checks the value of the predicate variable. If the value is 1 then it waits on condition variable until it is signaled. When it is signaled, it calls the display(“cd\n”) function and then sets the value of the predicate to 1. Then it signals the condition variable and releases the mutex.
- “main” thread will wait for thread1 and thread2 to finish execution
- Destroys the mutex

## Number of Processes/Threads and their Functions
The “synthread2” creates two threads. The thread1 and thread2. The thread1 displays the
message “ab” and thread2 displays the message “cd\n”.
