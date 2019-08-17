{ stdenv, ncurses, gcc }:

stdenv.mkDerivation {
  name="cligraph";
  srcs=./.;
  buildInputs = [
    gcc
    ncurses
  ];
  installPhase =
    ''
      mkdir -p $out/{lib,bin}
      cp -r lib $out
      cp -r bin $out
    '';
}

