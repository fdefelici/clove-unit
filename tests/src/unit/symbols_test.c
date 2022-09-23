#define CLOVE_SUITE_NAME UNIT_SymbolsTest
#include <clove-unit.h>

CLOVE_TEST(ValidateEqualityRule) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;
    
    __clove_vector_t includes = __clove_vector_null();
    __clove_vector_t excludes = __clove_vector_null();
    ctx.includes = &includes;
    ctx.excludes = &excludes;

    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_expr_t));
    __clove_vector_init(&includes, &params);
    __clove_vector_add_slot(&includes);

    __clove_test_expr_t expr;
    __clove_test_expr_init(&expr, "MySuite.Test01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "MySuite.Test02");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_test_expr_init(&expr, "YourSuite.Test01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_vector_free(&includes);
}

CLOVE_TEST(ValidateSuiteRule) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;

    __clove_vector_t includes = __clove_vector_null();
    __clove_vector_t excludes = __clove_vector_null();
    ctx.includes = &includes;
    ctx.excludes = &excludes;

    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_expr_t));
    __clove_vector_init(&includes, &params);
    __clove_vector_add_slot(&includes);

    __clove_test_expr_t expr;
    __clove_test_expr_init(&expr, "MySuite");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "MySuit*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

     __clove_test_expr_init(&expr, "*ySuite");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "*ySuit*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_free(&includes);
}

CLOVE_TEST(ValidateTestRule) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;

    __clove_vector_t includes = __clove_vector_null();
    __clove_vector_t excludes = __clove_vector_null();
    ctx.includes = &includes;
    ctx.excludes = &excludes;

    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_expr_t));
    __clove_vector_init(&includes, &params);
    __clove_vector_add_slot(&includes);

    __clove_test_expr_t expr;
    __clove_test_expr_init(&expr, "MySuite.*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "*.Test01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

     __clove_test_expr_init(&expr, "*.*01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "*.*est*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_test_expr_init(&expr, "MySuite.Test*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_free(&includes);
}

CLOVE_TEST(ValidateRuleProblems) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;

    __clove_vector_t includes = __clove_vector_null();
    __clove_vector_t excludes = __clove_vector_null();
    ctx.includes = &includes;
    ctx.excludes = &excludes;

    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_expr_t));
    __clove_vector_init(&includes, &params);
    __clove_vector_add_slot(&includes);

    __clove_test_expr_t expr;
    
    __clove_test_expr_init(&expr, "**");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_test_expr_init(&expr, "*.**");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

     __clove_test_expr_init(&expr, ".");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_test_expr_init(&expr, "..");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_test_expr_init(&expr, "*..*");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

     __clove_test_expr_init(&expr, "*.");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_vector_free(&includes);
}


CLOVE_TEST(ValidateExclusionRule) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;

    __clove_vector_t includes = __clove_vector_null();
    __clove_vector_t excludes = __clove_vector_null();
    ctx.includes = &includes;
    ctx.excludes = &excludes;
    
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_test_expr_t));
    __clove_vector_init(&excludes, &params);
    __clove_vector_add_slot(&excludes);

    __clove_test_expr_t expr;
    
    __clove_test_expr_init(&expr, "*");
    __clove_vector_set(&excludes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_vector_free(&excludes);
}