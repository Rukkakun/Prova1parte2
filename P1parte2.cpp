#include <stdio.h>
#include <stdlib.h>

int mdc(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int sao_primos_entre_si(int a, int b) {
    return mdc(a, b) == 1;
}

int eh_primo(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

int inverso_modular(int a, int n) {
    int t = 0, novo_t = 1;
    int r = n, novo_r = a;

    printf("Calculando inverso modular de %d modulo %d:\n", a, n);
    while (novo_r != 0) {
        int quoc = r / novo_r;
        int temp = novo_t;
        novo_t = t - quoc * novo_t;
        t = temp;

        temp = novo_r;
        novo_r = r - quoc * novo_r;
        r = temp;

        printf("Quociente: %d | r: %d | novo_r: %d | t: %d | novo_t: %d\n", quoc, r, novo_r, t, novo_t);
    }

    if (r > 1) {
        printf("Nao existe inverso modular pois mdc(%d, %d) = %d > 1\n", a, n, r);
        return -1;
    }
    if (t < 0) t += n;

    printf("Inverso modular eh %d\n", t);
    return t;
}

int potencia_modular(int base, int exp, int mod) {
    int resultado = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            resultado = (resultado * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return resultado;
}

int phi(int n) {
    int resultado = n;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            resultado -= resultado / i;
        }
    }
    if (n > 1)
        resultado -= resultado / n;
    return resultado;
}

int calcula_base_a(int H, int G, int n) {
    printf("Verificando se G = %d e n = %d sao primos entre si\n", G, n);
    if (!sao_primos_entre_si(G, n)) {
        printf("G e n nao sao primos entre si.\n");
        exit(1);
    }
    printf("Sao primos entre si.\n");

    int G_inv = inverso_modular(G, n);
    if (G_inv == -1) {
        printf("Inverso modular de G nao existe.\n");
        exit(1);
    }

    int a = (H * G_inv) % n;
    printf("a = (%d * %d) mod %d = %d\n", H, G_inv, n, a);
    return a;
}

int calcula_x1(int a, int n1) {
    printf("Verificando se a = %d e n1 = %d sao primos entre si\n", a, n1);
    if (!sao_primos_entre_si(a, n1))
        printf("nao sao primos entre si.\n");
    else
        printf("Sao primos entre si.\n");

    if (eh_primo(n1)) {
        printf("n1 = %d eh primo, entao x1 = n1 - 1 = %d\n", n1, n1 - 1);
        return n1 - 1;
    } else {
        int phi_n1 = phi(n1);
        printf("n1 = %d nao eh primo, phi(%d) = %d\n", n1, n1, phi_n1);
        return phi_n1;
    }
}

int calcular_resultado_final(int a, int x, int x1, int n1) {
    int q = x / x1;
    int r = x % x1;
    printf("Decompondo x = %d: x = %d * %d + %d\n", x, x1, q, r);

    int x2 = potencia_modular(a, x1, n1);       
    int x2_q = potencia_modular(x2, q, n1);
    int a_r = potencia_modular(a, r, n1);
    printf("x2 = a^x1 mod n1 = %d^%d mod %d = %d\n", a, x1, n1, x2);
    printf("x2^q mod n1 = %d^%d mod %d = %d\n", x2, q, n1, x2_q);
    printf("a^r mod n1 = %d^%d mod %d = %d\n", a, r, n1, a_r);

    int resultado = (x2_q * a_r) % n1;
    printf("Resultado final = ((%d^%d) * %d) mod %d = %d\n", x2, q, a_r, n1, resultado);
    return resultado;
}

int main() {
    int H, G, n, x, n1;

    printf("Digite H, G e n (para Zn): ");
    scanf("%d %d %d", &H, &G, &n);

    printf("Digite o expoente x: ");
    scanf("%d", &x);

    printf("Digite o modulo n1: ");
    scanf("%d", &n1);

    int a = calcula_base_a(H, G, n);
    int x1 = calcula_x1(a, n1);
    int resultado = calcular_resultado_final(a, x, x1, n1);

    return 0;
}
