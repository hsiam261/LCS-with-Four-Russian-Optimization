# LCS-with-Four-Russian-Optimization

This code computes LCS between two strings using four-russian-optimization in O(n^2/lgn) time.

# Build and Test
Use ```make``` to build the four-russian-optimization.out executable and use ```make run-randomized-test``` to test against random datasets of size 100 to 1000. ```run-randomized-test``` also validates the output by comparing the size of the LCS with the size retrieved from the naive O(n^2) method and also checks whether the output LCS is actually a common subsequence of that size. 
