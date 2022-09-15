#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int produto = 1;

int fatorial(int, int);

int fatorial(int start, int n) {
  int produto_ = 1;
  
  for (int i = start; i <= n; i++) {
    produto_ *= i;
  }

  return produto_;
}

int main(void) {
    siginfo_t infop;
    pid_t primeiroFilho, segundoFilho;
    __id_t fSon = primeiroFilho, sSon_ = segundoFilho;
    int fd[2], n, fStatus, sStatus;
    int meio, fator;

    printf("%s", "Digite um numero: ");
    scanf("%d", &n);

    if (n < 2) {
        printf("\nFatorial de %d e %s\n", n, "1");
        return EXIT_SUCCESS;
    }
    if (pipe(fd) < 0) {
        fprintf(stderr, "\nerro de pipe");
        exit(EXIT_FAILURE);
    }
    else if ((primeiroFilho = fork()) < 0 || (segundoFilho = fork()) < 0) {
        fprintf(stderr, "\n eroo de fork\n");
        exit(EXIT_FAILURE);
    }
    else if (primeiroFilho == 0) {
        meio = n / 2;
        fator = fatorial(1, meio);
        printf("\n%s: %d", "Resultado do primeiro processo", fator);
        close(fd[0]);
        write(fd[1], &fator, sizeof(int));
    }
    else if (segundoFilho == 0) {
        meio = n / 2 + 1;
        fator = fatorial(meio, n);
        printf("\n%s: %d", "Resultado do segundo processo", fator);
        close(fd[0]);
        write(fd[1], &fator, sizeof(int));
    }
    else {
        waitid(P_PID, fSon, &infop, WEXITED);
        close(fd[1]);
        read(fd[0], &fator, sizeof(int));
        produto *= fator;
        waitid(P_PID, sSon_, &infop, WEXITED);
        close(fd[1]);
        read(fd[0], &fator, sizeof(int));
        produto *= fator;
        printf("\nFatorial de %d Ã© %d\n", n, produto);
    }

    return EXIT_SUCCESS;
}
