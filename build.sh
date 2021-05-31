gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o command.o src/command.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o exec.o src/exec.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o main.o src/main.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o sighandler.o src/sighandler.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o strutils.o src/strutils.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o strvec.o src/strvec.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o userdata.o src/userdata.c
gcc -c -pipe -O2 -Wall -Wextra -fPIC  -I. -o jobs.o src/jobs.c
gcc -Wl,-O1 -o fvgsh command.o exec.o main.o sighandler.o strutils.o strvec.o userdata.o jobs.o
rm *.o