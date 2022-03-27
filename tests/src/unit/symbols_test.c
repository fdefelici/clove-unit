#define CLOVE_SUITE_NAME SymbolsTest
#include <clove-unit.h>

CLOVE_TEST(ValidateTest) {
    __clove_string_view_t suite_vw = __clove_string_view_from_str("MySuite");
    __clove_string_view_t type_vw = __clove_string_view_from_str("20");
    __clove_string_view_t name_vw = __clove_string_view_from_str("Test01");

    __clove_symbols_context_t ctx;
    bool pass;

    __clove_vector_t includes;
    ctx.includes = &includes;

    includes.count = 0;
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_vector_params_t params = __clove_vector_params_defaulted(sizeof(__clove_include_expr_t));
    __clove_vector_init(&includes, &params);
    __clove_vector_add_empty(&includes);

    __clove_include_expr_t expr;
    __clove_include_expr_init(&expr, "MySuite.Test01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_TRUE(pass);

    __clove_include_expr_init(&expr, "MySuite.Test02");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);

    __clove_include_expr_init(&expr, "YourSuite.Test01");
    __clove_vector_set(&includes, 0, &expr);
    pass = __clove_symbols_function_validate(&suite_vw, &type_vw, &name_vw, &ctx);
    CLOVE_IS_FALSE(pass);
}

