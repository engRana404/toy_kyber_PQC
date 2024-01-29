#include "toy.h"

// Function to fill an array with random small values
static void toy_fill_small(short* buf, int n)
{
    // Use a small binomial distribution with Hamming weight
    for (int k = 0; k < n; ++k)
    {
        short val = rand();
        val = (val >> 1 & 1) - (val & 1); // Small binomial distribution using Hamming weight
        if (val < 0)
            val += TK_Q;
        buf[k] = val;
    }
}

// Naive polynomial multiplication
static void toy_polmul_naive(short* dst, const short* a, const short* b, int add)
{
    dst[0] = ((dst[0] & -add) + a[0] * b[0] + NEG(a[3]) * b[1] + NEG(a[2]) * b[2] + NEG(a[1]) * b[3]) % TK_Q;
    dst[1] = ((dst[1] & -add) + a[1] * b[0] + a[0] * b[1] + NEG(a[3]) * b[2] + NEG(a[2]) * b[3]) % TK_Q;
    dst[2] = ((dst[2] & -add) + a[2] * b[0] + a[1] * b[1] + a[0] * b[2] + NEG(a[3]) * b[3]) % TK_Q;
    dst[3] = ((dst[3] & -add) + a[3] * b[0] + a[2] * b[1] + a[1] * b[2] + a[0] * b[3]) % TK_Q;
}

// Matrix-vector multiplication
static void toy_mulmv(short* dst, const short* mat, const short* vec)
{
    memset(dst, 0, TK_K * TK_N * sizeof(short));

    for (int kv = 0, idx = 0; kv < TK_K * TK_N; kv += TK_N)
    {
        for (int k = 0; k < TK_K * TK_N; k += TK_N, idx += TK_N)
            toy_polmul_naive(dst + kv, mat + idx, vec + k, 1);
    }
}

// Matrix transpose-vector multiplication
static void toy_mulmTv(short* dst, const short* mat, const short* vec)
{
    memset(dst, 0, TK_K * TK_N * sizeof(short));

    for (int kv = 0; kv < TK_K * TK_N; kv += TK_N)
    {
        for (int k = 0; k < TK_K * TK_N; k += TK_N)
            toy_polmul_naive(dst + kv, mat + TK_K * k + kv, vec + k, 1);
    }
}

// Dot product of two vectors
static void toy_dot(short* dst, const short* v1, const short* v2)
{
    memset(dst, 0, TK_N * sizeof(short));

    for (int k = 0; k < TK_K * TK_N; k += TK_N)
        toy_polmul_naive(dst, v1 + k, v2 + k, 1);
}

// Vector addition with modular arithmetic
static void toy_add(short* dst, const short* v1, const short* v2, int count, int v2_neg)
{
    for (int k = 0; k < count; ++k)
    {
        short val = v2[k];
        if (v2_neg)
            val = NEG(val);
        dst[k] = (v1[k] + val) % TK_Q;
    }
}

// Key generation
void toy_gen(short* A, short* t, short* s)
{
    short e[TK_K * TK_N];

    // Generate random values for public and secret keys
    for (int k = 0; k < TK_K * TK_K * TK_N; ++k)
        A[k] = rand() % TK_Q;

    toy_fill_small(s, TK_K * TK_N);
    toy_fill_small(e, TK_K * TK_N);

    // t = A.s + e
    toy_mulmv(t, A, s);
    toy_add(t, t, e, TK_K * TK_N, 0);
}

// Encryption
void toy_enc(const short* A, const short* t, int plain, short* u, short* v)
{
    short r[TK_K * TK_N], e1[TK_K * TK_N], e2[TK_N];

    toy_fill_small(r, TK_K * TK_N);
    toy_fill_small(e1, TK_K * TK_N);
    toy_fill_small(e2, TK_N);

    // u = A^T . r + e1
    toy_mulmTv(u, A, r);
    toy_add(u, u, e1, TK_K * TK_N, 0);

    // v = t^T . r + e2 + plain + q/2
    toy_dot(v, t, r);
    toy_add(v, v, e2, TK_N, 0);

    for (int k = 0; k < TK_N; ++k)
        v[k] = (v[k] + ((TK_Q >> 1) & -(plain >> (TK_N - 1 - k) & 1))) % TK_Q;
}

// Decryption
int toy_dec(const short* s, const short* u, const short* v)
{
    short p[TK_N];
    int plain;

    // Compute dot product
    toy_dot(p, s, u);
    // Compute plaintext
    toy_add(p, v, p, TK_N, 1);

    plain = 0;
    for (int k = 0; k < TK_N; ++k)
    {
        int val = p[k];
        if (val > TK_Q / 2)
            val -= TK_Q;
        printf(" %5d ", val);
        int bit = abs(val) > TK_Q / 4;
        plain |= bit << (TK_N - 1 - k);
    }

    return plain;
}
