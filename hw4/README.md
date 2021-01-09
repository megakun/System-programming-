# HW3 Short Answer Questions  
With each HW Assignment, there will be a few short answer questions for you to fill out.  
These questions will hopefully require you to reflect back on your experiences with the assignment.  
Please write at least a few sentences for each question.

## Question 1
##### This homework had you write a program that worked very closely with the network. List a few (>=2) things that you had to account for when writing code that directly interacted with the network. (Interactions could include reading/writing to the network, managing connections, and setting up the network server in ServerSocket.cc)
One major thing would be setting the request properly, such as socket type.
Another thing is most read or write activity should be wrapped to handle error. Coverting values from host to network format or the other way around is also important


## Question 2
##### In this homework, you wrote a program that worked with concurrency. List a few (>=2) notable things that you encountered when writing code or read provided code that took advantage of concurrency or was run concurrently.
Concurrency requires handling lock carefully, using lock and unlock properly is very important, otherwise deadlock might occur
Dispatching task to different thread is also very essential when utilizing concurrency



## Question 3
##### Why do we try to read many bytes at a time with WrappedRead when we call HttpConnection::GetNextRequest? Please explain your reasoning. 
The request could be very long, hence reading a lot of bytes is more efficient than byte by byte since less system call is require


