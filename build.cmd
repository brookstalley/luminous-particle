clear
cd \particle\firmware\main
make all PLATFORM=photon APPDIR=../../luminous-particle && particle flash light1 \particle\firmware\modules\luminous-particle\target\luminous-particle.bin
cd \particle\luminous-particle
