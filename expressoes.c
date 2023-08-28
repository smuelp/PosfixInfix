// Aluno: Samuel Ildebrando Pena (202219700140)

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define MAX 100

// modificação para ajustar o erro de arrendondamento
typedef struct {
    float data[MAX];
    int topo;
} Pilha;

void inicializarPilha(Pilha *pilha) {
    pilha -> topo = -1;
}

bool pilhaVazia(Pilha *pilha) {
    return pilha -> topo == -1;
}

bool pilhaCheia(Pilha *pilha) {
    return pilha -> topo == MAX - 1;
}

void empilhar(Pilha *pilha, float elemento) {
    if (!pilhaCheia(pilha)) {
        pilha -> data[++pilha -> topo] = elemento;
    }
}

float desempilhar(Pilha *pilha) {
    if (!pilhaVazia(pilha)) {
        return pilha -> data[pilha -> topo--];
    }
    return 0.0;
}

char topoDaPilha(Pilha *pilha) {
    if (!pilhaVazia(pilha)) {
        return pilha -> data[pilha -> topo];
    }
    return '\0';
}

int precedencia(char operador) {
    switch (operador) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        default:
            return 0;
    }
}

bool isOperador(char caractere) {
    return caractere == '+' || caractere == '-' || caractere == '*' || caractere == '/';
}

bool isDigitoDecimal(char caractere) {
    return caractere >= '0' && caractere <= '9';
}

void infixaParaPosfixa(const char *infixa, char *posfixa) {
    Pilha pilhaOperadores;
    inicializarPilha(&pilhaOperadores);

    int i = 0, j = 0;
    char atual;
    while ((atual = infixa[i]) != '\0') {
        if (isDigitoDecimal(atual)) {
            int k = i;
            while (isDigitoDecimal(infixa[k]) || infixa[k] == '.') {
                posfixa[j++] = infixa[k++];
            }
            posfixa[j++] = ' ';
            i = k - 1;
        } else if (atual == '.') {
            posfixa[j++] = atual;
        } else if (atual == '(') {
            empilhar(&pilhaOperadores, atual);
        } else if (atual == ')') {
            while (!pilhaVazia(&pilhaOperadores) && topoDaPilha(&pilhaOperadores) != '(') {
                posfixa[j++] = desempilhar(&pilhaOperadores);
            }
            desempilhar(&pilhaOperadores);
        } else if (isOperador(atual)) {
            while (!pilhaVazia(&pilhaOperadores) && precedencia(atual) <= precedencia(topoDaPilha(&pilhaOperadores))) {
                posfixa[j++] = desempilhar(&pilhaOperadores);
            }
            empilhar(&pilhaOperadores, atual);
        }
        i++;
    }
    while (!pilhaVazia(&pilhaOperadores)) {
        posfixa[j++] = desempilhar(&pilhaOperadores);
    }
    posfixa[j] = '\0';
}

float expressaoPosfixa(const char *posfixa) {
    Pilha pilhaOperandos;
    inicializarPilha(&pilhaOperandos);

    int i = 0;
    while (posfixa[i] != '\0') {
        char atual = posfixa[i];
        if (isDigitoDecimal(atual)) {
            float operando = 0;
            int contadorDecimais = 0;
            while (isDigitoDecimal(posfixa[i]) || posfixa[i] == '.') {
                if (posfixa[i] == '.') {
                    contadorDecimais++;
                } else {
                    if (contadorDecimais == 0) {
                        operando = operando * 10 + (posfixa[i] - '0');
                    } else {
                        operando = operando + (posfixa[i] - '0') / (10.0 * contadorDecimais);
                        contadorDecimais++;
                    }
                }
                i++;
            }
            empilhar(&pilhaOperandos, operando);
        } else if (atual == ' ') {
            // ignora os espacos
            i++;
        } else if (isOperador(atual)) {
            float operando2 = desempilhar(&pilhaOperandos);
            float operando1 = desempilhar(&pilhaOperandos);
            switch (atual) {
                case '+':
                    empilhar(&pilhaOperandos, operando1 + operando2);
                    break;
                case '-':
                    empilhar(&pilhaOperandos, operando1 - operando2);
                    break;
                case '*':
                    empilhar(&pilhaOperandos, operando1 * operando2);
                    break;
                case '/':
                    empilhar(&pilhaOperandos, operando1 / operando2);
                    break;
            }
            i++;
        }
    }
    return desempilhar(&pilhaOperandos);
}

void imprimirPosfixa(const char *posfixa) {
    int i = 0;
    while (posfixa[i] != '\0') {
        if (posfixa[i] != ' ') {
            printf("%c", posfixa[i]);
        }
        i++;
    }
}

int main() {
    char infixa[MAX], posfixa[MAX];

    scanf("%[^\n]%*c", infixa);

    infixaParaPosfixa(infixa, posfixa);

    printf("Infixa: %s\n", infixa);
    printf("Posfixa: "); 
    imprimirPosfixa(posfixa);
    printf("\n");

    printf("Resultado: %.2f\n", ceilf(expressaoPosfixa(posfixa) * 100) / 100);

    return 0;
}