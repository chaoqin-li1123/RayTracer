load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    deps = [":camera"],
    linkopts = ["-lpthread"]
)

cc_library(
    name = "camera",
    hdrs = ["camera.h"],
    deps = [":world"]
)

cc_library(
    name = "world",
    hdrs = ["world.h"],
    deps = [":ray", ":background", ":sphere"]
)

cc_library(
    name = "sphere",
    hdrs = ["sphere.h"],
    deps = [":material"]
)

cc_library(
    name = "material",
    hdrs = ["material.h"],
    deps = [":hittable", ":texture"]
)

cc_library(
    name = "hittable",
    hdrs = ["hittable.h"],
    deps = [":ray"]
)

cc_library(
    name = "background",
    hdrs = ["background.h"],
    deps = [":ray"]
)

cc_library(
    name = "texture",
    hdrs = ["texture.h"],
    deps = [":vec3"]
)

cc_library(
    name = "ray",
    hdrs = ["ray.h", "stb_image.h"],
    deps = [":vec3"]
)

cc_library(
    name = "vec3",
    hdrs = ["vec3.h"],
    deps = [":utility"]
)

cc_library(
    name = "utility",
    hdrs = ["utility.h"]
)