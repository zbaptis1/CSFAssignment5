Name: Yisehak Ebrahim JHED: yebrahi1
Name: Zyan Baptiste JHED: zbaptis1

##Contributions:

Yisehak:
*MS1
- Focused on receiver functionality
- Dealt with connection::receive() and receiver() logic and
  how it responds to sender
- Added additional client_util functions
- Created google doc to document thinking & progress
*MS2
- Implementing server functionality 
- Working on message queue sections

Zyan:
*MS1
- Focused on sender functionality
- Focused on message utilization 
- Dealt with connection::send() and sender() logic and
  how it interacts with receiver
- In charge of most gradescope submissions
*MS2
- Chat with sender on server  
- Fine details with message queue and room
- ReadME report

Both:
- Worked on connection, basic functions to complex ones 
- Utilized debugger to fix logic errors 
- Utilized valgrind to fix mem. issues
- Worked on message functionality (especially split_payload())
- Went to countless TA OH hours and worked diligently
- Worked collaboratively and followed guidelines of prompt

##Report
*Implementation Report: Thread Synchronization 
When it came to identifying the critical sections, we decided to go based on if any of the changing
data structures would be accessed by multiple users at the same time at any point in time: This would 
be related to the following structures: the broadcast_message method in Room.cpp, UserSet in Room.h, and MessageQueue.
  
  - broadcast_message: Decided to add a guard due to currUser's m_queue being accessed. Without the guard,
  a failure or deadlock could occur if both broadcast and Receiver were trying to access the same queue at the same
  time. 

  - UserSet is accessed in find_or_create_room in Server.cpp. We used a Guard because the set should stay consistent 
  throughout its updates in the method, then unlocking when it was acceptable. 

  - MessageQueue: For dequeue, threads should have access to the MessageQueue while time-related functions were running.

We don't have any race conditions or deadlocks because every lock is always called in the same order and 
unlocked after a section or when the function that is called goes out of scope. This means that material won't 
be accessed before its been unlocked which would lead to a deadlock. 