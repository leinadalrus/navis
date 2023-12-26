with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "pale-noel";
  src = ./.;

  nativeBuildInputs = [ pkg-config ];
  buildInputs = [
    boost
    gcc
    glew
    glfw
    miniaudio
    raylib
  ];

  buildPhase = "gcc -std=c++14 -o pale-noel pale-noel.cc";
  installPhase = ''
    mkdir -p $out/build pale-noel $out/bin/
  '';
}

## Build the default.nix file with nix-build command arguments:
# nix-build
# ./result/bin/pale-noel
