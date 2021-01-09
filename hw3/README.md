# HW3 Short Answer Questions  
With each HW Assignment, there will be a few short answer questions for you to fill out.  
These questions will hopefully require you to reflect back on your experiences with the assignment.  
Please write at least a few sentences for each question.

## Question 1
##### HW3 had you writing the in-memory structure from HW2 into a file on disk. What are some (>=2) advantages of storing this structure to a file? What is a disadvantage?
Advantages would be save up memory space since larger file might take up too much space, and some program misusing memory might cause memory to be containminated, another one would the index is now portable to other device since it is in a file instead of some memory fixed to this machine
One disadvantage would be it takes very long to process files since writing into disk is mucher slower than memory. Another one would be implementing such program is much more complicated, also means more likelyhood to cause some error(or this might just be my disadvantage)


## Question 2
##### At this point you have written some very similar non-trivial programs in C and C++. In HW2 you implemented searchshell.c and Memindex.c, while in this HW 3 you implemented the similar programs QueryProcessor.cc and filesearchshell.cc. List (>=2) notable differences in the experience you had writing in C vs in C++.
One major difference would C++ has more libaray than C, which means I can rely more on existng function than building one on my own. Features like auto is very handy. 
However, this also lead to another difference, I have to look after more stuff while programming, especially i think casting and converting varibale is more complicated in C++



## Question 3
##### Note that in FileIndexReader.cc STEP 1, we set it so that reading/writing to the index file was unbuffered. We do this so that we can use our own strategy of reading the "in-file" HashTable, and avoid the default buffering strategy provided by the C standard library. Why would turning off the buffer be more efficient than leaving it on in our use case of reading files?  
###### (Recall that if you were to read the first byte of a file with fread and buffering on, it would instead read the first many (likely >= 512) bytes and store it in an internal buffer before returning the one byte you requested. This is so that if you want to read the next byte in the file, fread() doesn't have to go to the OS and disk to read it, instead just returning the bytes it already read).
This is because so that we can minimize the usage of buffer, which is very valuable. The only time we use a large(512) buffer is to form checksum. I think miinimizing usage of memory would be to reduce the 
space complexity of our program.


