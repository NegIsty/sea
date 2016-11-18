#include <stdio.h>

int main(int argc, char *argv[]) {
    fork();
    fork();
    fork();
    sleep(10);
    return 0;
}

/*
Le programme exécute 8 processus :
------o------o------o------1
      |      |      |------5
      |      |------o------3
      |             |------6
      |------o------o------2
             |      |------7
	         |------o------4
                    |------8

pstree confirme cette réponse :
ex11─┬─ex11─┬─ex11───ex11
     │      └─ex11
     ├─ex11───ex11
     └─ex11
*/