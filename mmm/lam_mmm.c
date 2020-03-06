#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <omp.h>

int main (int argc, char *argv[]) {
  int itemsPerDimension, repeats, counter, i, j, k, sum;
  unsigned long totalBytes; 
  struct timeval startTime, stopTime;
  double wallTimeForAll, wallTimeForOne, MatrixKBPerSecond; 
  int numThreads;
  int opt = 0;

   while ((opt = getopt(argc, argv, "d:r:t:")) != -1) {
        switch (opt) {
            case 'd':
                itemsPerDimension = strtoul(optarg, (char**) NULL, 10);
                break;

            case 'r':
                repeats = strtoul(optarg, (char**) NULL, 10);
                break;

            case 't':
                numThreads = strtoul(optarg, (char**) NULL, 10);
                break;

            case '?':
                fprintf(stderr, "usage: -d <itemsPerDimension> -r <repeats>  -t <threads>\n");
                exit(1);

            default:
                break;
        }
   }

  
    
  totalBytes = itemsPerDimension * itemsPerDimension * sizeof(int); 
  
  int (*mOne)[itemsPerDimension] = malloc(sizeof(int[itemsPerDimension][itemsPerDimension]));
  int (*mTwo)[itemsPerDimension] = malloc(sizeof(int[itemsPerDimension][itemsPerDimension]));
  int (*mResult)[itemsPerDimension] = malloc(sizeof(int[itemsPerDimension][itemsPerDimension]));

  if ((mOne == NULL) || (mTwo == NULL) || (mResult == NULL)) {
    perror("initial malloc() of mOne, mTwo, and/or mResult failed");
    exit(-1); }

  // initialize the three matrices 

#pragma omp parallel for schedule(static) private(i,j) num_threads(numThreads)
  for (i = 0; i < itemsPerDimension; i++) {
    for(j = 0; j < itemsPerDimension; j++) {
      mOne[i][j] = 333333;         
      mTwo[i][j] = 777777; 
      mResult[i][j] = 0; } }

	if (gettimeofday(&startTime, NULL) != 0) { 
		perror("gettimeofday() startTime failed");
		exit(-1); }

  // do the math repeats number of times. do not 
  for (counter = 0; counter < repeats; counter++) {
    sum = 0; 
#pragma omp parallel for schedule(static) private(i,j,k,sum) shared(mResult) num_threads(numThreads)

    for (i = 0; i < itemsPerDimension; i++) {
      for (j = 0; j < itemsPerDimension; j++) {
        for (k = 0; k < itemsPerDimension; k++) {
          sum = sum + mOne[i][k] * mTwo[k][j]; } 

        mResult[i][j] = sum;
        sum = 0; } } 

}
	if (gettimeofday(&stopTime, NULL) != 0) { 
		perror("gettimeofday() stopTime failed");
		exit(-1); }
  
	wallTimeForAll = (double)(stopTime.tv_sec - startTime.tv_sec) + 
  					       (double)((stopTime.tv_usec - startTime.tv_usec) * 
  					       (double)0.000001); 

  // factor out repeats, generate rate in MatrixItemsPerSecond
  wallTimeForOne = wallTimeForAll / (double)repeats; 
  MatrixKBPerSecond = ((double)totalBytes / (double)1024.0) / wallTimeForOne; 

  // display informations, for testing and debugging, enabled via gcc -DDISPLAY ...
  #ifdef DISPLAY 
    fprintf(stderr, "# itemsPerDimension: %d, repeats: %d, num_threads: %d\n", itemsPerDimension, repeats, numThreads); 
    for (i = 0; i < 2; i++) {
      for(j = 0; j < 2; j++) {
        fprintf(stderr, "mResult[%d][%d] = %d\n", i, j, mResult[i][j]); } }

    fprintf(stderr, "# totalBytes, wallTimeForAll, wallTimeForOne, MatrixKBPerSecond\n"); 
    fprintf(stderr," %lu, %.3lf, %.3lf, %.3lf\n", totalBytes, wallTimeForAll, wallTimeForOne, MatrixKBPerSecond);  
  #endif
  
    // print a tuple of thread number and time for pdf uses, for automated script
    printf("%d %.3lf", numThreads, wallTimeForAll);

  free(mOne); free(mTwo); free(mResult); 
  exit(0);
}
