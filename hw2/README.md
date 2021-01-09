# HW2 Short Answer Questions  
With each HW Assignment, there will be a few short answer questions for you to fill out.  
These questions will hopefully require you to reflect back on your experiences with the assignment.  
Please write at least a few sentences for each question.

## Question 1
##### In this assignment, you had to make substantial use of the data structures in C that you have built so far. As a user of these data-structures, what difficulties did you encounter? If you did not have many difficulties, describe anything notable you encountered while using them. Write >= 3 detailed sentences.
Our data structures often untilzing an output parameter other than return, which is quite unintutive for me so far. Also, sometime is diffcult to pass in the correct variable since they get mix up pretty quickly, due to the complexity of some function.


## Question 2
##### Creating the file system crawler, indexer, and search engine required working with C-strings (char*). What were a few (>=3) things you had to consider when working with char* compared to the use of a more properly defined String object that would be provided in other languages?
The first thing is whether i need to malloc for the char.
Second thing is char only allow me to read the "string" char by char since it is actually a char array ptr, but that also creates greater flexibility
Last thing is there is much less funtion for char* than string, so
I must work around this


## Question 3
##### In this homework, we made use of the POSIX API to read files, and crawl directories. List a couple (>= 2) plausible reasons as to why we used POSIX instead of the C standard library functions for this assignment. Be sure to explain your reasoning.
POSIX offers more function than C standard library since it is the super set
POSIX is unbuffered, so less resource is used


## Question 4
##### In searchshell.c you had to come up with ways of dealing with user input. What were some things (>= 2) you encountered when handling user input that were challenging? How does user input handling compare to other languages you have programmed in? (>=2 comparisons)
everything goes to much lower level compare to java.console, even breaking a string into token are much more difficult.

manually adding "\0" is also a big challenge since c does not have string and we have to terminate the "string" on our selves, which is normally handled by the language


