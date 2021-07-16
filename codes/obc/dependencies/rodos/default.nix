with import <nixpkgs> {};

# to build rodos and create a tgz archive with the librodos.a and all header files.  
# It does so for a combination of targets/platforms: linux, posix, stm32f4, raspberry pi


let doit = (target: env: env.mkDerivation {
 name = "rodos-${target}";
 src = ./.;

 buildPhase =
  ''
  patchShebangs scripts/**/rodos-lib.sh
  patchShebangs scripts/**/rodos-executable.sh
  source setenvs.sh
  rodos-lib.sh ${target}
  if ! stat `find build -name librodos.a` &>/dev/null
  then
    echo "librodos.a not built!"
    exit 1
  fi
  pushd tutorials/10*
  rodos-executable.sh ${target} helloworld.cpp
  if ! stat tst &>/dev/null
  then
    echo "helloworld test program could not be compiled"
    exit 1
  fi
  popd
  '';

 installPhase =
  ''
  mkdir staging
  mkdir staging/lib
  mkdir staging/include
  mv build/libs/* staging/lib/
  mv api staging/include/rodos
  cd src
  find -L . -name "*.h" -exec bash -c 'mkdir -p ../staging/include/rodos/$(dirname $0); mv $0 ../staging/include/rodos/$0' '{}' ';'
  cd ..
  RODOS_NAME=rodos-${target}-${env.hostPlatform.system}
  mv staging $RODOS_NAME
  tar -caf ''${RODOS_NAME}.tgz $RODOS_NAME
  mkdir $out
  mv ''${RODOS_NAME}.tgz $out/
  '';
});

in

  map (cfg: doit (builtins.elemAt cfg 0) (builtins.elemAt cfg 1))
  [
  ["linux-makecontext" pkgsi686Linux.stdenv]
  ["linux-x86" pkgsi686Linux.stdenv]
  ["on-posix" pkgsi686Linux.stdenv]
  ["on-posix64" stdenv]
  ["discovery" pkgsCross.arm-embedded.stdenv]
  ["skith" pkgsCross.arm-embedded.stdenv]
  ["raspberrypi3" pkgsCross.arm-embedded.stdenv]
  ["sf2" pkgsCross.arm-embedded.stdenv]
  ]
