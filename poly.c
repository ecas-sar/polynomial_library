#include "poly.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>

int allocate_poly(polynomial_t *p, int poly_degree_plus_one)
{
    if (p == NULL || poly_degree_plus_one < 0)
    {
        return 1; // Fails as polynomials can't have degree less than 0, and need to be initialised to be allocated.
    }

    p->coefficients = malloc(poly_degree_plus_one*sizeof(double)); // Allocates an array of poly_degree_plus_one slots each of sizeof(double) bytes.
    if (p->coefficients == NULL)
    {
        return 1; // Fails if malloc fails.
    }

    p->poly_degree_plus_one = poly_degree_plus_one;

    return 0;
}

int free_poly(polynomial_t *p)
{
    if (p == NULL || p->coefficients == NULL)
    {
        return 1; // Can't free something that was never allocated.
    }

    free(p->coefficients);
    p->coefficients = NULL;
    p->poly_degree_plus_one = 0;

    return 0;
}

int initialise_poly_rand(polynomial_t *p, int poly_degree_plus_one, int val_min, int val_max)
{
    if (allocate_poly(p, poly_degree_plus_one) == 1)
    {
        return 1; // Stop if allocation fails to avoid a segfault.
    }

    for (int i = 0; i < p->poly_degree_plus_one; i++)
    {
        int random_coefficient = rand() % (val_max - val_min + 1) + val_min;
        p->coefficients[i] = random_coefficient;
    }
    // If the first coefficient in the polynomial is 0, this will waste space, so make sure this is never the case.
    if (p->coefficients[0] == 0)
    {
        p->coefficients++;
    }

    return 0;
}

char* string_representation(polynomial_t *p)
{
    if (p == NULL || p->coefficients == NULL || p->poly_degree_plus_one < 0)
    {
        return NULL;
    }

    char* str_poly = malloc(256); // Allocate a buffer.
    str_poly[0] = '\0';                                                                                                             
    char term[64];
    for (int i = 0; i < p->poly_degree_plus_one; i++)
    {
        // Stored current exponent in a variable for code readability.
        int current_exponent = p->poly_degree_plus_one-1-i;
        double current_coefficient = p->coefficients[i];
        // No point printing a 0 coefficient, it would simply be a waste of processing power!
        if (current_coefficient != 0)
        {
            if (current_exponent >= 2)
            {
                snprintf(term, sizeof(term), "%.2fx^%d + ", current_coefficient, current_exponent); // Generates a term in the polynomial.
            }
            else if (current_exponent == 1)
            {
                snprintf(term, sizeof(term), "%.2fx + ", current_coefficient);
            }
            else
            {
                snprintf(term, sizeof(term), "%.2f", current_coefficient);
            }
        }
        strcat(str_poly, term); // Concatentates terms to poly_str.
    }
    // White box testing.
    // printf("Polynomial: %s\n", str_poly);
    return str_poly;
}

double* zeroes(polynomial_t *p)
{
    // Variable naming for readability and efficiency;
    int poly_degree_plus_one = p->poly_degree_plus_one;
    int poly_degree = poly_degree_plus_one-1;
    double *zeroes_list = malloc((poly_degree)*sizeof(double));
    double *coefficients = p->coefficients;
    if (poly_degree_plus_one == 2)
    {
        double a = coefficients[0];
        double b = coefficients[1];
        zeroes_list[0] = -b/a;
    }
    else if (poly_degree_plus_one == 3)
    {
        double a = coefficients[0];
        double b = coefficients[1];
        double c = coefficients[2];
        double discriminant = b*b - 4.00*a*c;
        if (discriminant < 0)
        {
            printf("%s\n", "No real zeroes."); // If discriminant is negative, solutions will not be real.
            return NULL;
        }
        double numerator_one = -b + sqrt(discriminant);
        double numerator_two = -b - sqrt(discriminant);
        zeroes_list[0] = numerator_one/2*a;
        zeroes_list[1] = numerator_two/2*a;
    }
    else if (poly_degree_plus_one > 3)
    {
        printf("%s\n", "No efficient method to do this.");
        return NULL;
    }
    else 
    {  
        if (coefficients[0] != 0)
        {
            printf("%s\n", "No zeroes here."); // Constant functions don't have a 0 except when they are 0.
            return NULL;
        }
        else
        {
            printf("%s\n", "Entire real numbers!");
            return NULL; // Can't really add the entire real numbers to the list otherwise the CPU will crash.
        }
    }
    return zeroes_list;
}