#include "poly.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

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

int main()
{
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("TestSuite", 0, 0);
    CU_add_test(suite, "Allocation and freeing test", allocate_and_free_test);
    CU_add_test(suite, "String representation test", string_repr_test);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}