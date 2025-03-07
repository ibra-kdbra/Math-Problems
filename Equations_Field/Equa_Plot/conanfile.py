import os

from conan import ConanFile
from conan.tools.files import copy


class ImGuiExample(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("imgui/1.90.5")
        self.requires("glfw/3.3.8")
        self.requires("glew/2.2.0")
        self.requires("implot/0.16")
        self.requires("exprtk/0.0.3")

    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
             "res", "bindings"), os.path.join(self.source_folder, "bindings"))

