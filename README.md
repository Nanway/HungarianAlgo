# HungarianAlgo

To run this yourself use the make file and the command to run is ./scaledFootRule File1 File2 ...
To check your results, see the last paragraph (needs python + Selenium + Chromium)

The Hungarian Algorithm is an algorithm which solves the assignment problem. In essence this is a problem where you have N amount of objects that need to be assigned to N amount of spots.
However, the cost of assigning each object to each spot differs and we seek to find an optimal matching such that the overall 'cost' is minimised. For example, lets say we have workers for jobs and each worker has a different price for each job. 
We want to assign each worker to a job in such a way that the overall cost is minimised.

In my implementation of it I take in text files which have strings that are listed in order of their rank (strings at the top of the file have higher rank than ones at the bottom).
My algorithm takes in these files and finds the way to best rank these strings. Here, the scaled footrule is used as a means of quantifying the cost to assign each string to each position. An example usage of this would be several people ranking the same set of foods, this algorithm can then find the best overall way to rank this set of foods

This was done as an assignment for a DS&A course (where the context was ranking a set of search results from potentially different search engines) and in this I use the following data structures that had to be implemented ourselves (with help from university lab class code):
- Bipartite Graph
- Set implemented as a Binary Search Tree
- Array List 

I read in the files into an array of array lists as well as a BST. I use the BST so that I can quickly and simply create a unique set of URLs.
I use the array of array lists to represent the ordering of each URL in each of the files. It is an array list as I am unsure how many URLs there 
would be in each file. Using this set of URLs and this array of array lists I have enough information to create my bipartite graph. I then initialise the weights using the scaled footrule
and then apply the algorithm (which is explained in the comments inside the files)

Inside the lines you can uncomment one of the lines to print out a weight matrix representing the cost from each URL to each position.
If you copy this into the input.txt file and then run checkResults.py this will use Selenium to open up your browser and enter these 
weights into an online hungarian algorithm solver to check the validity of my implementation of it.

