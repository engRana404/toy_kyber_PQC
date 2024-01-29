#include "toy.h"

int main() {

    // Allocate memory for keys and other variables
    short A[TK_K * TK_K * TK_N];
    short t[TK_K * TK_N];
    short s[TK_K * TK_N];
    short u[TK_K * TK_N];
    short v[TK_N];

    // Generate keys
    toy_gen(A, t, s);

    // Print generated keys
    printf("Public Key A:\n");
    for (int i = 0; i < TK_K * TK_K * TK_N; ++i) {
        printf("%5d ", A[i]);
        if ((i + 1) % TK_N == 0) printf("\n");
    }

    printf("\nSecret Key s:\n");
    for (int i = 0; i < TK_K * TK_N; ++i) {
        printf("%5d ", s[i]);
        if ((i + 1) % TK_N == 0) printf("\n");
    }

    
    int plaintext = 5;  // Change the plaintext value as needed
    printf("\nPlaintext: %d\n", plaintext);

    // Encrypt a message
    toy_enc(A, t, plaintext, u, v);

    // Print encrypted message
    printf("\nEncrypted Message:\n");
    printf("u:\n");
    for (int i = 0; i < TK_K * TK_N; ++i) {
        printf("%5d ", u[i]);
        if ((i + 1) % TK_N == 0) printf("\n");
    }

    printf("\nv:\n");
    for (int i = 0; i < TK_N; ++i) {
        printf("%5d ", v[i]);
    }
    printf("\n");

    // Decrypt the message
    int decrypted = toy_dec(s, u, v);

    // Print decrypted message
    printf("\nDecrypted Message: %d\n", decrypted);

    // Verify that 22^4 mod 97 is 1
    int w_pow_4_mod_q = 1;
    for (int i = 0; i < 4; ++i) {
        w_pow_4_mod_q = (w_pow_4_mod_q * TK_W) % TK_Q;
    }

    printf("22^4 mod 97 is: %d\n", w_pow_4_mod_q);  // Expected: 1

    // Verify that 33^2 mod 97 is 22
    int sqrt_of_TK_W = sqrt(TK_W * TK_W);
    printf("sqrt(22^2) is: %d\n", sqrt_of_TK_W);  // Expected: 22

    int sqrt_w_sq_mod_q = sqrt_of_TK_W % TK_Q;
    printf("33^2 mod 97 is: %d\n", sqrt_w_sq_mod_q);  // Expected: 22

    return 0;
}
