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
        if (current_exponent >= 2)
        {
            snprintf(term, sizeof(term), "%.2fx^%d + ", p->coefficients[i], current_exponent); // Generates a term in the polynomial.
        }
        else if (current_exponent == 1)
        {
            snprintf(term, sizeof(term), "%.2fx + ", p->coefficients[i]);
        }
        else
        {
            snprintf(term, sizeof(term), "%.2f", p->coefficients[i]);
        }
        strcat(str_poly, term); // Concatentates terms to poly_str.
    }
    // White box testing.
    // printf("Polynomial: %s\n", str_poly);
    return str_poly;
}

double* zeroes(polynomial_t *p)
{
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
            printf("%s\n", "No real zeroes.");
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
        printf("%s\n", "No zeroes here.");
        return NULL;
    }
    return zeroes_list;
}