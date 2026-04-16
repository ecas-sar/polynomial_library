#ifndef POLY_H
#define POLY_H

// Polynomials will have the higher degree coefficients at the start of the list.
// E.g. [2, 4, 5] will represent 2x^2 + 4x + 5.
typedef struct {
    double *coefficients;
    int poly_degree_plus_one; // Number of coefficients in the list, as if there are 2 coefficients then we have a 2nd order polynomial_t.
} polynomial_t;

// Note: any functions that return int while appearing to 'do something' return 0 on success, 1 on failure.
int allocate_poly(polynomial_t *p, int poly_degree_plus_one);

int free_poly(polynomial_t *p);

char* string_representation(polynomial_t *p);

// Will only be done for polynomial_ts with degree <= 2 for now as there is usually not an efficient mathematical method to find 0s of high degree polynomial_ts,
int* zeroes(polynomial_t *p);

int* turning_points(polynomial_t *p);

int poly_sum(polynomial_t *p1, polynomial_t *p2, polynomial_t *result);

int poly_scalar_product(polynomial_t *p, double scalar, polynomial_t *result);

int poly_derivative(polynomial_t *p, polynomial_t *result);

int poly_integral(polynomial_t *p, polynomial_t *result);

int read_poly_from_file(polynomial_t *p, char* input_file);

int write_poly_to_file(polynomial_t *p, char* output_file);

#endif