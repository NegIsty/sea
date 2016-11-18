#include <stdio.h>
#include <unistd.h>

int main() {
    if(fork()==0) {
        write(1, "Je suis le fils\n", 17);
    } else {
        write(1, "Je suis le père\n", 18);
		wait();
    }
    
    return 0;
}

/*
Avec le wait() placé comme ci-dessus, le père s'affiche en premier.
On peut placer le wait avant le message du père pour forcer le fils à s'afficher en premier.
*/