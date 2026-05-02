#include "poly.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>

void allocate_and_free_test(void)
{
    polynomial_t p;
    int poly_degree_plus_one = 3;
    p.coefficients = NULL;

    // Test polynomial is allocated successfully.
    CU_ASSERT(allocate_poly(&p, poly_degree_plus_one) == 0);

    // Test polynomial member variables are what they should be.
    CU_ASSERT(p.coefficients != NULL);
    CU_ASSERT(p.poly_degree_plus_one == 3);

    CU_ASSERT(free_poly(&p) == 0);
}

void string_repr_test(void)
{
    polynomial_t p;
    int poly_degree_plus_one = 3;
    allocate_poly(&p, poly_degree_plus_one);
    p.coefficients[0] = 1;
    p.coefficients[1] = 3;
    p.coefficients[2] = 5;

    char what_it_should_be[] = "1.00x^2 + 3.00x + 5.00";
    CU_ASSERT(strcmp(string_representation(&p), what_it_should_be) == 0);
}

bool two_arrays_equal(double* arr1, double* arr2, int len1, int len2)
{
    if (len1 != len2 || arr1 == NULL || arr2 == NULL)
    {
        return false; // If the arrays have different lengths, they are clearly not equal.
    }
    for (int i = 0; i < len1; i++)
    {
        if (arr1[i] != arr2[i])
        {
            return false; // If one element is different, the arrays are different.
        }
    }
    return true;
}

void initialise_polys(void)
{
    int num_trials = 10;
    int poly_degree_plus_one = 3;
    for (int i = 0; i < num_trials; i++)
    {
        polynomial_t p;
        CU_ASSERT(initialise_poly_rand(&p, poly_degree_plus_one, 1, 5) == 0);       
    }
}

void test_zeroes(void)
{
    polynomial_t p;
    int poly_degree_plus_one = 3;
    int poly_degree = poly_degree_plus_one-1;
    allocate_poly(&p, poly_degree_plus_one);
    p.coefficients[0] = 1;
    p.coefficients[1] = -2;
    p.coefficients[2] = -3;

    double* expected_zeroes = malloc(poly_degree*sizeof(double));
    expected_zeroes[0] = 3.00;
    expected_zeroes[1] = -1.00;
    CU_ASSERT(two_arrays_equal(expected_zeroes, zeroes(&p), poly_degree, poly_degree) == true);
}

void test_sum(void)
{
    int num_sums = 5;
    int poly1_degree_plus_one = 3;
    int poly2_degree_plus_one = 5;
    for (int i = 0; i < num_sums; i++)
    {
        polynomial_t p1;
        polynomial_t p2;
        polynomial_t result;
        initialise_poly_rand(&p1, poly1_degree_plus_one, 1, 5);
        initialise_poly_rand(&p2, poly2_degree_plus_one, 1, 5);
        printf("%s\n", string_representation(&p1));
        printf("%s\n", string_representation(&p2));
        CU_ASSERT(poly_sum(&p1, &p2, &result) == 0);
    }
}

void test_scalar_product(void)
{
    int num_products = 5;
    int poly_degree_plus_one = 4;
    for (int i = 0; i < num_products; i++)
    {
        polynomial_t p;
        polynomial_t result;
        double scalar = rand() % (10 - 1 + 1) + 1;
        initialise_poly_rand(&p, poly_degree_plus_one, 1, 8);
        printf("%s\n", string_representation(&p));
        CU_ASSERT(poly_scalar_product(&p, scalar, &result) == 0);
    }
}

void test_derivative(void)
{
    int num_derivs = 5;
    int poly_degree_plus_one = 4;
    for (int i = 0; i < num_derivs; i++)
    {
        polynomial_t p;
        polynomial_t result;
        initialise_poly_rand(&p, poly_degree_plus_one, 1, 5);
        printf("%s\n", string_representation(&p));
        CU_ASSERT(poly_derivative(&p, &result) == 0);
    }
}

void test_integral(void)
{
    int num_integrals = 5;
    int poly_degree_plus_one = 4;
    for (int i = 0; i < num_integrals; i++)
    {
        polynomial_t p;
        polynomial_t result;
        initialise_poly_rand(&p, poly_degree_plus_one, 1, 5);
        printf("%s\n", string_representation(&p));
        CU_ASSERT(poly_integral(&p, &result) == 0);
    }
}

void test_write_to(void)
{
    int num_polys_to_write = 5;
    int poly_degree_plus_one = 4;
    for (int i = 0; i < num_polys_to_write; i++)
    {
        polynomial_t p;
        initialise_poly_rand(&p, poly_degree_plus_one, 1, 5);
        CU_ASSERT(write_poly_to_file(&p, "write_to") == 0);
    }
}

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("TestSuite", 0, 0);
    CU_add_test(suite, "Allocation and freeing test", allocate_and_free_test);
    CU_add_test(suite, "String representation test", string_repr_test);
    CU_add_test(suite, "Zeroes test", test_zeroes);
    CU_add_test(suite, "Random Initialisation Test", initialise_polys);
    CU_add_test(suite, "Sum Test", test_sum);
    CU_add_test(suite, "Scalar Product Test", test_scalar_product);
    CU_add_test(suite, "Derivative Test", test_derivative);
    CU_add_test(suite, "Integral Test", test_integral);
    CU_add_test(suite, "Writing to a File Test", test_write_to);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}