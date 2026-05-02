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

int add_zeros_to_coeff_list(polynomial_t *p, int poly_degree_plus_one_new)
{
    int p_degree_plus_one = p->poly_degree_plus_one;
    if (poly_degree_plus_one_new <= p_degree_plus_one)
    {
        return 1; // Logic of this fails if the new length is less than or equal to the current as we can't add 0s to it.
    }

    double* new_list = malloc(poly_degree_plus_one_new*sizeof(double));
    if (new_list == NULL)
    {
        return 1;
    }

    for (int i = 0; i < poly_degree_plus_one_new; i++)
    {
        int zeros_range = poly_degree_plus_one_new - p_degree_plus_one;
        if (i < zeros_range)
        {
            new_list[i] = 0;
        }
        else
        {
            new_list[i] = p->coefficients[i - zeros_range];
        }
    }
    free(p->coefficients);
    p->coefficients = new_list;
    p->poly_degree_plus_one = poly_degree_plus_one_new;
    return 0;
}

int remove_zeros_from_coeff_list(polynomial_t *p, int poly_degree_plus_one_new)
{
    int p_degree_plus_one = p->poly_degree_plus_one;
    if (poly_degree_plus_one_new > p_degree_plus_one)
    {
        return 1; // Opposite of above case, new degree must be smaller.
    }
    else if (poly_degree_plus_one_new == p_degree_plus_one)
    {
        return 0; // No work to be done if polynomials are of the same length.
    }

    double* new_list = malloc(poly_degree_plus_one_new*sizeof(double));
    for (int i = 0; i < p_degree_plus_one; i++)
    {
        if (p->coefficients[i] != 0)
        {
            new_list[i] = p->coefficients[i];
        }
    }

    p->coefficients = new_list;
    return 0;
}

int poly_sum(polynomial_t *p1, polynomial_t *p2, polynomial_t *result)
{
    int p1_degree_plus_one = p1->poly_degree_plus_one;
    int p2_degree_plus_one = p2->poly_degree_plus_one;
    int result_degree_plus_one = 0;
    // Set the result coeff_list length to be the max of the 2 input polynomials.
    if (p1_degree_plus_one > p2_degree_plus_one)
    {
        result_degree_plus_one = p1_degree_plus_one;
        // Adds zeros to the front of polynomial so that the shorter one can be added to the bigger one.
        // This way, if we have for example, x^2 + x + 1 and x^3 + 3x^2 + 2x + 1, then they will be represented as
        // [0, 2, 1, 1] and [1, 3, 2, 1], and adding them will end up with the cubed term being the same,
        // and the lower order terms being added.
        if (add_zeros_to_coeff_list(p2, p1_degree_plus_one) == 1)
        {
            return 1;
        }
    }
    else
    {
        result_degree_plus_one = p2_degree_plus_one;
        // Similar logic to here.
        if (add_zeros_to_coeff_list(p1, p2_degree_plus_one) == 1)
        {
            return 1;
        }
    }

    // If statement does run allocate_poly so does allocate the polynomial while simultaneously preventing errors if it fails, neat!
    if (allocate_poly(result, result_degree_plus_one) == 1)
    {
        return 1;
    }

    double *p1_coeff = p1->coefficients;
    double *p2_coeff = p2->coefficients;
    double *result_coeff = result->coefficients;
    for (int i = 0; i < result_degree_plus_one; i++)
    {
        result_coeff[i] = p1_coeff[i] + p2_coeff[i];
    }

    // Used for debugging.
    printf("%s\n", string_representation(result));

    // Removing unnecessary zeros from coefficient lists so that no space is wasted.
    if (p1_degree_plus_one > p2_degree_plus_one)
    {
        if (remove_zeros_from_coeff_list(p2, p2_degree_plus_one) == 1)
        {
            return 1;
        }
    }
    else if (p2_degree_plus_one > p1_degree_plus_one)
    {
        if (remove_zeros_from_coeff_list(p1, p1_degree_plus_one) == 1)
        {
            return 1;
        }
    }
    return 0;
}

int poly_scalar_product(polynomial_t *p, double scalar, polynomial_t *result)
{
    double *p_coefficients = p->coefficients;
    int p_degree_plus_one = p->poly_degree_plus_one;

    if (allocate_poly(result, p_degree_plus_one) == 1)
    {
        return 1;
    }

    double* result_coeff = result->coefficients;
    for (int i = 0; i < p_degree_plus_one; i++)
    {
        result_coeff[i] = scalar*p_coefficients[i];
    }
    printf("%s\n", string_representation(result));
    return 0;
}

int poly_derivative(polynomial_t *p, polynomial_t *result)
{
    int p_degree_plus_one = p->poly_degree_plus_one;
    int result_degree_plus_one = p_degree_plus_one-1; // By the power rule, the derivative of a polynomial is one degree less than that of the polynomial.

    if (allocate_poly(result, result_degree_plus_one) == 1)
    {
        return 1;
    }

    for (int i = 0; i < result_degree_plus_one; i++)
    {
        int current_exponent_p = result_degree_plus_one - i;
        result->coefficients[i] = p->coefficients[i]*current_exponent_p;
    }
    printf("%s\n", string_representation(result));
    return 0;
}

int poly_integral(polynomial_t *p, polynomial_t *result)
{
    int p_degree_plus_one = p->poly_degree_plus_one;
    int result_degree_plus_one = p_degree_plus_one+1; // By the power rule, the integral of a polynomial is one degree more than that of the polynomial.

    if (allocate_poly(result, result_degree_plus_one) == 1)
    {
        return 1;
    }

    for (int i = 0; i < result_degree_plus_one; i++)
    {
        int current_exponent_p = result_degree_plus_one-i;
        result->coefficients[i] = p->coefficients[i]/(current_exponent_p-1);
        if (i >= p_degree_plus_one)
        {
           result->coefficients[i] = rand() % (5 - 1 + 1) + 5;
        }
    }
    printf("%s\n", string_representation(result));
    return 0;
}

int write_poly_to_file(polynomial_t *p, char* output_file)
{
    char *string_poly = string_representation(p);
    if (string_poly == NULL)
    {
        return 1;
    }

    FILE *fptr;
    fptr = fopen(output_file, "a"); // a means I can append to a file (i.e. I can write to it without overwriting old content).
    if (fptr == NULL)
    {
        return 1;
    }
    fprintf(fptr, "%s\n", string_poly);
    fclose(fptr);
    return 0;
}