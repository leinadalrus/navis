{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "pale-noel";
  src = ./.;

  nativeBuildInputs = with pkgs.buildPackages; [ pkg-config ];
  buildInputs = with pkgs.buildPackages; [
    boost
    gcc
    glew
    glfw
    miniaudio
    neovim
    raylib
    vim
  ];

  buildPhase = "c++ -std=c++14 -o pale-noel pale-noel.cc";
  installPhase = ''
    mkdir -p $out/build pale-noel $out/bin/
  '';
}

## Run with shell command:
# nix-shell -p --pure \
#   pkg-config emscripten gcc \
#   glew glfw hidapi \
#   libusb1 miniaudio raylib

