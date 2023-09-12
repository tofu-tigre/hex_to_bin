load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
    name = "status_macros",
    commit = "1592ab2d4b4f92976fc3f4a6cb3a1323a4b549c3",
    remote = "https://github.com/jimrogerz/status_macros.git",
)

http_archive(
  name = "com_google_absl",  # 23-09-07
  urls = ["https://github.com/abseil/abseil-cpp/archive/3cb94be9bea2eda6dd2d572d1cf074efcc48a20b.zip"],
  strip_prefix = "abseil-cpp-3cb94be9bea2eda6dd2d572d1cf074efcc48a20b",
)

# Bazel Skylib.
http_archive(
  name = "bazel_skylib",  # 2023-05-31
  strip_prefix = "bazel-skylib-288731ef9f7f688932bd50e704a91a45ec185f9b",
  urls = ["https://github.com/bazelbuild/bazel-skylib/archive/288731ef9f7f688932bd50e704a91a45ec185f9b.zip"],
)

http_archive(
  name = "com_google_googletest",  # 23-09-07
  urls = ["https://github.com/google/googletest/archive/843976e4f582ccb76cf87e0f128585324335779b.zip"],
  strip_prefix = "googletest-843976e4f582ccb76cf87e0f128585324335779b",
)

http_archive(
    name = "com_github_google_benchmark",  # 23-09-07
    urls = ["https://github.com/google/benchmark/archive/02a354f3f323ae8256948e1dc77ddcb1dfc297da.zip"],
    strip_prefix = "benchmark-02a354f3f323ae8256948e1dc77ddcb1dfc297da",
)

http_archive(
    name = "com_github_gflags_gflags",
    sha256 = "34af2f15cf7367513b352bdcd2493ab14ce43692d2dcd9dfc499492966c64dcf",
    strip_prefix = "gflags-2.2.2",
    urls = ["https://github.com/gflags/gflags/archive/v2.2.2.tar.gz"],
)

http_archive(
    name = "com_github_google_glog",
    sha256 = "122fb6b712808ef43fbf80f75c52a21c9760683dae470154f02bddfc61135022",
    strip_prefix = "glog-0.6.0",
    urls = ["https://github.com/google/glog/archive/v0.6.0.zip"],
)
