# CS-3113 Intro to Operating Systems: Project 1

Lucas Ho
02/25/2024

# Summary

In this project, the concept of shared memory and the problems that can occur if shared memory is not protected adequately are introduced. Four processes are created and each of these processes will share a variable called "total." The variable "total" will be incremented by one to 100000, 200000, 300000, and 500000 by its respective function "process#()"

The parent process released the shared memory and terminate after all the child processes are finished. Parent processes used the waitpid() function to know precisely when each child process finished. As the child process finishes execution, the parent process printed the process id of each child. Finally, it released the shared memory and printed "End of Simulation."

# How to Run Code

1. navigate to source folder in terminal
2. type "gcc project1.c"; compiles the a.out file
3. type "./a.out"; produces the output of the c file

# Results

From Process 1: counter = 103981.
Child with ID: 32435 has just exited.
From Process 2: counter = 241407.
Child with ID: 32436 has just exited.
From Process 3: counter = 376144.
Child with ID: 32437 has just exited.
From Process 4: counter = 629877.
Child with ID: 32438 has just exited.
End of Simulation.

From Process 1: counter = 112001.
Child with ID: 32474 has just exited.
From Process 2: counter = 217467.
Child with ID: 32475 has just exited.
From Process 3: counter = 349597.
Child with ID: 32476 has just exited.
From Process 4: counter = 604325.
Child with ID: 32477 has just exited.
End of Simulation.

From Process 1: counter = 103508.
From Process 2: counter = 252297.
Child with ID: 32500 has just exited.
Child with ID: 32501 has just exited.
From Process 3: counter = 551950.
Child with ID: 32502 has just exited.
From Process 4: counter = 872862.
Child with ID: 32503 has just exited.
End of Simulation.

From Process 1: counter = 100577.
Child with ID: 32524 has just exited.
From Process 2: counter = 267006.
Child with ID: 32525 has just exited.
From Process 3: counter = 431122.
Child with ID: 32526 has just exited.
From Process 4: counter = 707843.
Child with ID: 32527 has just exited.
End of Simulation.

From Process 1: counter = 101010.
Child with ID: 32562 has just exited.
From Process 2: counter = 236635.
Child with ID: 32563 has just exited.
From Process 3: counter = 374625.
Child with ID: 32564 has just exited.
From Process 4: counter = 645754.
Child with ID: 32565 has just exited.
End of Simulation.

# Conclusion

The intended incrementation of 100000, 200000, 300000, 500000 did not result in these values. It appears the counter variables always exceeded the increment value for each processes. With shared memory, there is still unpredictibility in the values returned. In one of the simulations, two child processes executed before the parent processes which brings attention to waitpid() function. All the parent processes did increment pid as expected which may indicates no issues the parent processes.
