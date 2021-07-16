with import <nixpkgs> {};

let

cov_test = (target: env: env.mkDerivation {
  name = "rodos-${target}";
  src = ./.;
  buildInputs = [cmake lcov];
  cmakeFlags = [
    "-DCMAKE_TOOLCHAIN_FILE=../cmake/port/${target}.cmake"
    "-DEXECUTABLE=ON"
    "-DCOVERAGE=ON"
  ];
  installPhase =
    ''
      make coverage_collect test-report
      mkdir $out
      mv coverage $out/
      mv coverage.info $out/
      mv test-suite/test-report.txt $out/
    '';
});

compile_only = (target: env: env.mkDerivation {
  name = "rodos-${target}";
  src = ./.;
  nativeBuildInputs = [cmake];
  cmakeFlags = [
    "-DCMAKE_TOOLCHAIN_FILE=../cmake/port/${target}.cmake"
    "-DEXECUTABLE=ON"
  ];
  installPhase =
    ''
      mkdir $out
      touch $out/did-compile
    '';
});

in

{
  linux-x86 = cov_test "linux-x86" pkgsi686Linux.stdenv;
  linux-makecontext = cov_test "linux-makecontext" pkgsi686Linux.stdenv;
  posix = cov_test "posix" pkgsi686Linux.stdenv;
  discovery = compile_only "discovery" pkgsCross.arm-embedded.stdenv;
  skith = compile_only "skith" pkgsCross.arm-embedded.stdenv;
  raspberrypi3 = compile_only "raspberrypi3" pkgsCross.arm-embedded.stdenv;
  sf2 = compile_only "sf2" pkgsCross.arm-embedded.stdenv;

}
