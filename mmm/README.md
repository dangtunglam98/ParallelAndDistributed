Before running automated script, make sure you have your server/machine:
- gcc
- python3
- matplotlib

Run: ./run.sh (chmod +x run.sh if permission denied)
Output: 2 pdf files for strong and weak scaling.

In case there's no executable file:
Compile without -DDISPLAY: gcc -Wall --pedantic -std=c99 -O0 lam_mmm.c -fopenmp -o lam_mmm (Run this before running ./run.sh if no executable file)

For debugging/display (not to run automated script):
compile with -DDISPLAY: gcc -DDISPLAY -Wall --pedantic -std=c99 -O0 lam_mmm.c -fopenmp -o lam_mmm 