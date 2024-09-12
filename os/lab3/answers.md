
## 1. What does the program print, and in what order?
It prints out all 3 print statements. It first prints out "waiting..."
since this in the main function before the pause() call. Then we wait for a
signal using pause(). When we recieve an interrupt signal, our handler runs,
and in prints "recieved an interrupt". List it prints out "time to exit"
after waiting for a second.

## 2. Describe exactly what is happening to produce the answer observed for the abve question.
Already answered above

## 3. 
Since fork() creates a replica process, the file descriptor table would be the same for the created child.
This means that if we set the standard output to a file before forking, both the parent and
child process would output to that file unless changed again.

## 4.
In this case, only the parent has the updated file descriptor table that redirects the standard output
into the file. The child still outputs to standard out. So the output just goes to standand out.

