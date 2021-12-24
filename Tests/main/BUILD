

cc_test(
    name = "test-r",
    srcs = ["test.cpp"],
    copts = ["-Iexternal/gtest/include"],
    deps = ["@googletest//:gtest_main",
           "//lib:SIM800L"],
)
