#ifndef POLY_H
#define POLY_H

/* Polynomials will have the higher degree coefficients at the start of the list.
    E.g. [2, 4, 5] will represent 2x^2 + 4x + 5. Note about this: it might seem wasteful
    to have polynomial list such as [1, 0, 0, 3], but this is important to have. x^3 + 3 is
    different to x + 3 so we can't just remove groups of zeroes. */
typedef struct {
    double *coefficients;
    int poly_degree_plus_one; // Number of coefficients in the list, as if there are 3 coefficients then we have a 2nd degree polynomial.
} polynomial_t;


// Note: any functions that return int while appearing to 'do something' return 0 on success, 1 on failure.
int allocate_poly(polynomial_t *p, int poly_degree_plus_one);

int free_poly(polynomial_t *p);

// Good to have a random polynomial generator, this will make testing many different polynomials much easier.
int initialise_poly_rand(polynomial_t *p, int poly_degree_plus_one, int val_min, int val_max);

char* string_representation(polynomial_t *p);

// Will only be done for polynomials with degree <= 2 for now as there is usually not an efficient mathematical method to find 0s of high degree polynomials.
double* zeroes(polynomial_t *p);

// Will only be done for polynomials with degree <= 3 for similar reasoning to above.
double* turning_points(polynomial_t *p);

// This function is needed for sum_poly when the 2 polynomials being added have differing lengths.
int add_zeros_to_coeff_list(polynomial_t *p, int poly_degree_plus_one_new);

// Removes after sum is over in order to not waste space and hold inaccurate information about a poluynomials degree.
int remove_zeros_from_coeff_list(polynomial_t *p, int poly_degree_plus_one_new);

int poly_sum(polynomial_t *p1, polynomial_t *p2, polynomial_t *result);

int poly_scalar_product(polynomial_t *p, double scalar, polynomial_t *result);

int poly_derivative(polynomial_t *p, polynomial_t *result);

int poly_integral(polynomial_t *p, polynomial_t *result);

int read_poly_from_file(polynomial_t *p, char* input_file);

int write_poly_to_file(polynomial_t *p, char* output_file);

#endif