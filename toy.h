#ifndef TOY_H
#define TOY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Toy Post-Quantum Public-Key Cryptosystem Parameters
#define TK_K 3
#define TK_N 4
#define TK_Q 97
#define TK_W 22

// Helper macro to calculate the negative of a value modulo TK_Q
#define NEG(X) (TK_Q - (X))

// Function to fill an array with random small values
static void toy_fill_small(short* buf, int n);

// Naive polynomial multiplication
static void toy_polmul_naive(short* dst, const short* a, const short* b, int add);

// Matrix-vector multiplication
static void toy_mulmv(short* dst, const short* mat, const short* vec);

// Matrix transpose-vector multiplication
static void toy_mulmTv(short* dst, const short* mat, const short* vec);

// Dot product of two vectors
static void toy_dot(short* dst, const short* v1, const short* v2);

// Vector addition with modular arithmetic
static void toy_add(short* dst, const short* v1, const short* v2, int count, int v2_neg);

// Key generation
void toy_gen(short* A, short* t, short* s);

// Encryption
void toy_enc(const short* A, const short* t, int plain, short* u, short* v);

// Decryption
int toy_dec(const short* s, const short* u, const short* v);

#endif // TOY_H
