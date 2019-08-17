{pkgs ? import <nixpkgs> {}}:

{ 
  cligraph = pkgs.callPackage ./default.nix {}; 
}
