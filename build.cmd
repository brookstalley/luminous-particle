set device=light1
if not "%~1"=="" (
  SET device=%1
)
cd \particle\firmware\main
make all PLATFORM=photon APPDIR=../../luminous-particle && particle flash %device% \particle\firmware\modules\luminous-particle\target\luminous-particle.bin
cd \particle\luminous-particle
